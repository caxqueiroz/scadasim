#include "GenericUDPApplication.h"
#include "GenericUDPApplicationClientThread.h"
#include "GenericUDPApplicationServerThread.h"
#include "GenericApplicationMessage_m.h"
#include <iostream>

/**
 * @brief Implementation of a UDP application.
 *
 * UDP applications open a welcome socket and react on incoming connections.
 * (All clients are at least servers for UDPMisc traffic).
 * New connections of UDP clients can be triggered by the connection
 * manager.
 * Overload situations at servers are simulated by limiting the number
 * of UDP connections that may open at the same time. In case of overload
 * the welcome socket is closed and ICMP error message are generated. *
 *
 * @class GenericUDPApplication
 */

using std::map;
using std::pair;

Define_Module( GenericUDPApplication);

const double GenericUDPApplicationThreadBase::UDP_MAX_PAYLOAD = 1022.;

GenericUDPApplication::GenericUDPApplication()
{
	//
	// we have to set an appropriate application type
	// to register at the inetUser
	//
	applicationType = UDP_APP;
	mySocket = NULL;
	threadCount = 0;
	maxThreadCount = 0;
	selfMsg = NULL;
}

GenericUDPApplication::~GenericUDPApplication()
{
	if (selfMsg)
	{
		if (selfMsg->isScheduled())
			cancelEvent(selfMsg);
		delete selfMsg;
	}

	ThreadMap::iterator it = threadMap.begin();
	while (it != threadMap.end())
	{
		if (it->second != NULL)
		{
			delete it->second->thread->getSocket();
			delete it->second->thread;
			delete it->second;
		}
		it++;
	}

	selfMsg = NULL;
	if (mySocket)
	{
		delete mySocket;
		mySocket = NULL;
	}
}

/**
 * Closes the given thread and its associated socket. If server
 * was overloaded, a new welcome socket is opened now.
 * This method is called whenever a UDP communication has finished.
 *
 * @param thread Thread that should be closed
 */
void GenericUDPApplication::removeThread(GenericUDPApplicationThreadBase *thread)
{
	UDPSocket *sock = thread->getSocket();

	ThreadMap::iterator it = threadMap.find(sock->getSocketId());
	if (it != threadMap.end())
	{
		delete it->second;
		threadMap.erase(it);
	}
	else
		opp_error("cannot find UDP thread in threadMap\n");

	delete sock;
	delete thread;
	threadCount--;

	if (threadCount == (maxThreadCount - 1))
	{
		//
		// welcome new connection attempts
		//
		mySocket->setOutputGate(gate("udpOut"));
		mySocket->bind(port);
	}
	updateDisplay();
}

/**
 * Closes the given thread and its associated socket and sends statistics to
 * InetUser.
 *
 * @param thread Thread that should be closed
 * @param stat Statistics collected about the previous transmission
 */
void GenericUDPApplication::removeThread(GenericUDPApplicationThreadBase *thread, TransmissionStatistics stat)
{
	removeThread(thread);
	if (!isServer)
	{
		//
		// send transmission statistics to InetUser
		//
		transmissionDone(stat);
	}
}

/**
 * In case a new transmission is started, a new socket is opened.
 * In addition, a clientThread is started for the actual communication.
 */
void GenericUDPApplication::transmissionStart(TrafficProfile &p, TargetInfo &i)
{
	Enter_Method_Silent();
	//
	// this methode is only called for clients
	//
	ASSERT(!isServer);

	// randomize the profile values
	curProfile = p;
	//
	// set socket and gate
	//
	UDPSocket *socket = new UDPSocket();
	socket->setOutputGate(gate("udpOut"));
	socket->bind(IPvXAddress(), curProfile.ownPort);

	//
	// create the active ClientThread
	//
	GenericUDPApplicationThreadBase *t = new GenericUDPApplicationClientThread(curProfile, i);
	socket->setCallbackObject(t);
	t->init(this, socket);
	threadCount++;
	// mem debug
	ThreadMap::iterator it = threadMap.find(socket->getSocketId());
	if (it != threadMap.end())
		std::cerr << "Speicherleck durch Socket" << std::endl;
	threadMap.insert(pair<int, threadInfo*> (socket->getSocketId(), new threadInfo(false, t)));

	updateDisplay();
}

/**
 * Call initialize method of super class that registers at
 * InetUser and ConnectionManager. Then open a welcome socket
 * in case of a server and client (each client acts as server
 * for UDPMisc traffic).
 */
void GenericUDPApplication::initialize(int stages)
{
	//
	// process only at corresponding initialize stage
	//
	if (stages != INITIALIZATION_STAGE_NECESSARY)
		return;

	// Base-Constructor registers to InetUser and ConnectionManager
	GenericApplication::initialize(stages);

	// UDP Application acts as server and, for host systems, as client too.
	// mySocket is just for incoming server connection attempts
	//
	mySocket = new UDPSocket();
	mySocket->setOutputGate(gate("udpOut"));
	mySocket->bind(port);

	// a server may have a limited number of concurrently running threads
	if (par("isServer").boolValue())
		maxThreadCount = par("noThreads");

	if (maxThreadCount == 0)
		maxThreadCount = INT_MAX;
}

