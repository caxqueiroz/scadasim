#ifndef GENERICTCPAPPLICATIONSERVERTHREAD_H_
#define GENERICTCPAPPLICATIONSERVERTHREAD_H_

#include <omnetpp.h>
#include <queue>
#include "GenericTCPApplication.h"
#include "SCADASIMDefs.h"

/**
 * @brief A server thread implements the endpoint of a client/server connection.
 *
 * The server sends replies to all requests that are received on open sockets.
 * A reply to a client request can consists of multiple reply packets. These are sent
 * with a specified inter-packet delay. If the last request of a communication is
 * received, the socket is closed and the thread removed.
 *
 * @class GenericTCPApplicationServerThread
 */
class SCADASIM_API GenericTCPApplicationServerThread : public GenericTCPApplicationThreadBase
{
protected:
	/// Indicates that the socket should be closed due to end of the transmission
	bool doClose;
	/// Triggers sending of replies after waiting time specified by the profile
	cMessage *selfMsg;
	/// Set of replies that are sent as reaction to a request
	std::queue<cMessage *> replies;
public:
	GenericTCPApplicationServerThread();
	virtual ~GenericTCPApplicationServerThread();
	virtual void timerExpired(cMessage *msg);

protected:
	// Interface Methods
	virtual void socketDataArrived(int connId, void *youtPtr, cPacket *msg, bool urgent);
	virtual void socketEstablished(int connId, void *youtPtr);
	virtual void socketPeerClosed(int connId, void *youtPtr);
	virtual void socketClosed(int connId, void *youtPtr);
	virtual void socketFailure(int connId, void *youtPtr, int code);
	virtual void socketStatusArrived(int connId, void *youtPtr, TCPStatusInfo *status);	
};

#endif /*GENERICTCPAPPLICATIONSERVERTHREAD_H_*/
