#include "GenericICMPPingApplication.h"
#include "PingPayload_m.h"
#include "IPv4ControlInfo.h"
#include "IPv6ControlInfo.h"
#include "IPvXAddressResolver.h"

Define_Module( GenericICMPPingApplication);

/**
 * @brief Implementation of an ICMP Ping application.
 *
 * This implementation only sends icmp ping requests.
 * It has no influence to the processing of the corresponding
 * ICMP Pong.
 *
 * @class GenericICMPPingApplication
 */

GenericICMPPingApplication::GenericICMPPingApplication()
{
	applicationType = ICMP_APP;
	selfMsg = NULL;
}

GenericICMPPingApplication::~GenericICMPPingApplication()
{
	if (selfMsg)
	{
		if (selfMsg->isScheduled())
			cancelEvent(selfMsg);
		delete selfMsg;
	}
}

/**
 * Call initialization method of super class and set own IP address
 */
void GenericICMPPingApplication::initialize(int stages)
{
	if (stages != INITIALIZATION_STAGE_NECESSARY)
		return;

	GenericApplication::initialize(stages);
	myAddr = IPvXAddressResolver().resolve(getParentModule()->getFullPath().data());
	selfMsg = new cMessage("icmp ping");
}

/**
 * In case of self message send another ping request message and schedule next
 * message with delay of timeBetweenRequests.
 * In case the transmission ends, send statistics to InetHost
 */
void GenericICMPPingApplication::handleMessage(cMessage *msg)
{
	if (msg->isSelfMessage())
	{
		if (noRequestsToSend--)
		{
			sendPing();
			scheduleAt(simTime() + curProfile.getTimeBetweenRequests(true), selfMsg);
		}
		else
		{
			// send Statistics to InetUser
			TransmissionStatistics t(noBytesSend, noBytesReceived, noPacketSend, noPacketReceived);
			transmissionDone(t);
		}
	}
	else
	{
		// receive icmp pong
		receivePong(msg);
	}
}

void GenericICMPPingApplication::finish()
{
	//TODO
}

void GenericICMPPingApplication::updateDisplay()
{
}

/**
 * Starts a new ping/pong communication
 */
void GenericICMPPingApplication::transmissionStart(TrafficProfile &p, TargetInfo &i)
{
	Enter_Method_Silent();
	curProfile = p;
	noRequestsToSend = curProfile.getRequestsPerSession(true);
	noBytesSend = noBytesReceived = noPacketSend = noPacketReceived = 0;

	curTarget = i;

	scheduleAt(simTime() + SELF_CALL_DELAY, selfMsg);
}

/**
 * Creates ping message, fill it with necessary data and send it
 */
void GenericICMPPingApplication::sendPing()
{
	PingPayload *msg = new PingPayload("ping", 1);
	msg->setOriginatorId(getId());
	msg->setSeqNo(noPacketSend++);
	int packetSize = curProfile.getRequestLength(true);
	msg->setByteLength(packetSize);

	noBytesSend += packetSize;

	if (!curTarget.address.isIPv6())
	{
		// send to IPv4
		IPv4ControlInfo *ctrl = new IPv4ControlInfo();
		ctrl->setSrcAddr(myAddr.get4());
		ctrl->setDestAddr(curTarget.address.get4());
		ctrl->setTimeToLive(curProfile.hopLimit);
		msg->setControlInfo(ctrl);
		send(msg, "pingOut");
	}
	else
	{
		// send to IPv6
		IPv6ControlInfo *ctrl = new IPv6ControlInfo();
		ctrl->setSrcAddr(myAddr.get6());
		ctrl->setDestAddr(curTarget.address.get6());
		ctrl->setHopLimit(curProfile.hopLimit);
		msg->setControlInfo(ctrl);
		send(msg, "pingv6Out");
	}
}

/**
 * Process pong messages.
 * Update statistics if ping request was initiated by myself
 */
void GenericICMPPingApplication::receivePong(cMessage *msg)
{
	PingPayload *pmsg = dynamic_cast<PingPayload*> (msg);
	if (!pmsg)
	{
		delete msg;
		return;
	}
	if (pmsg->getOriginatorId() == getId())
	{
		// this module is initiator of ping request - no more pong to send
		//
		//if(pmsg->controlInfo())
		//	delete pmsg->controlInfo();
		noPacketReceived++;
		noBytesReceived += pmsg->getByteLength();
	}
	delete msg;
}
