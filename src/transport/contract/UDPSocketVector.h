#ifndef UDPSOCKETVECTOR_H_
#define UDPSOCKETVECTOR_H_

#include <vector>
#include <omnetpp.h>
#include "UDPSocket.h"
#include "TransmissionConfig.h"
#include "IPvXAddress.h"
#include "SCADASIMDefs.h"
#include "UDPControlInfo.h"
#include "UDPControlInfo_m.h"
//
// this is just a copy of the functionality of TCPSocketMap
//
// compared to TCPSocketMap we use the remote Port and Address
// instead of using the connectionId.
//
//

/**
 * This struct unites the informations of a UDP socket: UDPSocket, IPvX 
 * address, and port.
 */
struct UDPSocketVectorInfo
{
	UDPSocket *socket;
	IPvXAddress remoteAddr;
	int remotePort;
	UDPSocketVectorInfo(UDPSocket *sock, IPvXAddress addr, int port)
	{
		socket = sock;
		remoteAddr =addr;
		remotePort = port;
	}
};

/**
 * @brief Small utility class for managing a large number of UDPSocket objects.
 * 
 * This class is just a copy of the functionality of TCPSocketMap.
 * In comparison to TCPSocketMap, however, remote Port and Address are used
 * for unique identification of a connection instead of connectionId. 
 *
 * @class UDPSocketVector
 */
class SCADASIM_API UDPSocketVector
{
protected:
	typedef std::vector<UDPSocketVectorInfo*> SocketVector;
	/// Vector of UDP Sockets
	SocketVector socketVector;
public:
	UDPSocketVector();
	~UDPSocketVector();
	/// Returns UDPSocket corresponding to a specific message
	UDPSocket *findSocketFor(cMessage *msg);
	/// Adds a new UDPSocket to the vector
	void addSocket(UDPSocket *socket, cMessage *initialMsg);
	/// Adds a new UDPSocket to the vector
	void addSocket(UDPSocket *socket, TargetInfo &t);
	/// Deletes a UDPSocket from the vector
	UDPSocket *removeSocket(UDPSocket *socket);
	/// Returns size of the current UDPSocket vector
	unsigned int size() {return socketVector.size();}
	/// Clear complete UDPSocket vector
	void deleteSockets();
};

#endif /*UDPSOCKETMAP_H_*/
