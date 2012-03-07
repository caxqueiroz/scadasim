#include <sstream>
#include <iostream>
#include <limits.h>
#include "TribeFloodNetwork.h"
#include "PingPayload_m.h"
#include "ICMP.h"
#include "IPControlInfo_hacked_m.h"
#include "IPv6ControlInfo_hacked_m.h"
#include "UDPPacket.h"
#include "UDPControlInfo_m.h"
#include "RoutingTableAccess.h"
#include "ARP.h"
#include "TCPSegment.h"
#include "IPvXAddressResolver.h"
#include "InterfaceEntry.h"

/**
 * @brief Simulates the real DDoS generation tool Tribe Flood Network (TFN)
 *
 * This class allows for simulation of realistic attack traffic according
 * to the real tool Tribe Flood Network (TFN).
 * Different types of attack can be simulated by this class.
 * Additionally, multiple attack waves and changing zombies can be simulated.
 *
 * @class TribeFloodNetwork
 */

using std::stringstream;
using std::cout;
using std::cerr;
using std::endl;

//#include "UDP.h"
//
// couldn't include this header because of linker problems
// Workaround: define the needed preprocessor directives
//
const int UDP_HEADER_BYTES = 8;
const int TYPE_START_FLOODING = 1;
const int TYPE_SEND_PACKET = 2;

#define HACK_AND_BUG

Define_Module( TribeFloodNetwork);

TribeFloodNetwork::TribeFloodNetwork()
{
	floodingTime = NULL;
	// initialize parameters containing current state values
	c_packetSize = 1;
	c_sourcePort = c_victimPort = 0;
	spoofingOn = srcPortWalkOn = dstPortWalkOn = false;
	floodingTimes = 0;
	index = 0;
	c_maxPacket = 1;

	// initialize scalar parameters
	packetsSent = 0;
}

TribeFloodNetwork::~TribeFloodNetwork()
{
	cancelAndDelete(floodingTime);
}

/**
 * Initalization step reads all necessary parameters from configuration
 */
void TribeFloodNetwork::initialize(int stages)
{
	if (stages != 4)
		return;

	// read parameters from configuration (omnetpp.ini or NED file)
	StringVector vTime = cStringTokenizer(par("attackStartTime"), " ").asVector();
	for (unsigned int i = 0; i < vTime.size(); i++)
	{
		cMessage *newFlood = new cMessage("startFlooding");
		newFlood->setKind(TYPE_START_FLOODING);
		startMsg.push_back(newFlood);
		double startTime = atof(vTime[i].data()) + par("attackStartDiff").doubleValue();
		scheduleAt(simTime() + startTime, newFlood);
	}

	StringVector vPacket = cStringTokenizer(par("maxPacket"), " ").asVector();
	for (unsigned int i = 0; i < vPacket.size(); i++)
	{
		maxPacket.push_back(atoi(vPacket[i].data()));
	}

	StringVector vType = cStringTokenizer(par("attackType"), " ").asVector();
	for (unsigned int i = 0; i < vType.size(); i++)
	{
		attackType.push_back(atoi(vType[i].data()));
	}

	StringVector vSize = cStringTokenizer(par("packetSize"), " ").asVector();
	for (unsigned int i = 0; i < vSize.size(); i++)
	{
		packetSize.push_back(atoi(vSize[i].data()));
	}

	StringVector vVPort = cStringTokenizer(par("destinationPort"), " ").asVector();
	for (unsigned int i = 0; i < vVPort.size(); i++)
	{
		victimPort.push_back(atoi(vVPort[i].data()));
	}

	StringVector vSPort = cStringTokenizer(par("sourcePort"), " ").asVector();
	for (unsigned int i = 0; i < vSPort.size(); i++)
	{
		sourcePort.push_back(atoi(vSPort[i].data()));
	}

	StringVector vAddress = cStringTokenizer(par("victimAddress"), " ").asVector();
	for (unsigned int i = 0; i < vAddress.size(); i++)
	{
		IPvXAddress victim;
		//		std::cerr<<vAddress[i].data()<<std::endl;
		victim.set(IPvXAddressResolver().resolve(vAddress[i].data()));
		victimAddr.push_back(victim);
	}

	if ((vAddress.size() != vVPort.size()) || (vVPort.size() != vSPort.size()) || (vSPort.size() != vSize.size()) || (vSize.size() != vType.size()) || (vType.size() != vPacket.size()) || (vPacket.size() != vTime.size()))
		opp_error("input parameter arrays don't have the same lengths");

	probAttackOn = par("attackOnProbability").doubleValue();
	probAttackSwitch = par("attackSwitchProbability").doubleValue();
	probAttackOff = par("attackOffProbability").doubleValue();

	sourceAddrStart.set(par("sourceAddressStart"));
	sourceAddrEnd.set(par("sourceAddressEnd"));

	// start flooding timer
	floodingTime = new cMessage("time_to_flood");

	isActive = false;
}

