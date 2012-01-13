#include "ConnectionManager.h"

/**
 * @brief Connection manager is responsible for requesting traffic profiles and
 * server connection endpoints.
 *
 * A single global connection manager is available, where all local connection
 * managers have to register. In addition, all servers of the topology register
 * with their local connection manager. Then, the connection manager can be
 * asked to request a traffic profile and finally, searches for an according
 * server in its AS or in a foreign AS (depending on the traffic profile).
 *
 * @class ConnectionManager
 */

using namespace ::std;

#define SIMULATION_END 102

/// Set to 1 if all servers and AS have a uniform probability
#define UNIFORM_PROB 1

Define_Module( ConnectionManager);

ConnectionManager::ConnectionManager()
{
	topCM = NULL;
	serverProbabilityIsDirty = networkProbabilityIsDirty = false;
	tpm = NULL;
	isTopCM = false;
	isFlatNet = true;
	endSimulationMessage = NULL;
}

ConnectionManager::~ConnectionManager()
{
	serverMap::iterator it = servers.begin();
	while (it != servers.end())
	{
		serverVector *vec = it->second;
		for (unsigned int i = 0; i < vec->size(); i++)
			delete (*vec)[i];
		delete vec;
		it++;
	}
	servers.clear();
	if (!subCM.empty())
	{
		for (unsigned int i = 0; i < subCM.size(); i++)
			delete subCM[i];
		subCM.clear();
	}
	if (endSimulationMessage != NULL)
	{
		if (endSimulationMessage->isScheduled())
			cancelAndDelete(endSimulationMessage);
		else
			delete endSimulationMessage;
	}
}

/**
 * Initialize method that starts registration of connection manager and
 * sets timer for simulation end message.
 */
void ConnectionManager::initialize()
{
	// first determine top connection manager and register with it
	registerConnectionManager(getId());

	// find TrafficProfileManager
	TrafficProfileManagerAccess tma;
	tpm = tma.get();
	if (tpm == NULL)
		opp_error("couldn't get TrafficProfileManager");

	// set timer for simulation end
	endSimulationMessage = new cMessage("simulationEndTimer");
	endSimulationMessage->setKind(SIMULATION_END);
	scheduleAt(par("simulationDuration"), endSimulationMessage);
}

/**
 * This method is called by servers of a single AS during simulation startup.
 * Each server registers with its AS connection manager and gives all
 * necessary information (address, port, profile ID, and module ID).
 */
void ConnectionManager::registerServer(IPvXAddress address, int port, int profileId, int moduleId)
{
	//Enter_Method_Silent();
	//Enter_Method("registerServer");
	if (isTopCM)
		isFlatNet = true;

	std::cout << "Registering server for profile: " << profileId << ", address: " << address.str() << ", port: " << port << std::endl;

	serverMap::iterator it = servers.find(profileId);
	// if profileID is unknown add a new empty serverVector
	if (it == servers.end())
		it = servers.insert(it, std::pair<int, serverVector*>(profileId, new serverVector()));

	// add given server to the serverVector of the profile ID
	serverVector *svector = it->second;
	// Application of priorities not yet implemented. Thus, all servers
	// have the same selection probability (value of 1).
	svector->push_back(new serverConnection(address, port, profileId, 1, moduleId, this));
	serverProbabilityIsDirty = true;

	updateDisplay();
}

/**
 * Request random traffic profile from traffic profile manager. Then, an
 * according server is selected. If a local server is requested and none is
 * available, request a foreign server for communication.
 *
 * @param ti Target info will be filled with the information of the selected server
 * @param tp Traffic profile will be filled with information from traffic
 *           profile manager
 * @param except Traffic profile ID that should not be selected
 * @param moduleId ID of the calling module
 */
void ConnectionManager::getServer(TargetInfo &ti, TrafficProfile &tp, int &except, int moduleId)
{
	double profileProb, foreignServerProb;
	bool isForeign = false;

	// ask TrafficProfileManager for traffic profile
	if (except == -1)
	{
		foreignServerProb = uniform(0, 1, 1);
		profileProb = uniform(0, 1, 1);
		tpm->getTrafficProfile(tp, profileProb, foreignServerProb, isForeign);
	}
	else
	{
		while (tp.profileID == except)
		{
			foreignServerProb = uniform(0, 1, 1);
			profileProb = uniform(0, 1, 1);
			tpm->getTrafficProfile(tp, profileProb, foreignServerProb, isForeign);
		}
	}
	except = tp.profileID;
	
	// check, if connectionManager can handle current profile or if he has to
	// ask a foreign connectionManager
	//

	if (isForeign && topCM)
	{
		if (topCM->isFlatNet)
			haveProfile(ti, tp.profileID, getId());
		else
			// ask topCM for profile
			topCM->getForeignServer(ti, tp.profileID, getId());
	}
	// if local server is requested but not available, request foreign server
	else if (!haveProfile(ti, tp.profileID, moduleId))
	{
		if (topCM)
		{
			if (topCM->isFlatNet)
				cerr << "getServer: no local server could be found in flat topology" << endl;
			else
				topCM->getForeignServer(ti, tp.profileID, getId());
		}
		else
			opp_error("couldn't get server - no TopCM available\n");
	}
}

