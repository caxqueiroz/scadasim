#include "UDPWormVictim.h"
#include "IPControlInfo_hacked_m.h"
#include "IPv6ControlInfo_hacked_m.h"
#include "UDPPacket.h"
#include "UDPControlInfo_m.h"
#include "UDPWormQueryMessage_m.h"
#include <iostream>

using std::cerr;
using std::cout;
using std::endl;

const int UDP_HEADER_BYTES = 8;

Define_Module( UDPWormVictim);

UDPWormVictim::UDPWormVictim()
{
}

UDPWormVictim::~UDPWormVictim()
{
	if (probingTimer)
	{
		if (probingTimer->isScheduled())
			cancelAndDelete(probingTimer);
		else
			delete probingTimer;
	}
}

/**
 * Initialize parameters for worm propagation.
 * Randomly set some hosts to infected at start of the
 * propagation. All other hosts get the state vulnerable.
 */
void UDPWormVictim::initialize(int stages)
{
	if (stages != 3)
		return;

	probingDelay = par("timeBetweenProbingPackets");
	maxProbingPackets = par("maxProbingPackets");
	sourcePort = par("sourcePort");
	victimPort = par("listenAndVictimPort");
	packetLength = par("probingPacketLength");

	// ensure differentiation from DDoS attacks
	attackTag = par("attackTag");
	attackTag += FIRST_ATTACK_ID_OF_WORM_SPREAD;

	const char *beginString = par("addressRangeStart");
	addrStart.set(beginString);
	const char *endString = par("addressRangeEnd");
	addrEnd.set(endString);

	startTime = par("startProbing");
	double startTimeDiff = par("startProbingDiff");
	double startProbingProbability = par("startProbingProbability");

	// set some worm hosts randomly to active at start of propagation
	if (startProbingProbability > uniform(0, 1, 4))
		isActive = true;
	else
		isActive = false;

	probingTimer = new cMessage("timeoutForProbingPacket");
	if (isActive)
		scheduleAt(simTime() + startTime + startTimeDiff, probingTimer);
}

/**
 * If self message a new probing packet should be sent.
 * In case of a UDP packet we received a probing packet.
 * This probing causes a state change to infected if we
 * were just vulnerable till now.
 *
 * @param msg Received message
 */
void UDPWormVictim::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
	{
		// time to send the probing packet
		if (maxProbingPackets != 0)
		{
			if (maxProbingPackets > 0)
				maxProbingPackets--;

			scheduleAt(simTime() + probingDelay, msg);
			sendProbingPacket();
		}
	}
	else if (dynamic_cast<UDPPacket*> (msg))
	{
		cPacket *packet = (UDPPacket*) msg;
		if (isActive)
		{
			delete packet;
			return;
		}
		if (dynamic_cast<UDPWormQueryMessage *> (packet->getEncapsulatedMsg()))
		{
			isActive = true;

			// now i'm an active worm and start probing
			cout << "Worm at " << getFullPath() << " now active" << endl;

			scheduleAt(simTime() + probingDelay, probingTimer);
		}
		delete packet;
	}
	else
		delete msg;
}

/**
 * Create a probing packet and send it to a random host within the
 * probing address range.
 * Probing packet is encapsulated into a UDP packet.
 */
void UDPWormVictim::sendProbingPacket()
{
	IPvXAddress targetAddr;
	UDPWormQueryMessage *probing = new UDPWormQueryMessage("probing");

	probing->setByteLength(packetLength);
	targetAddr.set(intuniform(addrStart.get4().getInt(), addrEnd.get4().getInt(), 4));
	probing->setKind(UDP_I_DATA);

	UDPPacket *udpPacket = new UDPPacket("worm_udp");
	udpPacket->setByteLength(UDP_HEADER_BYTES);
	udpPacket->encapsulate(probing);
	udpPacket->setSourcePort(sourcePort);
	udpPacket->setDestinationPort(victimPort);

	if (!targetAddr.isIPv6())
	{
		IPControlInfo_hacked *ctrl = new IPControlInfo_hacked();
		ctrl->setProtocol(IP_PROT_UDP);
		ctrl->setSrcAddr(IPvXAddress().get4());
		ctrl->setDestAddr(targetAddr.get4());
		ctrl->setAttackTag(attackTag);
		udpPacket->setControlInfo(ctrl);
		send(udpPacket, "ipOut");
	}
	else
	{
		IPv6ControlInfo_hacked *ctrl = new IPv6ControlInfo_hacked();
		ctrl->setProtocol(IP_PROT_UDP);
		ctrl->setSrcAddr(IPvXAddress().get6());
		ctrl->setDestAddr(targetAddr.get6());
		ctrl->setAttackTag(attackTag);
		udpPacket->setControlInfo(ctrl);
		send(udpPacket, "to_ipv6_udp");
	}
}

