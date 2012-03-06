#ifndef GENERICUDPAPPLICATIONCLIENTTHREAD_H_
#define GENERICUDPAPPLICATIONCLIENTTHREAD_H_

#include <omnetpp.h>
#include "GenericUDPApplication.h"

/**
 * @brief A client thread implements the endpoint of a client/server connection.
 *
 * The client opens a socket and starts the communication by sending the first request.
 * Such a request, however, must be fragmented before sending. The last fragment contains 
 * all data necessary for the server to generate a reply.
 * The UDPApplication gives only the last fragment of the replies to the client for
 * processing.
 * After a maximum number of requests has been sent, the socket is closed and the 
 * thread removed.
 *
 * @class GenericUDPApplicationClientThread
 */
class SCADASIM_API  GenericUDPApplicationClientThread : public GenericUDPApplicationThreadBase
{
private:
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
	/// state variables required due to fragmentation
	/// @{
	int packetsPerRequest;
	int packetSize;
	int curPacketsPerRequest;
	/// @}
	/// Necessary for static inter-packet wait time
	double curResponseTime;

public:
	GenericUDPApplicationClientThread(TrafficProfile &p, TargetInfo &i);
	virtual ~GenericUDPApplicationClientThread();
	virtual void init(GenericUDPApplication *owner, UDPSocket *sock);
	virtual void timerExpired(cMessage *msg);

protected:
	//
	// Interface Methods
	//
	virtual void socketDatagramArrived(int sockId, void *yourPtr, cMessage *msg, UDPControlInfo *ctrl);
	virtual void socketPeerClosed(int sockId, void *yourPtr) {};
	
	/// @brief Create a request message and send it to the server
	void sendRequest();
};



#endif /*GENERICUDPAPPLICATIONCLIENTTHREAD_H_*/
