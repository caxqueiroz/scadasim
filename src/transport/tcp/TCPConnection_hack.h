#ifndef TCPCONNECTION_HACK_H_
#define TCPCONNECTION_HACK_H_
#include <omnetpp.h>
#include "TCPConnection.h"
#include "SCADASIMDefs.h"

/**
 * @brief Extends the original TCP implementation by limited number of open 
 *        TCP connections
 *
 * This class is derived from the original TCPConnection class.
 * The extension allows for simulation of a limited number of open TCP 
 * connections per host system. Thus, overload situations at endsystems
 * can be simulated, too.
 *
 * @class TCPConnection_hack
 */
class SCADASIM_API TCPConnection_hack: public TCPConnection {
protected:
	/// @brief state variable: Worker thread or not?
	bool worker;
public:
	TCPConnection_hack(TCP *mod, int appGateIndex, int connId) :
		TCPConnection(mod, appGateIndex, connId) {
		this->worker = false;
	}
	;
	~TCPConnection_hack();
	int getTCPQueueDrops();
	int getTCPRecvQueueLen();
	int getFreeSpaceRecvQueue();
	int getMaxRecvQueueLimit();
	const char * getTCPRecvQueueInfo();
protected:
	/// @brief Process incoming TCP segment
	TCPEventCode processSegmentInListen(TCPSegment *tcpseg, IPvXAddress src,
			IPvXAddress dest);
	/// @brief Sets this TCPConnection to the state of a worker thread
	void setWorkerThread() {
		this->worker = true;
	}
	;
	/// @brief Returns if this connections is a worker thread or not
	bool isWorkerThread() {
		return worker;
	}
	;
	/// @brief Clone a listening connection. Used for forking.
	TCPConnection *cloneListeningConnection();
	/// @brief Implements the slightly changed TCP state machine
	bool performStateTransition(const TCPEventCode& event);

	void sendToApp(cMessage *msg);
};

#endif /*TCPCONNECTION_HACK_H_*/
