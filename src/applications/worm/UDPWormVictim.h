#ifndef UDPWORMVICTIM_H_
#define UDPWORMVICTIM_H_

#include <omnetpp.h>
#include "IPvXAddress.h"
#include "UDPSocket.h"
#include "SCADASIMDefs.h"

const int FIRST_ATTACK_ID_OF_WORM_SPREAD = 10000;

/**
 * @brief This class implements a generator for a worm propagation.
 *
 * The infected hosts start sending probing packets to random addresses
 * of the given address range. If another UDPWormVictim, that is not yet
 * infected, receives such a probing packet it switches its state to 
 * infected and starts itself sending probing packets. Having sent a
 * maximum number of probing packets, the host switch to inactive.
 *
 * @class UDPWormVictim
 */
class SCADASIM_API UDPWormVictim : public cSimpleModule
{
protected:
	/// Fixed inter-packet delay for subsequent probing packets
	double probingDelay;
	/// Maximum number of probing packets sent by an infected host
	int maxProbingPackets;
	/// IP address range used for probing packets
	/// @{
	IPvXAddress addrStart, addrEnd;
	/// @}
	/// Source port of probing packets
	int sourcePort;
	/// Destination port of probing packets
	int victimPort;
	/// Timer message indicating that a probing packet should be sent
	cMessage *probingTimer;
	/// Start time of worm propagation
	double startTime;
	/// Indicates if the host is already infected or just vulnerable
	bool isActive;
	/// Tag of this attack that identifies probing packets
	int attackTag;
	/// Length of probing packets
	int packetLength;

public:
	UDPWormVictim();
	virtual ~UDPWormVictim();

protected:
	virtual int numInitStages() const{return 4; };
	/// @brief Decide if initially infected or just vulnerable
	virtual void initialize(int stages);
	/// @brief Send new own probing packet or received a probing packet
	virtual void handleMessage(cMessage *msg);
	/// @brief Actually send a probing packet
	void sendProbingPacket();
};

#endif /*UDPWORMVICTIM_H_*/
