#include "GenericTCPApplicationClientThread.h"
#include "GenericApplicationMessage_m.h"
#include "TCPSocket.h"
#include <iostream>

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

GenericTCPApplicationClientThread::GenericTCPApplicationClientThread(
        TrafficProfile &p, TargetInfo &i) {
    curProfile = p;
    curTarget = i;

    // maximum number of requests per session
    noRequestsToSend = curProfile.getRequestsPerSession(false);
    selfMsg = NULL;
}

GenericTCPApplicationClientThread::~GenericTCPApplicationClientThread() {
    if (selfMsg) {
        if (selfMsg->isScheduled())
            cancelEvent(selfMsg);
        delete selfMsg;
    }
}

/**
 * Sets timer for starting the new communication.
 */
void GenericTCPApplicationClientThread::init(GenericTCPApplication *owner,
        TCPSocket *sock) {
    GenericTCPApplicationThreadBase::init(owner, sock);
    noBytesSend = noBytesReceived = noPacketSend = noPacketReceived = 0;
    selfMsg = new cMessage("requestTimer");

    // set timer to start transmission
    threadState = WAITING;
    scheduleAt(simTime() + SELF_CALL_DELAY, selfMsg);
}

/**
 * Manage TCP state transitions
 */
void GenericTCPApplicationClientThread::timerExpired(cMessage *msg) {
    switch (threadState) {
    case WAITING:
        // not yet established a connection
        // --> so start it now!
        socket->connect(curTarget.address, curTarget.port);
        break;
    case CONNECTED:
        // time to start a new request
        sendRequest();
        break;
    case DISCONNECTED:
        // the server has sent the last data and closed his socket.
        // Now after I (client) sent all data, close my socket
        // TODO: wenn viele Pakete verloren gehen kann es ewig dauern,
        // bis der InetUser benachrichtigt wird. Hier sollten eventuell
        // Zombie-Threads laufen, die nur auf das Ende einer Verbindung warten
        //
        if (socket->getState() != TCPSocket::LOCALLY_CLOSED)
            socket->close();
        break;
    case FINISH:
        // both sides of connection are closed
        // respond to the GenericTCPApplication
        TransmissionStatistics s(noBytesSend, noBytesReceived, noPacketSend,
                noPacketReceived);
        ownerModule->removeThread(this, s);
        break;
    }
}

/**
 * Process data received from server.
 * In case no more requests must be sent, wait timeBetweenRequests and close the socket.
 */
void GenericTCPApplicationClientThread::socketDataArrived(int connId,
        void* yourPtr, cPacket *msg, bool urgent) {
    noBytesReceived += msg->getByteLength();
    noPacketReceived++;

    delete msg;

    if (noRequestsToSend <= 0)
        scheduleAt(simTime() + SELF_CALL_DELAY, selfMsg);
    else
        scheduleAt(simTime() + curProfile.getTimeBetweenRequests(true),
                selfMsg);
}

/**
 * Start sending requests after the socket is opened.
 */
void GenericTCPApplicationClientThread::socketEstablished(int connId,
        void *yourPtr) {
    // guarantee that at least one request will be send
    if (noRequestsToSend < 1)
        noRequestsToSend = 1;

    threadState = CONNECTED;

    sendRequest();
}

/**
 *
 */
void GenericTCPApplicationClientThread::socketPeerClosed(int connId,
        void *yourPtr) {
    //
    // if the server closed his connection, i can still send him data
    //

//    if (socket->getState() == TCPSocket::PEER_CLOSED) {
//        socket->close();
//        threadState = FINISH;
//        EV << "remote TCP closed, closing here as well\n";
//    }
}

/**
 * At the end of a transmission, wait timeBetweenSessions and then close this thread.
 * In case of abnormal socket closing, close this thread immediately.
 */
void GenericTCPApplicationClientThread::socketClosed(int connId,
        void *yourPtr) {
    if (selfMsg->isScheduled())
        cancelEvent(selfMsg);

    if (threadState == DISCONNECTED)
        scheduleAt(simTime() + curProfile.getTimeBetweenSessions(true),
                selfMsg);
    else
        // e.g. userTimer expired
        scheduleAt(simTime() + SELF_CALL_DELAY, selfMsg);

    threadState = FINISH;
}

/**
 * Close thread and thus, stop transmission in case of socket failure
 */
void GenericTCPApplicationClientThread::socketFailure(int connId, void *youtPtr,
        int code) {
    switch (code) {
    case TCP_I_CONNECTION_REFUSED:
        EV << "TCP Client Error - Connection refused\n";
        break;
    case TCP_I_CONNECTION_RESET:
        EV << "TCP Client Error - Connection reset\n";
        break;
    case TCP_I_TIMED_OUT:
        EV << "TCP Client Error - Timed out\n";
        break;
    }

    TransmissionStatistics s;
    ownerModule->removeThread(this, s);
}

void GenericTCPApplicationClientThread::socketStatusArrived(int connId,
        void *yourPtr, TCPStatusInfo *status) {
    //
    // TODO: do nothing
    //
    delete status;
}

/**
 * Creates a new request message and sets all necessary data.
 * Then, the message is sent via the socket associated to this thread.
 */
void GenericTCPApplicationClientThread::sendRequest() {
    noRequestsToSend--;
    if (noRequestsToSend <= 0)
        threadState = DISCONNECTED;

    noPacketSend++;

    int packetSize = curProfile.getRequestLength(true);
    noBytesSend += packetSize;

//	stringstream ss;
//	srand((unsigned) time(0));
//	int random_integer = rand();
//	//ss << SIMTIME_STR(simTime());
//	unsigned long id = ev.getUniqueNumber() + random_integer;
//	ss << id;

    GenericApplicationMessage *appmsg = new GenericApplicationMessage(
            "data-app");

    appmsg->setByteLength(packetSize);
    appmsg->setReplyLength(curProfile.getReplyLength(true));
    double timeToRespond = curProfile.getTimeToRespond(false);

    appmsg->setTimeToRespond(timeToRespond);
    appmsg->setReplyPerRequest(1);
    appmsg->setPacketNumber(noPacketSend);
    appmsg->setLast(threadState == DISCONNECTED);

    socket->send(appmsg);
}

