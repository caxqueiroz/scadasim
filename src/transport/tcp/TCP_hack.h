#ifndef TCP_HACK_H_
#define TCP_HACK_H_

#include <omnetpp.h>
#include <limits.h>
#include "TCP.h"
#include "SCADASIMDefs.h"

/**
 * @brief Extends the original TCP implementation by limited number of open 
 *        TCP connections
 *
 * This class is derived from the original TCP class.
 * The extension allows for simulation of a limited number of open TCP 
 * connections per host system. Thus, overload situations at endsystems
 * can be simulated, too.
 *
 * @class TCP_hack
 */
class SCADASIM_API TCP_hack : public TCP
{
protected:
	/// Maximum number of open TCP connections allowed
	int maxThreadCount;
	/// Current number of open TCP connections
	int currentConnectionCount;
	int droppedConnections;
public:
	/// @brief Checks wether another new connection can be opened
	bool acceptAnotherConnection();
	/// @brief Decreases number of currently open connections
	bool canceledHalfOpenConnection();

	//void dump(cMessage *msg, TCPConnection *tcpConn, bool received);
	int getCurrentNumberOfConnection();
	int getCurrentNumberOfDroppedConnections();
	int getMaxThreadCount();
protected:
	/// Factory method
	virtual TCPConnection *createConnection(int appGateIndex, int connId);
	/// Handle method for messages from application
	virtual void handleMessage(cMessage *msg);
	/// Initializiation of TCP instance and new parameters
	virtual void initialize();
};

#endif /*TCP_HACK_H_*/