/**
 * Sets the parameters of the current attack to the attack with given index.
 *
 * @param i Index of attack
 */
void TribeFloodNetwork::setCurrentFloodingParameter(unsigned int i)
{
	if (i >= victimPort.size())
		opp_error("index in setCurrentFloodingParameter greater than vector size");

	floodingTimes = 0;
	dstPortWalkOn = srcPortWalkOn = false;

	c_victimPort = victimPort[i];
	if (c_victimPort == -1)
	{
		// if victim port is unspecified, the port is increased every attack packet
		dstPortWalkOn = true;
		c_victimPort = 1;
	}

	c_sourcePort = sourcePort[i];
	if (c_sourcePort == -1)
	{
		srcPortWalkOn = true;
		c_sourcePort = 9999;
	}

	c_victimAddr = victimAddr[i];
	c_packetSize = packetSize[i];

	if (c_packetSize < 1)
		c_packetSize = 1;

	c_attackType = attackType[i];
	setFloodingFunc(c_attackType);

	c_floodingDelay = par("timeBetweenPacketMin").doubleValue();
	c_floodingDelay += par("timeBetweenPacketDiff").doubleValue();

	c_maxPacket = maxPacket[i];

	if (floodingTime->isScheduled())
		cancelEvent(floodingTime);

	printAttackPath();

	// set scalar output values
	destIP = c_victimAddr;
	std::string aAddress = getParentModule()->getFullPath();
	sourceIP.set(IPvXAddressResolver().resolve(aAddress.data()));
	attackIndex = index;
	attackStart = simTime().dbl() + c_floodingDelay;
}

/**
 * Reads complete path from attacker to victim and writes it to
 * standard out.
 */
void TribeFloodNetwork::printAttackPath()
{
#ifdef HACK_AND_BUG
	int count = 0;
	stringstream path;
	path << "<" << getParentModule()->getFullPath() << ">";
	IRoutingTable *rt = RoutingTableAccess().get();
	ARP *arp = check_and_cast<ARP*> (findModuleWherever("arp", this));
	cModule *next = NULL;
	while ((rt->getRouterId() != c_victimAddr.get4()) && (count < 20))
	{
		count++;
		InterfaceEntry *ie = rt->getInterfaceForDestAddr(c_victimAddr.get4());
		next = (arp->gate("nicOut", ie->getNetworkLayerGateIndex())->getPathEndGate())->getOwnerModule()->getParentModule();
		next = next->gate("phys$o")->getPathEndGate()->getOwnerModule()->getParentModule();
		path << "-->" << next->getParentModule()->getFullPath();
		rt = check_and_cast<IRoutingTable*> (findModuleSomewhereUp("routingTable", next));
		arp = check_and_cast<ARP*> (findModuleWherever("arp", next));
	}
	path << endl;
	cout << "AttackPath (" << simTime() << ") :" << path.str() << endl;
#endif
}

/**
 * If start message is received decide if taking part in the attack,
 * stopping attack, or switching attack.
 *
 * In case of other self messages send the next attack packet by calling
 * the floodFunc.
 *
 * @param msg Received message
 */
void TribeFloodNetwork::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
	{
		if (msg->getKind() == TYPE_START_FLOODING)
		{
			double prob = uniform(0, 1, 4);
			if (isActive)
			{
				// if attack currently is active, decide if switching
				// or stopping attack
				if (prob <= probAttackSwitch)
				{
					recordAttackScalars();
					setCurrentFloodingParameter(index);
					scheduleAt(simTime() + c_floodingDelay, floodingTime);
				}
				else
				{
					prob = uniform(0, 1, 4);
					if (prob <= probAttackOff)
					{
						isActive = false;
						if (floodingTime->isScheduled())
							cancelEvent(floodingTime);
						recordAttackScalars();
					}
				}
			}
			else
			{
				// else decide if taking part in the attack
				if (prob <= probAttackOn)
				{
					setCurrentFloodingParameter(index);
					scheduleAt(simTime() + c_floodingDelay, floodingTime);
					isActive = true;
				}
			}

			index++;
			delete msg;
		}
		else
		{
			// Sends next attack packet
			(*this.*floodFunc)();
			floodingTimes++;
			packetsSent++;
			if ((floodingTimes < c_maxPacket) || (c_maxPacket < 0))
			{
				scheduleAt(simTime() + c_floodingDelay, floodingTime);
			} else {
				isActive = false;
				recordAttackScalars();
			}
		}
	}
	else
	{
		// all incoming messages are just discarded
		delete msg;
	}
}

