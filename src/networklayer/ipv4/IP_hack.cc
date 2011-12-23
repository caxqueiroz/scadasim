#include <omnetpp.h>
#include "IP_hack.h"
#include "IPControlInfo_hacked_m.h"
#include "ARPPacket_m.h"
#include "UDPPacket.h"

Define_Module( IP_hack);

IP_hack::IP_hack()
{
	tracingOn = false;
	spoofingAllowed = false;
	startTrace = false;
	totalTrace = udpTrace = tcpTrace = icmpTrace = 0;
	totalPackets = udpPackets = tcpPackets = icmpPackets = 0; 
	traceInterval = 0.;
	traceMsg = NULL;
}

IP_hack::~IP_hack()
{
	if (traceMsg)
	{
		if (traceMsg->isScheduled())
			cancelAndDelete(traceMsg);
		else
			delete traceMsg;
	}
}

void IP_hack::initialize()
{
	IP::initialize();
	spoofingAllowed = par("spoofingOn").boolValue();
	tracingOn = par("tracingOn").boolValue();
	if(spoofingAllowed)
		EV << "Spoofing is allowed for module "<<getFullPath()<<"\n";
	if(tracingOn)
	{
		EV << "Write TraceVector for module "<<getFullPath()<<"\n";
		output.setName("trace history");
		udpout.setName("udpout");
		tcpout.setName("tcpout");
		icmpout.setName("icmpout");
		traceMsg = new cMessage("traceOut");
		traceInterval = par("tracingInterval");
		double startTime = par("traceStartTime");
		if(startTime <=0)
		scheduleAt(simTime() + traceInterval, traceMsg);
		else
		scheduleAt(simTime() + startTime, traceMsg);
	}
}

/**
 * Handles interval timer message.
 *
 * In case tracing support is activated, a periodic traceMessage is sent
 * and received. In case of reception the recorded values (drops, frames)
 * of the current interval are written out to vector file.
 * Then the state variables are reset to 0 for the next interval.
 *
 * @param msg Message to handle
 */
void IP_hack::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
	{
		if (msg == traceMsg)
		{
			// start tracing in case of first message
			if (!startTrace)
			{
				startTrace = true;
				totalTrace = 0;
				icmpTrace = 0;
				tcpTrace = 0;
				udpTrace = 0;
				scheduleAt(simTime() + traceInterval, traceMsg);
				return;
			}

			// write packet counts into vector file
			if (tracingOn)
			{
				output.recordWithTimestamp(simTime(), totalTrace);
				icmpout.recordWithTimestamp(simTime(), icmpTrace);
				tcpout.recordWithTimestamp(simTime(), tcpTrace);
				udpout.recordWithTimestamp(simTime(), udpTrace);
				totalTrace = icmpTrace = tcpTrace = udpTrace = 0;
			}
			if (traceMsg->isScheduled())
				cancelEvent(traceMsg);
			scheduleAt(simTime() + traceInterval, traceMsg);
		}
		else
			AbstractQueue::handleMessage(msg);
	}
	else
		AbstractQueue::handleMessage(msg);
}

/**
 * Dispatch received message to correct handler.
 *
 * @param msg Received message to be dispatched
 */
void IP_hack::endService(cPacket *msg)
{
	if (msg->getArrivalGate()->isName("transportIn"))
	{
		handleMessageFromHL(msg);
	}
	else if (dynamic_cast<ARPPacket *> (msg))
	{
		// dispatch ARP packets to ARP
		handleARP((ARPPacket *) msg);
	}
	else if (msg->getArrivalGate()->isName("queueIn"))
	{
		IPDatagram *dgram = check_and_cast<IPDatagram *> (msg);
		handlePacketFromNetwork(dgram);
	}
	else
	{
		opp_error("msg received on IP_hack layer that cannot be handled\n");
	}

	if (ev.isGUI())
		updateDisplayString();
}

/**
 * Overloads original method to allow for address spoofing and tagging
 * of attack packets.
 * Original method encapsulates packet from transport layer into
 * IP packet.
 */

IPDatagram *IP_hack::encapsulate(cPacket *transportPacket, InterfaceEntry *&destIE)
{
	IPControlInfo *controlInfo = check_and_cast<IPControlInfo*> (transportPacket->removeControlInfo());

	IPDatagram_hacked *datagram = new IPDatagram_hacked(transportPacket->getName());
	datagram->setByteLength(IP_HEADER_BYTES);
	datagram->encapsulate(transportPacket);

	// set source and destination address
	IPAddress dest = controlInfo->getDestAddr();
	datagram->setDestAddress(dest);

	// IP_MULTICAST_IF option, but allow interface selection for unicast packets as well
	destIE = ift->getInterfaceById(controlInfo->getInterfaceId());

	IPAddress src = controlInfo->getSrcAddr();

	// ReaSE: set the attackTag in case of attack packets
	if (dynamic_cast<IPControlInfo_hacked*> (controlInfo))
		datagram->setAttackTag(((IPControlInfo_hacked *) controlInfo)->getAttackTag());

	// when source address was given, use it; otherwise it'll get the address
	// of the outgoing interface after routing
	if (!src.isUnspecified())
	{
		//if interface parameter does not match existing interface, do not send datagram
		if (rt->getInterfaceByAddress(src) == NULL)
		{
			// ReaSE: now we can apply spoofing
			if (spoofingAllowed)
				EV<< "Spoofing Sourceaddress\n";
				else
				opp_error("Wrong source address %s in (%s)%s: no interface with such address",
						src.str().c_str(), transportPacket->getClassName(), transportPacket->getFullName());
			}
			datagram->setSrcAddress(src);
		}

		// set other fields
		datagram->setDiffServCodePoint(controlInfo->getDiffServCodePoint());

		datagram->setIdentification(curFragmentId++);
		datagram->setMoreFragments(false);
		datagram->setDontFragment (controlInfo->getDontFragment());
		datagram->setFragmentOffset(0);

		datagram->setTimeToLive(
				controlInfo->getTimeToLive()> 0 ?
				controlInfo->getTimeToLive() :
				(datagram->getDestAddress().isMulticast() ? defaultMCTimeToLive : defaultTimeToLive)
		);

		datagram->setTransportProtocol(controlInfo->getProtocol());
		delete controlInfo;

		// setting IP options is currently not supported

		return datagram;
	}

