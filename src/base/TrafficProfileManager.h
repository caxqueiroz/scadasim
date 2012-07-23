#ifndef TRAFFICPROFILEMANAGER_H_
#define TRAFFICPROFILEMANAGER_H_

#include <vector>
#include <omnetpp.h>
#include "TransmissionConfig.h"
#include "ModuleAccess.h"
#include "SCADASIMDefs.h"

/**
 * @brief Manages available traffic profiles and randomly selects one for
 * the next client/server communication.
 * 
 * The traffic profile manager reads all available traffic profiles from
 * a configuration file and returns a random traffic profile if requested.
 * In addition it decides if the server communication endpoints lies in
 * the current AS or in a foreign AS.
 *
 * @class TrafficProfileManager
 */
class SCADASIM_API TrafficProfileManager : public cSimpleModule
{
private:
	/// Vector of available traffic profiles
	std::vector<TrafficProfile *> profiles;

public:
	TrafficProfileManager();
	virtual ~TrafficProfileManager();
	/// Returns a random traffic profile
	void getTrafficProfile(TrafficProfile &t, double profileProb, double foreignServerProb, bool &isForeign);
	/// Returns a fix traffic profile
	void getFixTrafficProfile(TrafficProfile &t, bool &isForeign);

protected:
	/// Sets initialization stage to 1
	virtual int numInitStages() const { return 1; };
	/// Reads available profiles from config file
	virtual void initialize(int stages);
	/// No message handling is supported
	virtual void handleMessage(cMessage *msg);
};


/**
 * @brief Gives access to the TrafficProfileManager instance.
 *
 * @class TrafficProfileManagerAccess
 */ 
class SCADASIM_API TrafficProfileManagerAccess : public ModuleAccess<TrafficProfileManager>
{
public:
	TrafficProfileManagerAccess() : ModuleAccess<TrafficProfileManager>("trafficProfileManager"){};
};


#endif /*TRAFFICPROFILEMANAGER_H_*/
