#ifndef GENERICAPPLICATION_H_
#define GENERICAPPLICATION_H_

#include <omnetpp.h>
#include <cstdlib>
#include "InetUser.h"
#include "ConnectionManager.h"
#include "TransmissionConfig.h"
#include "SCADASIMDefs.h"
#include <string>

using namespace std;

/// Message Kind
/// @{
#define UNDEF 0
#define CONNECT 1
#define SEND 2
#define RESET 3
///@}

// first initialization stage
#define INITIALIZATION_STAGE_NECESSARY 4

// Self-Timer configuration
#define SELF_CALL_DELAY 0.001

//#define CALL_ME(msg) scheduleAt(simTime() + SELF_CALL_DELAY, msg)
//#define CALL_ME(msg, time) scheduleAt(simTime()+time, msg)

class InetUser;

/**
 * @brief Abstract class for implementation of applications
 *
 * This class provides methods for the implementation of
 * applications that are used within the simulation in order
 * to generate realistic, self-similar background traffic.
 *
 * @class GenericApplication
 */
class SCADASIM_API GenericApplication : public cSimpleModule
{
protected:
	/// Pointer to the InetUser in case of a client
	InetUser *user;
	/// Currently used traffic profile
	TrafficProfile curProfile;
	/// Pointer to the connection manager
	ConnectionManager *cm;
	/// Indicates if host is a server of a client
	bool isServer;
	/// Number of associated profile
	long profileNumber;
	/// Port number of associated profile
	long port;
	/// Type of the associated application
	int applicationType;
	bool pingOn, udpOn;
	
public:
	/// Application type has to be set in constructor
	GenericApplication();
	virtual ~GenericApplication();
	/// Virtual method for starting a communication
	virtual void transmissionStart(TrafficProfile &p, TargetInfo &i) = 0;
	/// Registering server at the connection manager
	void registerServer(int profileId, int port);

protected:
	virtual int numInitStages() const {return INITIALIZATION_STAGE_NECESSARY+1; };
	/// Initialization: Registering at the connection manager
	virtual void initialize(int stages);
	/// Creates communication statistics
	virtual void transmissionDone(TransmissionStatistics s);

};

#endif /*GENERICAPPLICATION_H_*/
