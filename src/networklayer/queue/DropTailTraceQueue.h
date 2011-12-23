#ifndef DROPTAILTRACEQUEUE_H_
#define DROPTAILTRACEQUEUE_H_

#include <omnetpp.h>
#include "PassiveQueueBase.h"

/**
 * @brief Drop-tail queue with additional tracing support.
 *
 * Divides traffic into intervals of equal time-based length
 * and writes out the packet counts of different aggregates
 * observed in the last interval into vector file.
 *
 * @class DropTailTraceQueue
 */
class DropTailTraceQueue : public PassiveQueueBase
{
protected:
	// configuration
	int frameCapacity;
	simtime_t intervalSize;
	bool tracingOn;

	// state
	cQueue queue;
	int dropsInInterval;
	int framesInInterval;

	// statistics
	cOutVector frameVec;
	cOutVector dropVec;

	// periodic message to trigger interval end
	cMessage *traceMessage;

protected:
    virtual void initialize();

    /**
     * Redefined from PassiveQueueBase.
     */
    virtual bool enqueue(cMessage *msg);

    /**
     * Redefined from PassiveQueueBase.
     */
    virtual cMessage *dequeue();

    /**
     * Redefined from PassiveQueueBase.
     */
    virtual void sendOut(cMessage *msg);

    //virtual void finish(){/*do nothing*/}; //FIXME: is this needed? -Claus

    /**
     * Handles interval timer message.
     */
    virtual void handleMessage(cMessage *msg);

};

#endif /*DROPTAILTRACEQUEUE_H_*/
