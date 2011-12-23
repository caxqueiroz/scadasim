#include "UDP_hack.h"
#include "GenericApplicationMessage_m.h"

Define_Module( UDP_hack);

/**
 * @brief Overwrites original UDP in order to process
 * undeliverable GenericAppMsg messages.
 *
 * The original functionality is kept except of the processing of undeliverable
 * GenericAppMsg messages. Such undeliverable messages are caused by the fact
 * that a client's UDP socket connection is closed too early if a UDP profile
 * of our traffic generation is used. In the original implementation all
 * incoming UDP packets then are sent back to the source as ICMP error
 * messages.
 *
 * @class UDP_hack
 */
UDP_hack::UDP_hack()
{
}

UDP_hack::~UDP_hack()
{
}

/**
 * Undeliverable GenericAppMsg messages are silently discarded if the socket
 * connection has already been closed. All other messages are processed
 * according to the original UDP implementation.
 */
void UDP_hack::processUndeliverablePacket(UDPPacket *udpPacket, cPolymorphic *ctrl)
{
	if (dynamic_cast<GenericApplicationMessage*> (udpPacket->getEncapsulatedMsg()))
	{
		delete udpPacket;
		delete ctrl;
	}
	else
		UDP::processUndeliverablePacket(udpPacket, ctrl);
}
