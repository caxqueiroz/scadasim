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
class DropTailTraceQueue: public PassiveQueueBase {
private:
    int pendingRequests;
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
    virtual cMessage *enqueue(cMessage *msg);

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

    /**
     * The queue should send a packet whenever this method is invoked.
     * If the queue is currently empty, it should send a packet when
     * when one becomes available.
     */
    virtual void requestPacket();

    /**
     * Returns number of pending requests.
     */
    virtual int getNumPendingRequests();

    /**
     * Return true when queue is empty, otherwise return false.
     */
    virtual bool isEmpty();

    /**
     * Clear all queued packets and stored requests.
     */
    virtual void clear();

};

#endif /*DROPTAILTRACEQUEUE_H_*/
