#ifndef TRIBEFLOODNETWORK_H_
#define TRIBEFLOODNETWORK_H_

#include <omnetpp.h>
#include "IPvXAddress.h"
#include <vector>
#include "ReaSEDefs.h"

/// Attack tags describing different DDoS attack types
/// @{
const int MIN_ATTACK_TYPE = 1;
const int SYN_FLOODING = 1;
const int SYN_ACK_FLOODING = 2;
const int RST_FLOODING = 3;
const int ICMP_FLOODING = 4;
const int SMURF_FLOODING = 5;
const int UDP_FLOODING = 6;
const int FIN_FLOODING = 7;
const int MAX_ATTACK_TYPE = 7;
/// @}

#define FLOODING_DELAY		0.001

using std::vector;

/**
 * @brief Simulates the real DDoS generation tool Tribe Flood Network (TFN)
 *
 * This class allows for simulation of realistic attack traffic according
 * to the real tool Tribe Flood Network (TFN).
 * Different types of attack can be simulated by this class.
 * Additionally, multiple attack waves and changing zombies can be simulated.
 *
 * @class TribeFloodNetwork
 */
class REASE_API TribeFloodNetwork: public cSimpleModule
{
	typedef void (TribeFloodNetwork::*FloodingFunc)();
	typedef vector<cMessage *> START_TIME_VEC;
	typedef std::vector<std::string> StringVector;

protected:
	/// Packet sizes of the different attacks
	vector<int> packetSize;
	/// Attack types of the different attacks
	vector<int> attackType;
	/// Maximum number of packets of the different attacks
	vector<int> maxPacket;
	/// IP addresses of the victims of the different attacks
	vector<IPvXAddress> victimAddr;
	/// IP address range used in case of spoofing
	/// @{
	IPvXAddress sourceAddrStart, sourceAddrEnd;
	/// @}
	/// Attacked port at the victim of the different attacks
	vector<int> victimPort;
	/// Source port of attack packets of the different attacks
	vector<int> sourcePort;
	/// Apply spoofing for attack packtes
	bool spoofingOn;
	/// If source port not specified it is increased every packet
	bool srcPortWalkOn;
	/// If victim Port not specified it is increased every packet
	bool dstPortWalkOn;
	/// Message for internally starting
	cMessage *floodingTime;
	/// Messages scheduled at start times of the different attacks
	START_TIME_VEC startMsg;

	/// Probability that a zombies takes part in the current attack
	double probAttackOn;
	/// Probability that a zombie switches to the new attack
	double probAttackSwitch;
	/// Probability that a zombie stops taking part in an attack
	double probAttackOff;

	/// Time to wait before sending next attack packet (min time + diff time)
	double floodingDelay;
	/// Number of attack packets sent
	int floodingTimes;

	/// Function to send an attack packet
	FloodingFunc floodFunc;
	/// Attack index
	int index;

	/// state variables for currently used values
	/// @{
	int c_victimPort;
	int c_sourcePort;
	int c_packetSize;
	int c_attackType;
	int c_maxPacket;
	double c_floodingDelay;
	IPvXAddress c_victimAddr;
	/// @}

	/// Indicates if an attack currently is active
	bool isActive;

	/// state variables for output into scalar file
	/// @{
	int attackIndex;
	long packetsSent;
	double attackStart;
	double attackEnd;
	IPvXAddress sourceIP;
	IPvXAddress destIP;
	/// @}
public:
	TribeFloodNetwork();
	virtual ~TribeFloodNetwork();
protected:
	/// @brief Start initialization in stage 5
	virtual int numInitStages() const { return 5; }
	/// @brief Read all necessary parameters from configuration
	virtual void initialize(int stages);
	/// @brief Handles self messages to start attack or send attack packet
	virtual void handleMessage(cMessage *msg);
	/// @brief Creates TCP attack packet and give it to IP layer
	virtual void handleTCPFlood();
	/// @brief Creates UDP attack packet and give it to IP layer
	virtual void handleUDPFlood();
	/// @brief Creates ICMP attack packet and give it to IP layer
	virtual void handlePingFlood();
	/// @brief Set floodFunc pointer to appropriate packet generation method
	virtual void setFloodingFunc(int attackType);
	/// @brief Set parameters of current attack to the attack with the given index
	virtual void setCurrentFloodingParameter(unsigned int index);
	/// @brief Write attack path from zombie to victim to standard out
	void printAttackPath();

	/// @brief Records scalar values for attack statistics
	void recordAttackScalars();
	void finish();
};

#endif /*TRIBEFLOODNETWORK_H_*/