/**
 * TCP attack packet is generated and filled with realistic data.
 * Then, the attack packet is given to IPv4 or IPv6 layer,
 * respectively, for sending.
 */
void TribeFloodNetwork::handleTCPFlood()
{
	IPvXAddress sourceAddr;
	if (spoofingOn)
		sourceAddr.set(intuniform(sourceAddrStart.get4().getInt(), sourceAddrEnd.get4().getInt(), 4));
	TCPSegment *tcpseg = new TCPSegment("SYN-Flood");

	// choose a random sequence number
	tcpseg->setSequenceNo(intuniform(1, INT_MAX, 4));

	// values are set like in TribeFloodNetwork
	switch(c_attackType) {
		case SYN_FLOODING:
			tcpseg->setSynBit(true);
			break;
		case SYN_ACK_FLOODING:
			tcpseg->setSynBit(true);
			tcpseg->setAckBit(true);
			break;
		case RST_FLOODING:
			tcpseg->setRstBit(true);
			break;
		case FIN_FLOODING:
			tcpseg->setFinBit(true);
			break;
	}
	tcpseg->setUrgBit(true);
	tcpseg->setWindow(65535);

	if (srcPortWalkOn)
		c_sourcePort--;
	if (c_sourcePort < 1)
		c_sourcePort = 9999;
	tcpseg->setSrcPort(c_sourcePort);

	if (dstPortWalkOn)
		c_victimPort++;
	if (c_victimPort > 9999)
		c_victimPort = 1;
	tcpseg->setDestPort(c_victimPort);

	tcpseg->setByteLength(TCP_HEADER_OCTETS+tcpseg->getPayloadLength());

	// now send packet to ip layer
	if (!c_victimAddr.isIPv6())
	{
		IPControlInfo_hacked *ctrl = new IPControlInfo_hacked();
		ctrl->setProtocol(IP_PROT_TCP);
		ctrl->setSrcAddr(sourceAddr.get4());
		ctrl->setDestAddr(c_victimAddr.get4());
		ctrl->setAttackTag(SYN_FLOODING);
		tcpseg->setControlInfo(ctrl);
		send(tcpseg, "to_ip_tcp");
	}
	else
	{
		IPv6ControlInfo_hacked *ctrl = new IPv6ControlInfo_hacked();
		ctrl->setProtocol(IP_PROT_TCP);
		ctrl->setSrcAddr(sourceAddr.get6());
		ctrl->setDestAddr(c_victimAddr.get6());
		ctrl->setAttackTag(SYN_FLOODING);
		tcpseg->setControlInfo(ctrl);
		send(tcpseg, "to_ipv6_tcp");
	}
}

/**
 * UDP attack packet is generated and filled with realistic data.
 * Then, the attack packet is given to IPv4 or IPv6 layer,
 * respectively, for sending.
 */
void TribeFloodNetwork::handleUDPFlood()
{
	IPvXAddress sourceAddr;
	if (spoofingOn)
		sourceAddr.set(intuniform(sourceAddrStart.get4().getInt(), sourceAddrEnd.get4().getInt(), 4));

	// FIXME: GenericUDPApplication should discard this packet!
	// DONE!
	//
	cPacket *packet = new cPacket("udp_flood");
	packet->setBitLength(c_packetSize);

	packet->setKind(UDP_I_DATA);

	UDPPacket *udpPacket = new UDPPacket("udp_flood");
	udpPacket->setByteLength(UDP_HEADER_BYTES);
	udpPacket->encapsulate(packet);

	if (srcPortWalkOn)
		c_sourcePort--;
	if (c_sourcePort < 1)
		c_sourcePort = 9999;
	udpPacket->setSourcePort(c_sourcePort);

	if (dstPortWalkOn)
		c_victimPort++;
	if (c_victimPort > 9999)
		c_victimPort = 1;
	udpPacket->setDestinationPort(c_victimPort);

	if (!c_victimAddr.isIPv6())
	{
		IPControlInfo_hacked *ctrl = new IPControlInfo_hacked();
		ctrl->setProtocol(IP_PROT_UDP);
		ctrl->setSrcAddr(sourceAddr.get4());
		ctrl->setDestAddr(c_victimAddr.get4());
		ctrl->setAttackTag(UDP_FLOODING);
		udpPacket->setControlInfo(ctrl);
		send(udpPacket, "to_ip_udp");
	}
	else
	{
		IPv6ControlInfo_hacked *ctrl = new IPv6ControlInfo_hacked();
		ctrl->setProtocol(IP_PROT_UDP);
		ctrl->setSrcAddr(sourceAddr.get6());
		ctrl->setDestAddr(c_victimAddr.get6());
		ctrl->setAttackTag(UDP_FLOODING);
		udpPacket->setControlInfo(ctrl);
		send(udpPacket, "to_ipv6_udp");
	}
}

