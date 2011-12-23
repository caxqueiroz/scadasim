#ifndef GENERICAPPLICATIONTIMEOUT_H_
#define GENERICAPPLICATIONTIMEOUT_H_

#include <omnetpp.h>
#include "GenericUDPApplication.h"
#include "ReaSEDefs.h"

#define MSG_KIND_TIMEOUT 1033

/**
 * @brief Timer for UDP messages
 *
 * @class GenericUDPApplicationTimeout
 */
class REASE_API GenericUDPApplicationTimeout : public cPolymorphic
{
private:
	/// Time of last timeout
	simtime_t lastTimeout;
	/// Default timer duration
	simtime_t defaultTimeout;
	/// Timeout message that is scheduled
	cMessage *timeout;
	/// Thread associated to this timeout
	GenericUDPApplicationThreadBase *user;
public:
	GenericUDPApplicationTimeout(GenericUDPApplicationThreadBase *user);
	GenericUDPApplicationTimeout(simtime_t defaultTimeout, GenericUDPApplicationThreadBase *user);
	virtual ~GenericUDPApplicationTimeout();
	/// Set timeout to two times the given RTT
	virtual void setTimeout(double RTT);
	/// Reset the timer
	virtual void reset();
};

#endif /*GENERICAPPLICATIONTIMEOUT_H_*/
