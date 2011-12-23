#ifndef GENERICICMPPINGAPPLICATION_H_
#define GENERICICMPPINGAPPLICATION_H_

#include <omnetpp.h>
#include "GenericApplication.h"
#include "TransmissionConfig.h"
#include "ReaSEDefs.h"

/**
 * @brief Implementation of an ICMP Ping application.
 *
 * This implementation only sends icmp ping requests.
 * It has no influence to the processing of the corresponding
 * ICMP Pong.
 *
 * @class GenericICMPPingApplication
 */
class REASE_API GenericICMPPingApplication: public GenericApplication
{
private:
	/// IP address of the ICMP client
	IPvXAddress myAddr;
	/// Traffic profile used by this thread
	TrafficProfile curProfile;
	/// Communication partner of this thread
	TargetInfo curTarget;
	/// Connection statistics of this thread
	/// @{
	int noRequestsToSend, noBytesSend, noBytesReceived, noPacketSend, noPacketReceived;
	/// @}
	/// Self message for UDP state transitions
	cMessage *selfMsg;
	/// Number of ping messages to send
	int pingToSend;
protected:
	/// @brief Register at InetUser and ConnectionManager and open welcome socket (server only!)
	virtual void initialize(int stage);
	/// @brief Process packets of communication partners and self messages of threads
	virtual void handleMessage(cMessage *msg);
	virtual void finish();
	void updateDisplay();

	/// @brief Create an ICMP Ping message and send it to the server
	void sendPing();
	/// @brief Process the ICMP Pong message from the server
	void receivePong(cMessage *msg);

public:
	GenericICMPPingApplication();
	virtual ~GenericICMPPingApplication();
	/// @brief Starts a new connection to a server (client only!)
	virtual void transmissionStart(TrafficProfile &p, TargetInfo &i);

};

#endif /*GENERICICMPPINGAPPLICATION_H_*/
