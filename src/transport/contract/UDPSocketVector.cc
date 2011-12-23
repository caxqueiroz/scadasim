#include "UDPSocketVector.h"

/**
 * @brief Small utility class for managing a large number of UDPSocket objects.
 *
 * This class is just a copy of the functionality of TCPSocketMap.
 * In comparison to TCPSocketMap, however, remote Port and Address are used
 * for unique identification of a connection instead of connectionId.
 *
 * @class UDPSocketVector
 */
UDPSocketVector::UDPSocketVector()
{
}

UDPSocketVector::~UDPSocketVector()
{
}

/**
 * Searches for the UDPSocket that corresponds to the the remote information
 * of the received UDP message. This information is extracted of the
 * UDPControlInfo contained in UDP messages.
 *
 * @param msg Received UDP message
 * @return Corresponding UDP socket
 */
UDPSocket *UDPSocketVector::findSocketFor(cMessage *msg)
{
	UDPControlInfo *ctrl = dynamic_cast<UDPControlInfo *> (msg->getControlInfo());
	if (!ctrl)
		opp_error("UDPSocketVector: findSocketFor(): no UDPControl info in message (not from UDP?)");
	SocketVector::iterator i = socketVector.begin();
	while (i != socketVector.end())
	{
		if ((*i)->remoteAddr == ctrl->getSrcAddr())
			if ((*i)->remotePort == ctrl->getSrcPort())
				break;
		i++;
	}
	return (i == socketVector.end()) ? NULL : (*i)->socket;
}

/**
 * First the method checks if a socket with these remote informations
 * already exists. If not, the new socket is added.
 *
 * @param socket UDPSocket belonging to the received message information
 * @param init Received UDP message
 */
void UDPSocketVector::addSocket(UDPSocket *socket, cMessage *init)
{
	UDPControlInfo *ctrl = dynamic_cast<UDPControlInfo *> (init->getControlInfo());
	if (!ctrl)
		opp_error("UDPSocketVector: findSocketFor(): no UDPControl info in message (not from UDP?)");
	SocketVector::iterator i = socketVector.begin();
	while (i != socketVector.end())
	{
		if ((*i)->remoteAddr == ctrl->getSrcAddr())
			if ((*i)->remotePort == ctrl->getSrcPort())
				break;
		i++;
	}
	ASSERT(i == socketVector.end());
	socketVector.push_back(new UDPSocketVectorInfo(socket, ctrl->getSrcAddr(), ctrl->getSrcPort()));
}

/**
 * First the method checks if a socket with these remote informations
 * already exists. If not, the new socket is added.
 *
 * @param socket UDPSocket belonging to the received message information
 * @param t TargetInfo of received UDP message
 */
void UDPSocketVector::addSocket(UDPSocket *socket, TargetInfo &t)
{
	SocketVector::iterator i = socketVector.begin();
	while (i != socketVector.end())
	{
		if ((*i)->remoteAddr == t.address)
			if ((*i)->remotePort == t.port)
				break;
		i++;
	}
	ASSERT(i == socketVector.end());
	socketVector.push_back(new UDPSocketVectorInfo(socket, t.address, t.port));
}

/**
 * Checks if this socket really is contained in the vector.
 * If so, delete it from vector and return it.
 *
 * @param socket UDPSocket to delete
 * @return Returns UDPSocket that should be deleted
 */
UDPSocket *UDPSocketVector::removeSocket(UDPSocket *socket)
{
	SocketVector::iterator i = socketVector.begin();
	while (i != socketVector.end())
	{
		if ((*i)->socket->getSocketId() == socket->getSocketId())
		{
			socketVector.erase(i);
			break;
		}
	}
	return socket;
}

/**
 * Deletes all UDPSocket contained in the vector.
 */
void UDPSocketVector::deleteSockets()
{
	for (SocketVector::iterator i = socketVector.begin(); i != socketVector.end(); ++i)
		delete *i;
	socketVector.clear();
}
