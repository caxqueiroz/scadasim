#include "GenericTCPApplicationServerThread.h"
#include "GenericApplicationMessage_m.h"
#include "TCPSocket.h"

GenericTCPApplicationServerThread::GenericTCPApplicationServerThread()
{
	selfMsg = new cMessage("replyTimer");
	doClose = false;
}

GenericTCPApplicationServerThread::~GenericTCPApplicationServerThread()
{
	if (selfMsg)
	{
		if (selfMsg->isScheduled())
			cancelEvent(selfMsg);
		delete selfMsg;
	}
	while (!replies.empty())
	{
		delete replies.front();
		replies.pop();
	}
}

/**
 * Send replies to the requesting client.
 * Furthermore, the socket is closed if the last request was received.
 */
void GenericTCPApplicationServerThread::timerExpired(cMessage *msg)
{
	// time to respond to request
	if (!replies.empty())
	{
		socket->send(replies.front());
		replies.pop();
	}

	// if this was the last packet, then close the socket (on my side)
	//
	if (doClose && (socket->getState() != TCPSocket::LOCALLY_CLOSED))
		socket->close();
}

/**
 * Process received client requests.
 * In case this was the last request (information contained in the message)
 * set doClose to true and close socket.
 */
void GenericTCPApplicationServerThread::socketDataArrived(int connId, void* yourPtr, cPacket *msg, bool urgent)
{
	GenericApplicationMessage *appmsg = dynamic_cast<GenericApplicationMessage *> (msg);
	if (!appmsg)
		opp_error("Message (%s) %s is not a GenericApplicationMessage", msg->getClassName(), msg->getName());

	int replyLength = appmsg->getReplyLength();

	doClose = appmsg->getLast();
	if (replyLength == 0)
	{
		delete appmsg->removeControlInfo();
		delete appmsg;
	}
	else
	{
		appmsg->setByteLength(replyLength);
		delete appmsg->removeControlInfo();

		// delay the reply if required by the profile
		if (appmsg->getTimeToRespond() > 0)
		{
			replies.push(appmsg);
			int repliesSize = replies.size();
			double timeToRespond = appmsg->getTimeToRespond();
			scheduleAt(simTime() + appmsg->getTimeToRespond(), selfMsg);
		}
		else
		{
			socket->send(appmsg);

			// if this was the last packet and no delay is needed,
			// close the socket
			if (doClose && (socket->getState() != TCPSocket::LOCALLY_CLOSED))
				socket->close();
		}
	}
}

/**
 * Nothing to be done since communication is started by client
 */
void GenericTCPApplicationServerThread::socketEstablished(int connId, void *yourPtr)
{
	//TODO: or not interested

}

/**
 * Close server socket if client socket was closed.
 */
void GenericTCPApplicationServerThread::socketPeerClosed(int connId, void *yourPtr)
{
	//
	// just set a flag that i know - the client sends me no more request
	// and so i can close my socket after sending my replies
	//
	doClose = true;
}

/**
 * At the end of a transmission, close thread.
 */
void GenericTCPApplicationServerThread::socketClosed(int connId, void *yourPtr)
{
	// both sides of connection are closed i can delete the server thread
	ownerModule->removeThread(this);
}

/**
 * Close thread in case of a socket failure.
 */
void GenericTCPApplicationServerThread::socketFailure(int connId, void *youtPtr, int code)
{
	ownerModule->removeThread(this);
}

void GenericTCPApplicationServerThread::socketStatusArrived(int connId, void *yourPtr, TCPStatusInfo *status)
{
	//TODO: or not interested
	delete status;
}
