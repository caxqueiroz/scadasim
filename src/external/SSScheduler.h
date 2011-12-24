//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
//

//
//  SSScheduler.h
//  RTScheduler
//
//  Created by cq on 11/05/11.
//  Copyright 2011 Carlos Queiroz. All rights reserved.
//

#ifndef SSSCHEDULER_H_
#define SSSCHEDULER_H_
#include <sys/time.h>
#include <string>
#include <vector>
#include <iostream>
#include "SSGate.h"
#include "ModbusGate.h"
#include <omnetpp.h>

using namespace std;

class SSScheduler : public cScheduler{
	
private:
	vector<SSGate *> gates;
	timeval baseTime;
    virtual void setupConnector();
//    virtual bool receiveWithTimeout(long usec);
    virtual int receiveUntil(const timeval& targetTime);
	
public:
	SSScheduler();
	virtual ~SSScheduler();
	
	/**
	 * Called at the beginning of a simulation run.
	 */
	virtual void startRun();
	
	/**
	 * Called at the end of a simulation run.
	 */
	virtual void endRun();
	
	/**
	 * Recalculates "base time" from current wall clock time.
	 */
	virtual void executionResumed();
	
	/**
	 * To be called from the module which wishes to receive data from the
	 * socket. The method must be called from the module's initialize()
	 * function.
	 */
	//	    virtual void setInterfaceModule(cModule *module, cMessage *notificationMsg,
	//	                                    uint8_t *recvBuffer, int recvBufferSize, int *numBytesPtr);
	
	/**
	 * Scheduler function -- it comes from cScheduler interface.
	 */
	virtual cMessage *getNextEvent();
	
	/**
	 * Send on the currently open connection
	 */
	//virtual void sendBytes(uint8_t * buf, size_t numBytes);
	
	virtual SSGate *getGateway(int gateType);
	virtual timeval getBaseTime();
};

#endif /* SSSCHEDULER_H_ */

