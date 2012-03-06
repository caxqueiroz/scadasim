#ifndef GENERICUDPAPPLICATIONSERVERTHREAD_H_
#define GENERICUDPAPPLICATIONSERVERTHREAD_H_

#include <omnetpp.h>
#include "GenericUDPApplication.h"
#include "SCADASIMDefs.h"

/**
 * @brief A server thread implements the endpoint of a client/server connection.
 *
 * The server sends replies to the request that is received on the sockets.
 * A reply to a client request can consists of multiple reply packets. These are sent
 * with a specified inter-packet delay. 
 * Such replies, however, must be fragmented before sending.
 * The socket is closed as soon as all replies to this single request have been sent.
 *
 * @class GenericUDPApplicationServerThread
 */
class SCADASIM_API GenericUDPApplicationServerThread : public GenericUDPApplicationThreadBase
{
private:
	cMessage *selfMsg;
	int noReplysToSend, threadState, packetSize, noPacketSend;
	int packetPerReply;
	double timeBetweenReplys;

protected:
	//
	// Interface Methods
	//
	virtual void socketDatagramArrived(int sockId, void *yourPtr, cMessage *msg, UDPControlInfo *ctrl);
	virtual void socketPeerClosed(int sockId, void *yourPtr) {};

	/// @brief Create a reply message and send it to the client
	void sendReply();

public:
	GenericUDPApplicationServerThread();
	virtual ~GenericUDPApplicationServerThread();
	virtual void init(GenericUDPApplication *owner, UDPSocket *sock);
	virtual void timerExpired(cMessage *msg);

};

#endif /*GENERICUDPAPPLICATIONSERVERTHREAD_H_*/