/**
 * In case of self messages the callback method of the corresponding thread is called.
 * Else a udp error message or an application message is received. The UDP erro
 * handling is done in this method directly. In case the message is of another
 * UDP application it must be processed by the corresponding socket or a new
 * connection must be opened (only if instance is a server).
 *
 * In case of overload the welcome socket is closed. This results in the generation of
 * ICMP message for further connection attempts.
 *
 * @param msg Received message
 */
void GenericUDPApplication::handleMessage(cMessage *msg)
{
	//
	// a mixture of socket-api usage and direct udp-handling,
	// thx to beautiful UDPSockets
	//
	if (msg->isSelfMessage())
	{
		//
		// all timeouts from threads are selfMsg
		//
		GenericUDPApplicationThreadBase *thread = (GenericUDPApplicationThreadBase *) msg->getContextPointer();
		thread->timerExpired(msg);
	}
	else if (msg->getKind() == UDP_I_ERROR)
	//
	// direct udp handling for error-msg
	//
	{
		UDPControlInfo *ctrl = dynamic_cast<UDPControlInfo *> (msg->getControlInfo());
		//
		// if the error message deals with mySocket
		//
		if (ctrl->getSockId() == mySocket->getSocketId())
		{
			//FIXME: have i to close the socket and open a new one?


			//mySocket->close();
			//delete mySocket;
			//mySocket = new UDPSocket();
			//mySocket->setOutputGate(gate("udpOut"));
			//mySocket->bind(port);
		}
		else
		{
			ThreadMap::iterator it = threadMap.find(ctrl->getSockId());
			if (it == threadMap.end())
				opp_error("ICMP error message at udp layer with no corresponding socket\n");
			else
			{
				if (it->second->isServer)
					//
					// in this case we have just kill the server-thread
					// no stats for inetUser to send
					//
					removeThread(it->second->thread);
				else
				{
					//
					// the thread is killed in a not-normal way. For InetUser
					// we create a dump-statistic
					// TODO: the statistics of the thread should be sent
					//
					// THIS INDICATES A CONGESTION OR A ERROR AT SERVER
					//
					TransmissionStatistics s(0, 0, 0, 0);
					removeThread(it->second->thread, s);
				}
			}
			EV<< "Kill UDP Thread because of ICMP Error\n";
		}
		delete msg;

	}
	else
	{
		// a new connection attempt so create new thread for this communication
		// a new connection attempt has packet number 1
		// A client has to number a request with a one, the server must not
		// number the reply (otherwise the client will start a new server and so on...)
		//
		// a client calculates a static timeout when he has to send the next request.
		// if the application receives a last message (a server sends last when he
		// send out his last reply) the client has to decide to end up or to
		// continue with further requests.
		//
		GenericApplicationMessage *appmsg = dynamic_cast<GenericApplicationMessage *>(msg);
		if(!appmsg)
		{
			//
			// invalid packet
			//
			delete msg;
			return;
		}
		UDPControlInfo *ctrl = dynamic_cast<UDPControlInfo *>(msg->getControlInfo());
		if(appmsg->getPacketNumber() == 1)
		{
			// start a new thread for a new communication in case of packet number 1
			if(!ctrl)
			opp_error("received non-udp packet at udp-application\n");

			UDPSocket *socket = new UDPSocket();
			socket->setOutputGate(gate("udpOut"));
			socket->bind(ctrl->getDestPort());
			socket->connect(ctrl->getSrcAddr(), ctrl->getSrcPort());
			GenericUDPApplicationThreadBase *thread = (GenericUDPApplicationThreadBase *) new GenericUDPApplicationServerThread();
			socket->setCallbackObject(thread);
			thread->init(this, socket);
			threadCount++;
			ctrl->setSockId(socket->getSocketId()); //FIXME: quick hack to ensure functionality. maybe handle all communication via mySocket and not create new ones. need to check if this would create problems with traffic profiles

			if(threadCount == maxThreadCount)
			{
				//
				// this server will accept no more connection attempts
				// i hope you have fixed the UDP.cc - ICMP Error handling!
				//
				std::cerr<<std::endl<<"SERVER_SOCKET_CLOSED"<<std::endl;
				mySocket->close();
			}

			updateDisplay();
			ThreadMap::iterator it = threadMap.find(socket->getSocketId());
			if(it == threadMap.end())
			{
				threadMap.insert(pair<int, threadInfo *>(socket->getSocketId(), new threadInfo(true, thread)) );
			}
			else
			opp_error("socket id is already in use");

			socket->processMessage(msg);
		}
		else if(appmsg->getLast())
		{
			//
			// indicates the last message from a server. Send message to client
			// thread, which decides to end transmission or not
			//
			ThreadMap::iterator it = threadMap.find(ctrl->getSockId());
			if(it == threadMap.end())
			opp_error("received last UDP message but no client is waiting for it");
			else
			{
				if(it->second->isServer)
				opp_error("a client may not send a <last> udp message to a server\n");
				else
				//
				// send packet to client thread
				//
				it->second->thread->getSocket()->processMessage(msg);
			}
		}
		else
		//
		// all other incoming messages are just discarded
		//
		delete msg;
	}
}

void GenericUDPApplication::finish()
{
	//TODO:
}

void GenericUDPApplication::updateDisplay()
{
	if (!ev.isGUI())
		return;

	char buf[40];
	sprintf(buf, "%d threads, is %s (%s)", threadCount, isServer ? "Server" : "Client", ((threadCount < maxThreadCount) ? "on" : "off"));
	getDisplayString().setTagArg("t", 0, buf);
}