/**
 * ICMP attack packet is generated and filled with realistic data.
 * Then, the attack packet is given to IPv4 or IPv6 layer,
 * respectively, for sending.
 */
void TribeFloodNetwork::handlePingFlood()
{
	IPvXAddress sourceAddr;
	if (spoofingOn)
		sourceAddr.set(intuniform(sourceAddrStart.get4().getInt(), sourceAddrEnd.get4().getInt(), 4));

	// create a Ping payload like in the GenericPingApplication
	PingPayload *msg = new PingPayload("ping_flood", 1);
	msg->setOriginatorId(getId());
	msg->setSeqNo(1);
	msg->setByteLength(c_packetSize);

	ICMPMessage *request = new ICMPMessage("ping_flood");
	request->setType(ICMP_ECHO_REQUEST);
	request->encapsulate(msg);

	if (!c_victimAddr.isIPv6())
	{
		IPControlInfo_hacked *ctrl = new IPControlInfo_hacked();
		ctrl->setProtocol(IP_PROT_ICMP);
		ctrl->setSrcAddr(sourceAddr.get4());
		ctrl->setDestAddr(c_victimAddr.get4());
		ctrl->setAttackTag(ICMP_FLOODING);
		request->setControlInfo(ctrl);
		send(request, "to_ip_ping");
	}
	else
	{
		IPv6ControlInfo_hacked *ctrl = new IPv6ControlInfo_hacked();
		ctrl->setProtocol(IP_PROT_ICMP);
		ctrl->setSrcAddr(sourceAddr.get6());
		ctrl->setDestAddr(c_victimAddr.get6());
		ctrl->setAttackTag(ICMP_FLOODING);
		request->setControlInfo(ctrl);
		send(request, "to_ipv6_ping");
	}
}

/**
 * Sets the floodFunc, which is responsible for sending the
 * next attack packet, according to the given attack type.
 *
 * The flooding functions differ dependent on the transport
 * protocol used by the attack packets.
 *
 * @param type Attack type of the next attack
 */
void TribeFloodNetwork::setFloodingFunc(int type)
{
	if (type > MAX_ATTACK_TYPE || type < MIN_ATTACK_TYPE)
		opp_error("invalid attackType %u\n", type);

	switch (type)
	{
		case SYN_FLOODING:
		case SYN_ACK_FLOODING:
		case RST_FLOODING:
		case FIN_FLOODING:
			floodFunc = &TribeFloodNetwork::handleTCPFlood;
			break;
		case ICMP_FLOODING:
		case SMURF_FLOODING:
			floodFunc = &TribeFloodNetwork::handlePingFlood;
			break;
		case UDP_FLOODING:
			floodFunc = &TribeFloodNetwork::handleUDPFlood;
			break;
	}
}

/**
 * This method records the scalars to OMNeT++'s scalar file as soon as an attack finished
 */
void TribeFloodNetwork::recordAttackScalars()
{
	attackEnd = simTime().dbl();

	recordScalar("Attack index", attackIndex);
	std::cout << getParentModule()->getFullPath() << " (" << sourceIP.str() << "): victim is " << destIP.str() << " (Time: " << simTime().str() << ")" << std::endl;
	//	recordScalar("Victim", destIP.str());
	//	recordScalar("Attacker", sourceIP.str());
	recordScalar("Attack started", attackStart);
	recordScalar("Attack ended", attackEnd);
	recordScalar("Attack packets sent", packetsSent);

	packetsSent = 0;
}

void TribeFloodNetwork::finish()
{
	if(isActive)
		recordAttackScalars();
}
