#ifndef GENERICUDPAPPLICATION_H_
#define GENERICUDPAPPLICATION_H_

#include <omnetpp.h>
#include "GenericApplication.h"
#include "UDPSocket.h"
#include "UDPSocketVector.h"
#include <map>
#include <limits.h>

class GenericUDPApplicationThreadBase;

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
class SCADASIM_API GenericUDPApplication: public GenericApplication
{
/**
 * Structure that contains information about a UDP thread
 */
struct threadInfo{
	/// Indicates if thread belongs to a server
	bool isServer;
	/// Pointer to the actual thread
	GenericUDPApplicationThreadBase *thread;

	threadInfo(bool isServer, GenericUDPApplicationThreadBase *t)
	{
		this->isServer = isServer;
		this->thread = t;
	}
};

typedef std::map<int, threadInfo*> ThreadMap;

private:
        /// UDP socket in case of server instance
	UDPSocket *mySocket;
	/// Maximum number of threads that may be open at the same time
	int maxThreadCount;
	/// Number of threads that are open currently
	int threadCount;
	/**
	 * This map is used for error handling. If an icmp error message
	 * arrives, the corresponding socket and thread are killed.
	 */
	ThreadMap threadMap;
	cMessage *selfMsg;

public:
	GenericUDPApplication();
	virtual ~GenericUDPApplication();
	/// @brief Closes thread and associated socket
	void removeThread(GenericUDPApplicationThreadBase *thread);
	/// @brief Closes thread and associated socket and sends statistics to InetUser
	void removeThread(GenericUDPApplicationThreadBase *thread, TransmissionStatistics s);
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
 * implement the communciation endpoints in case of a UDP transmission.
 *
 * @class GenericUDPApplicationThreadBase
 */

class SCADASIM_API GenericUDPApplicationThreadBase : public cPolymorphic, public UDPSocket::CallbackInterface
{

protected:
	const static double UDP_MAX_PAYLOAD;
	/// Pointer to application that started this thread
	GenericUDPApplication *ownerModule;
	/// Socket used by this thread
	UDPSocket *socket;
	/// State of current thread
	int threadState;

protected:
	/// @brief Create a request message and send it to the server
	void sendRequest();
public:
	GenericUDPApplicationThreadBase() {ownerModule = NULL; socket = NULL;};
	virtual ~GenericUDPApplicationThreadBase() {};
	/// Set owner application module and socket of this thread
	virtual void init(GenericUDPApplication *owner, UDPSocket *sock) {ownerModule = owner; socket = sock; };
	/// Handling of internal timer messages
	virtual void timerExpired(cMessage *msg) = 0;
	/// Return instance of UDPApplication that started the thread
	GenericUDPApplication *getOwnerModule() {return ownerModule; };
	/// Delegate message scheduling to owner module
	void scheduleAt(simtime_t t, cMessage *msg) {msg->setContextPointer(this), ownerModule->scheduleAt(t, msg); };
	/// Delegate message canceling to owner module
	void cancelEvent(cMessage *msg) {ownerModule->cancelEvent(msg); };
	/// Delegate message canceling and deletion to owner module
	void cancelAndDelete(cMessage *msg) {ownerModule->cancelAndDelete(msg); };

	/// Return socket of this thread
	UDPSocket *getSocket() {return socket; };
};

#endif /*GENERICUDPAPPLICATION_H_*/
