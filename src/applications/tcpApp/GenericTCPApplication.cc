#include "GenericTCPApplication.h"
#include "GenericTCPApplicationClientThread.h"
#include "GenericTCPApplicationServerThread.h"

/**
 * @brief Implementation of a TCP application.
 *
 * TCP servers open a welcome socket and react on incoming connections.
 * New connections of TCP clients can be triggered by the connection
 * manager.
 * Overload situations at servers are simulated by limiting the number
 * of TCP connections that may open at the same time.
 *
 * @class GenericTCPApplication
 */

Define_Module( GenericTCPApplication);

GenericTCPApplication::GenericTCPApplication()
{
	// we have to set an appropriate application type
	// to register at the InetUser
	applicationType = TCP_APP;
	serverSocket = NULL;
	threadCount = 0;
	maxThreadCount = 0;
	selfMsg = NULL;
}

GenericTCPApplication::~GenericTCPApplication()
{
	if (selfMsg)
	{
		if (selfMsg->isScheduled())
			cancelEvent(selfMsg);
		delete selfMsg;
	}

	ThreadList::iterator it = threadList.begin();
	while (it != threadList.end())
	{
		delete (*it)->getSocket();
		delete (*it);
		it++;
	}

	selfMsg = NULL;
	if (serverSocket)
		delete serverSocket;
}

/**
 * Closes the given thread and its associated socket.
 * This method is called whenever a TCP communication has finished.
 *
 * @param thread Thread that should be closed
 */
void GenericTCPApplication::removeThread(GenericTCPApplicationThreadBase *thread)
{
	ThreadList::iterator it = threadList.begin();
	while (it != threadList.end())
		if (thread == *it)
			break;
		else
			it++;
	if (it != threadList.end())
		threadList.erase(it);
	else
		opp_error("cannot find TCP thread in threadList\n");

	delete socketMap.removeSocket(thread->getSocket());
	delete thread;
	threadCount--;

	updateDisplay();
}

/**
 * Closes the given thread and its associated socket and sends statistics to
 * InetUser.
 *
 * @param thread Thread that should be closed
 * @param stat Statistics collected about the previous transmission
 */
void GenericTCPApplication::removeThread(GenericTCPApplicationThreadBase *thread, TransmissionStatistics stat)
{
	removeThread(thread);
	if (!isServer)
	{
		// send transmission statistics to InetUser
		transmissionDone(stat);
	}
}

/**
 * In case a new transmission is started, a new socket is opened.
 * In addition, a clientThread is started for the actual communication.
 */
void GenericTCPApplication::transmissionStart(TrafficProfile &p, TargetInfo &i)
{
	Enter_Method_Silent();
	//
	// this methode is only called for a client
	//
	ASSERT(!isServer);
	//
	// randomize the profile values
	//
	curProfile = p;
	//
	// set socket and gate
	//
	TCPSocket *socket = new TCPSocket();
	socket->bind(IPvXAddress(), curProfile.ownPort);
	socket->setOutputGate(gate("tcpOut"));
	//
	// create an active ClientThread
	//
	GenericTCPApplicationThreadBase *t = new GenericTCPApplicationClientThread(curProfile, i);
	socket->setCallbackObject(t);
	t->init(this, socket);
	socketMap.addSocket(socket);
	threadCount++;
	threadList.push_back(t);

	updateDisplay();
}

/**
 * Call initialize method of super class that registers at
 * InetUser and ConnectionManager. Then open a welcome socket
 * in case of a server instance.
 */
void GenericTCPApplication::initialize(int stages)
{
	if (stages != INITIALIZATION_STAGE_NECESSARY)
		return;

	// Base-Constructer registers to InetUser and ConnectionManager
	GenericApplication::initialize(stages);
	if (isServer)
	{
		// open the ServerSocket and start listening
		serverSocket = new TCPSocket();
		serverSocket->setOutputGate(gate("tcpOut"));
		serverSocket->bind(IPvXAddress(), port);
		serverSocket->listen();
		maxThreadCount = par("noThreads");
		cout << "Server Listening to new connections: " << this->getFullPath() << " threads: " << maxThreadCount << endl;
		// a server may have a limited number of concurrently running threads

	}

	if (maxThreadCount == 0)
		maxThreadCount = INT_MAX;
}

/**
 * In case of self messages the callback method of the corresponding thread is called.
 * Else the message must be processed by the corresponding socket or a new connection
 * must be opened (only if instance is a server).
 *
 * @param msg Received message
 */
void GenericTCPApplication::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
	{
		GenericTCPApplicationThreadBase *thread = (GenericTCPApplicationThreadBase *) msg->getContextPointer();
		thread->timerExpired(msg);
	}
	else
	{
		// a new connection attempt so create new thread for this communication
		TCPSocket *socket = socketMap.findSocketFor(msg);
		if (!socket)
		{
			if (!isServer)
				opp_error("received client message to unknown socket\n");
			if (threadCount >= maxThreadCount)
			{
				// if thread limit is hit - kill all incoming connection attempts
				// TODO: to do it correct the socket should be closed to indicate
				// the overload situation of the server. This, however, does not
				// work correctly with the INET framework. Thus, connection is
				// excepted and killed immediately.
//				socket = new TCPSocket(msg);
//				socket->setOutputGate(gate("tcpOut"));
//
//				if (socket->getState() != TCPSocket::CLOSED)
//					socket->close();
//				delete socket;
				delete msg;
				return;
			}

			// valid connection attempts get their own server thread
			socket = new TCPSocket(msg);
			socket->setOutputGate(gate("tcpOut"));
			GenericTCPApplicationThreadBase *thread = new GenericTCPApplicationServerThread();
			socket->setCallbackObject(thread);
			thread->init(this, socket);
			socketMap.addSocket(socket);
			threadCount++;
			threadList.push_back(thread);

			updateDisplay();
		}

		// process messages to already opened sockets
		socket->processMessage(msg);
	}
}

void GenericTCPApplication::finish()
{
	//TODO:
}

void GenericTCPApplication::updateDisplay()
{
	if (!ev.isGUI())
		return;

	char buf[40];
	sprintf(buf, "%d threads, is%s (%s)", threadCount, (isServer ? "Server" : "Client"), ((threadCount < maxThreadCount) ? "on" : "off"));
	getDisplayString().setTagArg("t", 0, buf);
}

