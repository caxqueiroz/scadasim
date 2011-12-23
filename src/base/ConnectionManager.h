#ifndef CONNECTIONMANAGER2_H_
#define CONNECTIONMANAGER2_H_

#include <omnetpp.h>
#include "ReaSEDefs.h"
#include "IPvXAddress.h"
#include "TransmissionConfig.h"
#include "ModuleAccess.h"
#include "TrafficProfileManager.h"

class ConnectionManager;

// Helper Structs

/**
 * @brief Structure containing all information regarding a single server
 * within the AS.
 *
 * This structure saves the server's address and port, the according traffic
 * profile ID, its module ID, and the connection manager of this AS.
 *
 * The variable ratio is for future use in order to give some servers a higher
 * selection probability (achieves priority support).
 */
struct serverConnection
{
	IPvXAddress address;
	int port;
	int profileID;
	// Used only for priorities
	int ratio;
	// Module ID
	int id;
	// Selection probability of this server within its profile (currently uniformly distributed)
	// Could be used, e.g. to simulate flash crowd events
	double probability;
	ConnectionManager *manager;

	serverConnection(IPvXAddress a, int port, int profileID, int ratio, ConnectionManager *cm)
	{
		address = a;
		this->port = port;
		this->profileID = profileID;
		this->ratio = ratio;
		manager = cm;
		id = -1;
	}

	serverConnection(IPvXAddress a, int port, int profileID, int ratio, int moduleId, ConnectionManager *cm)
	{
		address = a;
		this->port = port;
		this->profileID = profileID;
		this->ratio = ratio;
		manager = cm;
		this->id = moduleId;
	}
};

/**
 * @brief Structure containing all information regarding the local
 * connection managers of the topology.
 *
 * This structure saves the CM's module ID, a pointer to the CM instance,
 * and a pointer to the topCM instance. The structure is used by the topCM
 * for management of the local CMs.
 *
 * The variable ratio is for future use in order to give some Cms a higher
 * selection probability (achieves priority support).
 */
struct foreignNetwork
{
	ConnectionManager *manager;
	// Used only for priorities
	int ratio;
	// Module ID
	int id;
	// Selection probability of this AS (currently uniformly distributed)
	double probability;
	ConnectionManager *topManager;

	foreignNetwork(ConnectionManager *c, int moduleId, int ratio, ConnectionManager *topCM)
	{
		manager = c;
		this->ratio = ratio;
		topManager = topCM;
		this->id = moduleId;
		this->probability = ratio;
	}
};

//
// TODO: change Server and Network probability dynamically
// declare them as friends and create setter Methods in the structs
//
typedef std::vector<serverConnection*> serverVector;
typedef std::map<int, serverVector*> serverMap;
typedef std::vector<foreignNetwork*> networkVector;

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
class REASE_API ConnectionManager: public cSimpleModule
{
protected:
	/// Used by CM: Map of servers registered within own AS
	serverMap servers;
	/// Used by the topCM: List of all registered local connection managers
	networkVector subCM;
	/// Global Connection Manager
	ConnectionManager *topCM;
	/// State variables indicating that probability has to be re-calculated
	/// @{
	bool serverProbabilityIsDirty, networkProbabilityIsDirty;
	/// }@
	/// Indicates that this connection manager is the global one
	bool isTopCM;
	/// Indicates that the managed topology is flat (only true AS-level topology only)
	bool isFlatNet;
	/// Set traffic profile manager to request profiles
	TrafficProfileManager *tpm;
	/// Message necessary to indicate simulation end to connection manager
	cMessage *endSimulationMessage;


public:
	ConnectionManager();
	virtual ~ConnectionManager();
	/// @brief Server registration with connection manager in its own AS
	void registerServer(IPvXAddress address, int port, int profileID, int moduleId = -1);
	/// @brief Request traffic profile and according server
	void getServer(TargetInfo &ti, TrafficProfile &tp, int &expect, int moduleId = -1);
	/// @brief Request server of other AS for current traffic profile
	void getForeignServer(TargetInfo &ti, int profileId, int moduleId);
	/// @brief Selects a server for a given traffic profile
	bool haveProfile(TargetInfo &ti, int profileId, int moduleId = -1);

	void getServer(TargetInfo &ti, TrafficProfile &tp, int moduleId);

protected:
	virtual void initialize();
	/// @brief Stop simulation
	virtual void handleMessage(cMessage *msg);
	/// @brief Registers connection manager with topCM or sets it to topCM
	void registerConnectionManager(int moduleId);
	/// @brief Adds a local connection manager to the list of the global connection manager
	void addSubCM(ConnectionManager *other, int moduleId);
	/// @brief Adds a local connection manager to the list of the global connection manager
	void addSubCM(foreignNetwork *network);
	/// @brief Normalization of probabilities if new server is added
	void updateServerProbabilities();
	/// @brief Normalization of probabilities if new AS is added
	void updateNetworkProbabilities();
	/// @brief Updates the description within the INET GUI
	void updateDisplay();
};



/**
 * @brief Gives access to the ConnectionManager instance of parent Autonomous System.
 *
 * @class ConnectionManagerAccess
 */
class REASE_API ConnectionManagerAccess
{
  public:
    ConnectionManagerAccess() {}

    ConnectionManager *get()
    {
	// returns global connectionManager
    	return check_and_cast<ConnectionManager*>(simulation.getModuleByPath("connectionManager"));
    }

    ConnectionManager *get(std::string asName)
    {
	// returns connectionManager of given autonomous system
	asName = asName.append(".connectionManager");
    	return check_and_cast<ConnectionManager*>(simulation.getModuleByPath(asName.c_str()));
    }
};

// cannot use standard ModuleAccess interface, because of changes to its search pattern in omnet 4.0
// search would terminate at first module with @node property and toplevel connectionmanager cannot be found
//
//class REASE_API ConnectionManagerAccess : public ModuleAccess<ConnectionManager>
//{
//  public:
//    ConnectionManagerAccess() : ModuleAccess<ConnectionManager>("connectionManager") {}
//};

#endif /*CONNECTIONMANAGER2_H_*/
