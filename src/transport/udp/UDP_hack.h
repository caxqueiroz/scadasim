#ifndef UDP_HACK_H_
#define UDP_HACK_H_

#include <omnetpp.h>
#include "UDP.h"
#include "UDPPacket.h"
#include "UDPControlInfo_m.h"
#include "SCADASIMDefs.h"

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
class SCADASIM_API UDP_hack : public UDP
{
public:
	UDP_hack();
	virtual ~UDP_hack();
protected:
	/// @brief Processes undeliverable GenericAppMsg messages
	virtual void processUndeliverablePacket(UDPPacket *udpPacket, cPolymorphic *ctrl);
};

#endif /*UDP_HACK_H_*/