/**
 *b
 */
void ConnectionManager::getServer(TargetInfo &ti, TrafficProfile &tp, int moduleId){
	//double foreignServerProb;
	bool isForeign = false;

	tpm->getFixTrafficProfile(tp,isForeign);

	if (isForeign && topCM)
		{
			if (topCM->isFlatNet)
				haveProfile(ti, tp.profileID, getId());
			else
				// ask topCM for profile
				topCM->getForeignServer(ti, tp.profileID, getId());
		}
		// if local server is requested but not available, request foreign server
		else if (!haveProfile(ti, tp.profileID, moduleId))
		{
			if (topCM)
			{
				if (topCM->isFlatNet)
					cerr << "getServer: no local server could be found in flat topology" << endl;
				else
					topCM->getForeignServer(ti, tp.profileID, getId());
			}
			else
				opp_error("couldn't get server - no TopCM available\n");
		}


}

/**
 * Request server of other AS for current traffic profile
 *
 * Randomly selects a foreign AS for the connection endpoint. Within the selected
 * AS, a server is chosen for the given profile ID.
 *
 * @param ti Target info will be filled with the information of the selected server
 * @param profileId Profile ID to select a server for
 * @param moduleId ID of the calling module
 */
void ConnectionManager::getForeignServer(TargetInfo &ti, int profileId, int moduleId)
{
	Enter_Method_Silent();
	//Enter_Method("GetForeignServer");

	if (!isTopCM)
	{
		opp_error("asking non-TopCM for foreignServer\n");
		return;
	}

	if (networkProbabilityIsDirty)
		updateNetworkProbabilities();

	bool goOn = true;
	int runs = 0;
	double networkProb;
	double cumProb;
	unsigned int i;
	while (goOn && runs < 100)
	{
		runs++;
		// Hack to fasten simulations in case of a uniform selection probability of all AS
		if (UNIFORM_PROB == 1)
		{
			networkProb = uniform(0, subCM.size(), 1);
			i = floor(networkProb);
		}
		else
		{
			networkProb = uniform(0, 1, 1);
			cumProb = 0.;
			for (i = 0; i < subCM.size(); i++)
			{
				cumProb += subCM[i]->probability;
				if (cumProb >= networkProb)
					break;
			}
		}

		// i is the index of the choosen network
		// if it points to the network that's asking - redo
		//
		if (subCM[i]->id == moduleId)
			continue;
		//
		//check if the network has a server for current profile
		//and return the profile written in ti
		if (!subCM[i]->manager->haveProfile(ti, profileId)) {
		    cout << "no server for profile " << profileId << " found in AS " << subCM[i]->manager->getFullPath().c_str() << endl;
			continue;
		}
		goOn = false;
	}

	if (runs == 100 && goOn) {
	        // ensure that target info is invalid
	        // this is necessary in case only a single AS exists -> haveProfile is never called within this method and thus, ti is not set to invalid state else
		ti.address = IPvXAddress();
		ti.port = -1;
	    cerr << "getForeignServer: no foreign server for profile " << profileId << " could be found (tried 100 times)" << endl;
	}
}

/**
 * Selects a server for a given traffic profile
 *
 * If the selected server is equal to the given module id (calling module)
 * an other server will be selected.
 *
 * @param ti Target info will be filled with the information of the selected server
 * @param profileId Profile ID to select a server for
 * @param moduleId ID of the calling module
 */
bool ConnectionManager::haveProfile(TargetInfo &ti, int profileId, int moduleId)
{
	Enter_Method_Silent();
	//Enter_Method("haveProfile");

	if (isTopCM && !isFlatNet)
		opp_error("asking TopCM for ServerProfile\n");

	if (serverProbabilityIsDirty)
		updateServerProbabilities();

	serverMap::iterator it = servers.find(profileId);
	if (it == servers.end())
	{
	        // set TargetInfo to unspecified
		ti.address = IPvXAddress();
		ti.port = -1;
		return false;
	}

	serverVector *svector = it->second;
	bool goOn = true;
	int runs = 0;
	double cumProb;
	double serverProb;
	unsigned int i;
	while (goOn && runs < 100)
	{
		runs++;
		// Hack to fasten simulations in case of a uniform selection probability of all AS
		if (UNIFORM_PROB == 1)
		{
			serverProb = uniform(0, svector->size(), 1);
			i = floor(serverProb);
		}
		else
		{
			cumProb = 0.;
			serverProb = uniform(0, 1, 1);
			for (i = 0; i < svector->size(); i++)
			{
				cumProb += (*svector)[i]->probability;
				if (cumProb >= serverProb)
					break;
			}
		}

		if (((*svector)[i]->id == moduleId) && (moduleId != -1))
			continue;
		goOn = false;

		// set TargetInfo
		ti.address = (*svector)[i]->address;
		ti.port = (*svector)[i]->port;
		//cout << "found server for profile " << profileId << ": address " << (*svector)[i]->address.str() << ", port " << (*svector)[i]->port << std::endl;
	}

	if (runs == 100 && goOn)
	{
		cerr << "haveProfile() could not find a server within this AS (tried 100 times)" << endl;
		// set TargetInfo to unspecified
		ti.address = IPvXAddress();
		ti.port = -1;
		return false;
	}

	return true;
}

