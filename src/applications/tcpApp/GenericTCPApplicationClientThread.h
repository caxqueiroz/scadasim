#ifndef GENERICTCPAPPLICATIONCLIENTTHREAD_H_
#define GENERICTCPAPPLICATIONCLIENTTHREAD_H_

#include <omnetpp.h>
#include "GenericTCPApplication.h"
#include "SCADASIMDefs.h"

/**
 * @brief A client thread implements the endpoint of a client/server connection.
 *
 * The client opens a socket and starts the communication by sending the first request.
 * Such a request contains all data necessary for the server to generate a reply.
 * After a maximum number of requests has been sent, the socket is closed and the 
 * thread removed.
 *
 * @class GenericTCPApplicationClientThread
 */

/// Thread States
/// @{
#define NOT_SET 0
#define WAITING 1
#define CONNECTED 2
#define DISCONNECTED 3
#define FINISH 4
/// @}


class SCADASIM_API GenericTCPApplicationClientThread : public GenericTCPApplicationThreadBase
{
protected:
	/// Traffic profile used by this thread
	TrafficProfile curProfile;
	/// Communication partner of this thread
	TargetInfo curTarget;
	/// State of current thread
	int threadState;
	/// Connection statistics of this thread
	/// @{
	unsigned long noRequestsToSend, noBytesSend, noBytesReceived, noPacketSend, noPacketReceived;
	/// @}
	/// Self message for TCP state transitions
	cMessage *selfMsg;


public:
	GenericTCPApplicationClientThread(TrafficProfile &p, TargetInfo &i);
	virtual ~GenericTCPApplicationClientThread();
	virtual void init(GenericTCPApplication *owner, TCPSocket *sock);
	virtual void timerExpired(cMessage *msg);

protected:
	// Interface Methods
	virtual void socketDataArrived(int connId, void *youtPtr, cPacket *msg, bool urgent);
	virtual void socketEstablished(int connId, void *youtPtr);
	virtual void socketPeerClosed(int connId, void *youtPtr);
	virtual void socketClosed(int connId, void *youtPtr);
	virtual void socketFailure(int connId, void *youtPtr, int code);
	virtual void socketStatusArrived(int connId, void *youtPtr, TCPStatusInfo *status);
	
	/// @brief Create a request message and send it to the server
	virtual void sendRequest();
};

#endif /*GENERICTCPAPPLICATIONCLIENTTHREAD_H_*/
