#ifndef INETUSER_H_
#define INETUSER_H_

#include <omnetpp.h>
#include <string>
#include <map>
#include <crng.h>
#include <cstringtokenizer.h>
#include <iostream>
#include <ostream>
#include "ModuleAccess.h"
#include "GenericApplication.h"
#include "TrafficProfileManager.h"
#include "ConnectionManager.h"
#include "SCADASIMDefs.h"

class GenericApplication;

/**
 * @brief Implementation of a client that generates background traffic.
 *
 * This class implements the client functionality. Clients start a first
 * transmission at startTime. Each time a new transmission is started
 * a random traffic profile and a server are requested at the connection
 * manager.
 *
 * In addition, clients may register at the connection manager for Ping
 * and UDPMisc traffic.
 *
 * @class InetUser
 */
class SCADASIM_API InetUser: public cSimpleModule
{
protected:
	/// Map of available applications
	std::map<int, GenericApplication *> applications;
	/// Connection manager for requesting a new traffic profile
	ConnectionManager *cm;
	/// Counters for the number of profiles based on the application protocol
	/// @{
	int noICMPProfile, noUDPProfile, noTCPProfile;
	/// @}
	/// ID of currently used traffic profile
	int previousProfileId;
	// ID of pre-defined attachment traffic profile.
	int attachedProfileId;
	/// Currently used traffic profile
	TrafficProfile curTrafficProfile;
	/// Currently used server communication endpoint
	TargetInfo curTargetInfo;
	/// Container for collection of communication statistics
	UserCommunicationStatistics communicationStatistics;

public:
	InetUser();
	virtual ~InetUser();
	/// @brief Collects some statistics of the previous transmission and starts a new one 
	void transmissionDone(TransmissionStatistics t);
	/// @brief Sets application type to the given value
	void setApplication(int applicationType, GenericApplication *a, int attachedProfileNumber);
  
  
protected:
	virtual void initialize();
	/// @brief Actually starts the client at startTime
	virtual void handleMessage(cMessage *msg);
	/// @brief Actually starts a new transmission
	void transmissionDone();
	/// @brief Updates the information displayed in the GUI
	void updateDisplay();
	/// @brief Currently, nothing is done here
	virtual void finish();
};

/**
 * @brief Gives access to the InetUser instance within the host (client/server).
 *
 * @class InetUserAccess
 */ 
class SCADASIM_API InetUserAccess : public ModuleAccess<InetUser>
{
  public:
    InetUserAccess() : ModuleAccess<InetUser>("inetUser"){}
};

#endif /*INETUSER_H_*/
