#include "GenericApplication.h"
#include "ConnectionManager.h"
#include "IPvXAddressResolver.h"
#include "IPvXAddress.h"
#include <cassert>

/**
 * @brief Abstract class for implementation of applications
 *
 * This class provides methods for the implementation of
 * applications that are used within the simulation in order
 * to generate realistic, self-similar background traffic.
 *
 * @class GenericApplication
 */
GenericApplication::GenericApplication()
{
	user = NULL;
	applicationType = NOTSET;
}

GenericApplication::~GenericApplication()
{
}

/**
 * Initialization: servers are registered at the connection
 * manager, clients in case of Ping and UDPMisc traffic.
 */
void GenericApplication::initialize(int stages)
{
	if (stages == INITIALIZATION_STAGE_NECESSARY)
	{
		// checks if application was set correctly by sub-classes
		if (!applicationType)
			opp_error("invalid application type: did you forget to set a proper application type in constructor?");

		isServer = par("isServer").boolValue();
		profileNumber = par("profileNumber").longValue();
		port = par("port").longValue();

		// get the connectionManager
		cm = NULL;
		ConnectionManagerAccess cac;
		string asName = getParentModule()->getParentModule()->getFullName();
		if(asName.find("corporate") != string::npos || asName.find("remote") != string::npos || asName.find("field") != string::npos) {
		    // get AS-specific connection manager
		    cm = cac.get(asName);

		} else {
		    // get global connection manager
		    cm = cac.get();
		}
		if (cm == NULL)
		    opp_error("couldn't get ConnectionManager");

		// register servers at ConnectionManager
		if (isServer)
			cm->registerServer(IPvXAddressResolver().resolve(getParentModule()->getFullPath().data()), port, profileNumber);
		// if i'am not a server - this means a client, i have to
		// register myself to the current InetUser
		else
		{
			InetUserAccess uac;
			user = uac.get();
			user->setApplication(applicationType, this,profileNumber);


			// if the client acts additionaly as a Ping Receiver or
			// a receiver for Misc UDP Packet
//			if (profileNumber == ICMP_PING || profileNumber == UDP_STREAMING || profileNumber == UDP_NAMESERVER)
//				cm->registerServer(IPAddressResolver().resolve(getParentModule()->getFullPath().data()), port, profileNumber, user->getId());
			//
			// for client - create a TrafficProfile
			// and set necessary watches
			WATCH(curProfile.requestLength);
			WATCH(curProfile.requestsPerSession);
			WATCH(curProfile.replyLength);
			WATCH(curProfile.replyPerRequest);
			WATCH(curProfile.timeBetweenRequests);
			WATCH(curProfile.timeToRespond);
			WATCH(curProfile.timeBetweenSessions);
			WATCH(curProfile.probability);
			WATCH(curProfile.WANprob);
			WATCH(curProfile.profileID);
		}
	}
}

/**
 * Register server at the connection manager for given
 * profile ID and port.
 *
 * @param profileId Profile ID to register for
 * @param port Port to register for
 */
//void GenericApplication::registerServer(int profileId, int port)
//{
//	assert(cm);
//	cm->registerServer(IPAddressResolver().resolve(getParentModule()->getFullPath().data()), port, profileId);
//}

/*
 * Create transmission statistics after having finished a
 * communication.
 */
void GenericApplication::transmissionDone(TransmissionStatistics s)
{
	if (user)
		user->transmissionDone(s);
}
