#include "GenericUDPApplicationClientThread.h"
#include "IPvXAddress.h"
#include "GenericApplicationMessage_m.h"
#include <iostream>

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

using std::cerr;
using std::endl;

/// Thread States
/// @{
#define NOT_SET -1
#define CONNECTED 2
#define DISCONNECTED 3
#define FINISH 4
/// @}


GenericUDPApplicationClientThread::GenericUDPApplicationClientThread(TrafficProfile &p, TargetInfo &i)
{
	curProfile = p;
	curTarget = i;

	// maximum nuber of requests per session
	// TODO: Streaming Client should just make one request
	noRequestsToSend = curProfile.getRequestsPerSession(true);

	curResponseTime = curProfile.getTimeToRespond(true);

	packetsPerRequest = 1;
	packetSize = curProfile.getRequestLength(true);
	// Fragmentation must be done before sending UDP packets
	if (packetSize > UDP_MAX_PAYLOAD)
	{
		packetsPerRequest = (int) ceil(packetSize / UDP_MAX_PAYLOAD);
		packetSize = (int) UDP_MAX_PAYLOAD;
		curPacketsPerRequest = packetsPerRequest;
	}

	threadState = NOT_SET;
}

GenericUDPApplicationClientThread::~GenericUDPApplicationClientThread()
{
	if (selfMsg)
	{
		if (selfMsg->isScheduled())
			cancelAndDelete(selfMsg);
		else
			delete selfMsg;
	}
}

/**
 * Sets timer for starting the new communication and open socket to target.
 */
void GenericUDPApplicationClientThread::init(GenericUDPApplication *owner, UDPSocket *sock)
{
	GenericUDPApplicationThreadBase::init(owner, sock);
	noBytesSend = noBytesReceived = noPacketSend = noPacketReceived = 0;

	// set socket to target
	socket->connect(curTarget.address, curTarget.port);
	threadState = CONNECTED;

	selfMsg = new cMessage("requestTimer");
	scheduleAt(simTime() + SELF_CALL_DELAY, selfMsg);
}

/**
 * Manage UDP state transitions.
 * In case no more requests must be sent (DISCONNECTED), wait timeBetweenRequests
 * and then switch to FINISH.
 */
void GenericUDPApplicationClientThread::timerExpired(cMessage *msg)
{
	switch (threadState)
	{
		case CONNECTED:
			// time to start a new request
			sendRequest();
			break;
		case DISCONNECTED:
		{
			threadState = FINISH;
			scheduleAt(simTime() + curProfile.getTimeBetweenSessions(true), selfMsg);
			break;
		}
		case FINISH:
			socket->close();
			TransmissionStatistics s(noBytesSend, noBytesReceived, noPacketSend, noPacketReceived);
			ownerModule->removeThread(this, s);
			break;
	}
}

/**
 * Creates a new request message and all its fragemts. All necessary data is set.
 * Then, the messages are sent via the socket associated to this thread.
 */
void GenericUDPApplicationClientThread::sendRequest()
{
	noRequestsToSend--;
	if (noRequestsToSend <= 0)
		threadState = DISCONNECTED;

	int currentReplyLength = curProfile.getReplyLength(true);
	int curReplysToReceive = curProfile.getReplyPerRequest(true);

	noBytesReceived += currentReplyLength;
	noPacketReceived += ((int) ceil(packetSize / UDP_MAX_PAYLOAD));

	for (int i = packetsPerRequest; i > 0; i--)
	{
		noBytesSend += packetSize;
		noPacketSend++;

		GenericApplicationMessage *appmsg = new GenericApplicationMessage("udp_request_data");
		appmsg->setByteLength(packetSize);

		appmsg->setReplyLength(currentReplyLength);
		appmsg->setTimeToRespond(curResponseTime);
		appmsg->setReplyPerRequest(curReplysToReceive);
		appmsg->setLast(false);

		// the last fragment of request with packet number 1 will cause
		// a new server thread at the receiver side
		appmsg->setPacketNumber(i);

		socket->send(appmsg);
	}

	// static timer for next request

	// if the reply size is to big, the UDP packet will be queued at
	// source and will arrive delayed --> this could cause many ICMP-Error Msg
	scheduleAt(simTime() + curReplysToReceive * curResponseTime * 1.5, selfMsg);
}

/**
 * Process data received from server (only the last message is given to the thread by the UDPApplication).
 * In case no more requests must be sent, wait timeBetweenSessions and close the socket.
 */
void GenericUDPApplicationClientThread::socketDatagramArrived(int sockId, void *yourPtr, cMessage *msg, UDPControlInfo *ctrl)
{
	// this method is only called for the last message of a server
	if (!noRequestsToSend)
	{
		threadState = FINISH;
		if (selfMsg->isScheduled())
			cancelEvent(selfMsg);
		scheduleAt(simTime() + curProfile.getTimeBetweenSessions(true), selfMsg);
	}
	delete msg;
	delete ctrl;
}