/**
 * Invokes encapsulate(), then routePacket().
 *
 * This is an exact copy of the original method but it is the only way to
 * call the encapsulate methode overwritten by this class, which allows for
 * address spoofing.
 */
void IP_hack::handleMessageFromHL(cPacket *msg)
{
	// if no interface exists, do not send datagram
	if (ift->getNumInterfaces() == 0)
	{
		EV<< "No interfaces exist, dropping packet\n";
		delete msg;
		return;
	}

	// encapsulate and send
	InterfaceEntry *destIE; // will be filled in by encapsulate()
	IPDatagram *datagram = encapsulate(msg, destIE);

	// route packet
	if (!datagram->getDestAddress().isMulticast())
	routePacket(datagram, destIE, true);
	else
	routeMulticastPacket(datagram, destIE, NULL);
}

/**
 * If tracing is performed packet counts are updated here.
 *
 * @param datagram IP packet to be traced.
 */
void IP_hack::handlePacketFromNetwork(IPDatagram *datagram)
{
	// in case of tracing update packet count
	if (tracingOn)
	{
		totalTrace++;
		totalPackets++;
		switch (datagram->getTransportProtocol())
		{
			case IP_PROT_ICMP:
				icmpTrace++;
				icmpPackets++;
				break;
			case IP_PROT_TCP:
				tcpTrace++;
				tcpPackets++;
				break;
			case IP_PROT_UDP:
				udpTrace++;
				udpPackets++;
				break;
		}
	}

	// "Prerouting"

	// check for header biterror
	if (datagram->hasBitError())
	{
		// probability of bit error in header = size of header / size of total message
		// (ignore bit error if in payload)
		double relativeHeaderLength = datagram->getHeaderLength() / (double) datagram->getByteLength();
		if (dblrand() <= relativeHeaderLength)
		{
			EV<< "bit error found, sending ICMP_PARAMETER_PROBLEM\n";
			icmpAccess.get()->sendErrorMessage(datagram, ICMP_PARAMETER_PROBLEM, 0);
			return;
		}
	}

	// remove control info
	delete datagram->removeControlInfo();

	// hop counter decrement; FIXME but not if it will be locally delivered
	datagram->setTimeToLive(datagram->getTimeToLive()-1);

	// ReaSE: check for IP-Options before routing the packet
	processPacket(datagram, NULL, false, true);
}

/**
 * This method is called by handleFromNetwork and does an additional check
 * for IP options before forwarding the packet.
 */
void IP_hack::processPacket(IPDatagram *datagram, InterfaceEntry *destIE, bool fromHL, bool checkOpts)
{
	if (checkOpts && (datagram->getOptionCode() != IPOPTION_NO_OPTION))
	{
		// handleIPOption
		bool sendToControl = false;

		// check for IP-Options
		// FIXME: RFC says, the datagram could contain more than one option
		switch (datagram->getOptionCode())
		{
			case IPOPTION_END_OF_OPTIONS:
				break;
			case IPOPTION_NO_OPTION:
				break;
			case IPOPTION_SECURITY:
				//TODO
				break;
			case IPOPTION_LOOSE_SOURCE_ROUTING:
				//TODO
				break;
			case IPOPTION_TIMESTAMP:
				//TODO
				break;
			case IPOPTION_RECORD_ROUTE:
				//TODO
				break;
			case IPOPTION_STREAM_ID:
				//TODO
				break;
			case IPOPTION_STRICT_SOURCE_ROUTING:
				//TODO
				break;
			case IPOPTION_ROUTER_ALERT:
				//TODO
				break;
			default:
				opp_error("unknown IP option\n");
		}
	}

	// process local or remote routing
	//
	if (!datagram->getDestAddress().isMulticast())
		routePacket(datagram, NULL, false);
	else
		routeMulticastPacket(datagram, NULL, getSourceInterfaceFrom(datagram));
}

void IP_hack::finish() {

    recordScalar("Total packets", totalPackets);
    recordScalar("TCP packets", tcpPackets);
    recordScalar("UDP packets", udpPackets);
    recordScalar("ICMP packets", icmpPackets);
}
