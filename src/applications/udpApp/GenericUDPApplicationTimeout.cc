#include "GenericUDPApplicationTimeout.h"

GenericUDPApplicationTimeout::GenericUDPApplicationTimeout(GenericUDPApplicationThreadBase *user)
{
	timeout = new cMessage("timeout");
	timeout->setKind(MSG_KIND_TIMEOUT);
	lastTimeout = -1.0;
	this->user = user;
}

GenericUDPApplicationTimeout::GenericUDPApplicationTimeout(simtime_t defaultTimeout, GenericUDPApplicationThreadBase *user)
{
	timeout = new cMessage("timeout");
	timeout->setKind(MSG_KIND_TIMEOUT);
	this->defaultTimeout = defaultTimeout;
	lastTimeout = 0 - defaultTimeout;
	this->user = user;
}

GenericUDPApplicationTimeout::~GenericUDPApplicationTimeout()
{
	if (timeout->isScheduled())
		user->cancelEvent(timeout);
	delete timeout;
	timeout = NULL;
}

void GenericUDPApplicationTimeout::setTimeout(double RTT)
{
	if (lastTimeout < 0)
	{
		if (timeout->isScheduled())
			user->cancelEvent(timeout);
		user->scheduleAt(simTime() - lastTimeout, timeout);
	}
	else
	{
		//
		// timeout already set
		//
		if (timeout->isScheduled())
			user->cancelEvent(timeout);
		// TODO: Timeout calculation seems to be incorrect
		user->scheduleAt(simTime() + 2 * (simTime() - lastTimeout + RTT), timeout);
		lastTimeout = simTime();
	}
}

void GenericUDPApplicationTimeout::reset()
{
	if (timeout->isScheduled())
		user->cancelEvent(timeout);
	lastTimeout = 0 - defaultTimeout;
}