/**
 * If a SIMULATION_END message is received endSimulation() is called
 * and the simulation is stopped. Other messages cause an error message.
 *
 * @param msg Reveived message
 */
void ConnectionManager::handleMessage(cMessage *msg)
{
	if (msg->getKind() != SIMULATION_END)
		opp_error("ConnectionManager receives unexpected message");
	//
	// stop all
	//
	endSimulation();
}

/**
 * Registration of connection manager with global connection manager.
 * If there is none yet, registers itself as topCM.
 *
 * @param id Module ID of the calling connection manager
 */
void ConnectionManager::registerConnectionManager(int id)
{
	//
	// if we have already registered do nothing
	if (isTopCM || (topCM != NULL))
		return;

	// get global connection manager
	cModule *mod = NULL;
	for (cModule *curMod = getParentModule()->getParentModule(); !mod && curMod; curMod = curMod->getParentModule())
		mod = curMod->getSubmodule("connectionManager");
	if (mod && (mod != this))
		topCM = check_and_cast<ConnectionManager *> (mod);

	if (topCM == NULL)
	{
		EV<< "couldn't find a topCM: "<<getFullPath() <<endl;
		EV << "thus, registering myself as topCM"<<endl;
		isTopCM = true;
		topCM = this;
	}
	else
	{
		topCM->addSubCM(this, id);
	}
}

		/**
		 * Registers a local connection manager with the global connection manager
		 *
		 * @param other Registering local connection manager
		 * @param id Module ID of registering local connection manager
		 */
void ConnectionManager::addSubCM(ConnectionManager *other, int id)
{
	//Enter_Method_Silent();
	if (!isTopCM)
	{
		EV<< "registration of subCM failed since I'm not a topCM"<<endl;
		return;
	}

	isFlatNet = false;

	foreignNetwork *newNetwork = new foreignNetwork(other, id, 1, this);
	subCM.push_back(newNetwork);
	networkProbabilityIsDirty = true;

	updateDisplay();
}

		/**
		 * Registers a local connection manager with the global connection manager
		 *
		 * @param network foreignNetwork structure of registering connection manager
		 */
void ConnectionManager::addSubCM(foreignNetwork *network)
{
	//Enter_Method_Silent();

	if (!isTopCM)
	{
		EV<< "registration of subCM failed since I'm not a topCM"<<endl;
		return;
	}

	isFlatNet = false;

	isTopCM = true;
	subCM.push_back(network);
	networkProbabilityIsDirty = true;

	updateDisplay();
}

		/**
		 * Normalizes the selection probabilities of all servers to 100%
		 * after a new server was added or an existing server was removed.
		 *
		 * Called only by the local connection managers.
		 */
void ConnectionManager::updateServerProbabilities()
{
	serverProbabilityIsDirty = false;

	if (isTopCM && !isFlatNet)
		opp_error("invalid state: i'm topCM and have no server list!");

	serverMap::iterator it = servers.begin();
	while (it != servers.end())
	{
		serverVector *vec = it->second;
		double cum_ratio = 0.;
		for (unsigned int i = 0; i < vec->size(); i++)
			cum_ratio += (*vec)[i]->ratio;

		for (unsigned int i = 0; i < vec->size(); i++)
			(*vec)[i]->probability = (*vec)[i]->ratio / cum_ratio;
		it++;
	}
}

/**
 * Normalizes the selection probabilities of all AS to 100%
 * after a new AS was added or an existing AS was removed.
 *
 * Called only by the global connection manager.
 */
void ConnectionManager::updateNetworkProbabilities()
{
	networkProbabilityIsDirty = false;

	if (!isTopCM)
		opp_error("invalid state: i'm not topCM and have no network list");

	double cum_ratio = 0.;
	for (unsigned int i = 0; i < subCM.size(); i++)
		cum_ratio += subCM[i]->ratio;

	for (unsigned int i = 0; i < subCM.size(); i++)
		subCM[i]->probability = subCM[i]->probability / cum_ratio;
}

/**
 * Updates the description within the INET GUI
 */
void ConnectionManager::updateDisplay()
{
	if (!ev.isGUI())
		return;

	char buf[60];
	sprintf(buf, "got %u %s", isTopCM ? subCM.size() : servers.size(), isTopCM ? "networks" : "server profiles");
	getDisplayString().setTagArg("t", 0, buf);
}
