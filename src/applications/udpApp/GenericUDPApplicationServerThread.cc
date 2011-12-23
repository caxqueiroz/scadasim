#include "GenericUDPApplicationServerThread.h"
#include "GenericApplicationMessage_m.h"
#include "UDPControlInfo_m.h"

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

/// Thread States
/// @{
#define NOT_SET -1
#define CONNECTED 1
#define DISCONNECTED 2
#define LAST 3
/// @}


GenericUDPApplicationServerThread::GenericUDPApplicationServerThread()
{
	selfMsg = NULL;
	threadState = NOT_SET;
}

GenericUDPApplicationServerThread::~GenericUDPApplicationServerThread()
{
	if (selfMsg)
	{
		if (selfMsg->isScheduled())
			cancelEvent(selfMsg);
		delete selfMsg;
	}
	selfMsg = NULL;
}

/**
 * Sets timer for sending replies to client.
 */
void GenericUDPApplicationServerThread::init(GenericUDPApplication *owner, UDPSocket *sock)
{
	GenericUDPApplicationThreadBase::init(owner, sock);
	noReplysToSend = 0;
	noPacketSend = 0;
	timeBetweenReplys = -1.0;
	selfMsg = new cMessage("replyTimer");
	threadState = CONNECTED;
}

/**
 * Manage UDP state transitions.
 * In case no more replies must be sent (DISCONNECTED), close socket
 * and remove thread.
 */
void GenericUDPApplicationServerThread::timerExpired(cMessage *msg)
{
	switch (threadState)
	{
		case CONNECTED:
		case LAST:
			// time to send the next reply
			sendReply();
			break;
		case DISCONNECTED:
			socket->close();
			// last reply was sent -> end
			ownerModule->removeThread(this);
			break;
	}
}

/**
 * Creates a new reply message and all its fragemts. All necessary data is set.
 * Then, the messages are sent via the socket associated to this thread.
 * In case of the last reply the last flag is set, else the timer is restarted.
 */
void GenericUDPApplicationServerThread::sendReply()
{
	noReplysToSend--;
	noPacketSend++;

	for (int i = 0; i < packetPerReply; i++)
	{
		// respond without delay
		GenericApplicationMessage *appmsg = new GenericApplicationMessage("udp_reply_data");
		appmsg->setByteLength(packetSize);
		appmsg->setPacketNumber(noPacketSend);
		socket->send(appmsg);
	}

	// request was the last and all replys were send
	if (noReplysToSend <= 0)
	{
		threadState = DISCONNECTED;

		// self-call delay until termination
		scheduleAt(simTime() + SELF_CALL_DELAY, selfMsg);
	}
	else
		// set time for next reply
		scheduleAt(simTime() + timeBetweenReplys, selfMsg);
}

/**
 * Process data received from client (only the last message is given to the thread by the UDPApplication).
 * This method is invoked just once a time. After having sent the reply packets the socket is closed
 * and the server removed.
 * Wait timeBetweenReplies until the fragments of the first reply are sent to the client.
 */
void GenericUDPApplicationServerThread::socketDatagramArrived(int sockId, void *yourPtr, cMessage *msg, UDPControlInfo *ctrl)
{
	packetPerReply = 1;
	GenericApplicationMessage *appmsg = dynamic_cast<GenericApplicationMessage *> (msg);
	packetSize = appmsg->getReplyLength();
	noReplysToSend = appmsg->getReplyPerRequest();
	noPacketSend = appmsg->getPacketNumber();
	timeBetweenReplys = appmsg->getTimeToRespond();

	// Fragmentation must be done before sending UDP packets
	if (packetSize > UDP_MAX_PAYLOAD)
	{
		packetPerReply = (int) ceil(packetSize / UDP_MAX_PAYLOAD);
		packetSize = (int) UDP_MAX_PAYLOAD;
	}

	delete msg;
	delete ctrl;

	scheduleAt(simTime() + timeBetweenReplys, selfMsg);
}
