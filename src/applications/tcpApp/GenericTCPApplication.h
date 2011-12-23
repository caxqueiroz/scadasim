#ifndef GENERICTCPAPPLICATION_H_
#define GENERICTCPAPPLICATION_H_

#include <omnetpp.h>
#include <list>
#include <limits.h>
#include "GenericApplication.h"
#include "TCPSocket.h"
#include "TCPSocketMap.h"

class GenericTCPApplicationThreadBase;

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
class REASE_API GenericTCPApplication: public GenericApplication
{
typedef std::list<GenericTCPApplicationThreadBase*> ThreadList;
protected:
        /// Welcome socket in case of server instance
	TCPSocket *serverSocket;
	/// Map that contains sockets of all associated threads
	TCPSocketMap socketMap;
	/// List of all associated threads
	ThreadList threadList;
	/// Maximum number of threads that may be open at the same time
	int maxThreadCount;
	/// Number of threads that are open currently
	int threadCount;
	cMessage *selfMsg;

public:
	GenericTCPApplication();
	virtual ~GenericTCPApplication();
	/// @brief Closes thread and associated socket
	void removeThread(GenericTCPApplicationThreadBase *thread);
	/// @brief Closes thread and associated socket and sends statistics to InetUser
	void removeThread(GenericTCPApplicationThreadBase *thread, TransmissionStatistics s);
	/// @brief Starts a new connection to a server (client only!)
	virtual void transmissionStart(TrafficProfile &p, TargetInfo &i);
protected:
	/// @brief Register at InetUser and ConnectionManager and open welcome socket (server only!)
	virtual void initialize(int stages);
	/// @brief Process packets of communication partners and self messages of threads
	virtual void handleMessage(cMessage *msg);
	virtual void finish();
	void updateDisplay();

};

/**
 * @brief Abstract class for implementation of client and server threads.
 *
 * These threads resemble the actual client and server functionality and
 * implement the communciation endpoints in case of a TCP transmission.
 *
 * @class GenericTCPApplicationThreadBase
 */
class REASE_API GenericTCPApplicationThreadBase : public cPolymorphic, public TCPSocket::CallbackInterface
{
protected:
	/// Pointer to application that started this thread
	GenericTCPApplication *ownerModule;
	/// Socket used by this thread
	TCPSocket *socket;


protected:
	/// Interface Methods of TCPSocket
	/// @{
	/// @brief Called in case packets arrived at the socket
	virtual void socketDataArrived(int connId, void *yourPtr, cPacket *msg, bool urgent) = 0;
	/// @brief Called in case socket is opened
	virtual void socketEstablished(int connId, void *yourPrt) = 0;
	/// @brief Called in case the peering socket is closed
	virtual void socketPeerClosed(int connId, void *yourPtr) = 0;
	/// @brief Called in case the socket is closed
	virtual void socketClosed(int connId, void *yourPtr) = 0;
	/// @brief Close thread in case of socket failure
	virtual void socketFailure(int connId, void *yourPtr, int code) {ownerModule->removeThread(this); };
	/// @brief
	virtual void socketStatusArrived(int connId, void *yourPtr, TCPStatusInfo *status) {delete status; };
	/// @}

public:
	GenericTCPApplicationThreadBase() {socket = NULL; ownerModule = NULL; };
	virtual ~GenericTCPApplicationThreadBase() {};
	/// Set owner application module and socket of this thread
	virtual void init(GenericTCPApplication *owner, TCPSocket *sock) {ownerModule = owner; socket = sock; };
	/// Handling of internal timer messages
	virtual void timerExpired(cMessage *msg) = 0;
	/// Return instance of TCPApplication that started the thread
	GenericTCPApplication *getOwnerModule() {return ownerModule; };
	/// Return socket used for this thread
	TCPSocket *getSocket() {return socket; };
	/// Delegate message scheduling to owner module
	void scheduleAt(simtime_t t, cMessage *msg) {msg->setContextPointer(this); ownerModule->scheduleAt(t, msg); };
	/// Delegate message canceling to owner module
	void cancelEvent(cMessage *msg) {ownerModule->cancelEvent(msg); };
	/// Delegate message canceling and deletion to owner module
	void cancelAndDelete(cMessage *msg) {ownerModule->cancelAndDelete(msg); };
};

#endif /*GENERICTCPAPPLICATION_H_*/
