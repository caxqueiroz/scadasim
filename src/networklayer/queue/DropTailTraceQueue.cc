#include "DropTailTraceQueue.h"

Define_Module(DropTailTraceQueue);

/**
 * @brief Drop-tail queue with additional tracing support.
 *
 * Divides traffic into intervals of equal time-based sizes
 * and writes out the packet counts of different aggregates
 * observed in the last interval.
 *
 * @class DropTailTraceQueue
 */

void DropTailTraceQueue::initialize()
{
	PassiveQueueBase::initialize();
	queue.setName("l2queue");

	frameVec.setName("framesForQueue");
	dropVec.setName("dropsForQueue");

	// configuration
	frameCapacity = par("frameCapacity");
	intervalSize = par("intervalSize");
	tracingOn = par("traceOn");
	if (tracingOn)
		scheduleAt(simTime() + intervalSize, traceMessage);

	traceMessage = new cMessage("traceMsg");

	pendingRequests = dropsInInterval = framesInInterval = 0;
}

/**
 * Checks if there is enough space in queue to enqueue this message.
 * State variables drops and frames are updated accordingly.
 *
 * @param msg Message to enqueue
 * @return True if message is enqueued, false if queue is full
 */
bool DropTailTraceQueue::enqueue(cMessage *msg)
{
	if (frameCapacity && queue.length() >= frameCapacity)
	{
		dropsInInterval++;
		//delete msg;
		return false;
	}
	else
	{
		framesInInterval++;
		queue.insert(msg);
		if(pendingRequests>0){
		   cMessage *pk = dequeue();
		   sendOut(pk);
		   pendingRequests --;
		}
		return true;
	}
}

/**
 * Dequeues first message if queue is not empty
 *
 * @return Dequeued Message
 */
cMessage * DropTailTraceQueue::dequeue()
{
	if (queue.empty())
		return NULL;

	cMessage *pk = (cMessage *) queue.pop();
	return pk;
}

/**
 * Sends the given message to gate out.
 *
 * @param msg Message to be sent
 */
void DropTailTraceQueue::sendOut(cMessage *msg)
{
	send(msg, "out");
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
void DropTailTraceQueue::handleMessage(cMessage *msg)
{
	if (!tracingOn)
		PassiveQueueBase::handleMessage(msg);
	else
	{
		if (msg->isSelfMessage())
		{
			frameVec.recordWithTimestamp(simTime(), framesInInterval);
			framesInInterval = 0;
			dropVec.recordWithTimestamp(simTime(), dropsInInterval);
			dropsInInterval = 0;
			scheduleAt(simTime() + intervalSize, msg);
		}
		else
			PassiveQueueBase::handleMessage(msg);
	}
}

void DropTailTraceQueue::requestPacket(){
    if(isEmpty()){
        pendingRequests ++;
    }
}

int DropTailTraceQueue::getNumPendingRequests(){
    return pendingRequests;
}

bool DropTailTraceQueue::isEmpty(){
    return queue.isEmpty();
}

void DropTailTraceQueue::clear(){
    queue.clear();
}
