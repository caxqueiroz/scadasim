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
//  SSScheduler.cpp
//  RTScheduler
//
//  Created by caxq on 11/05/11.
//  Copyright 2011 Carlos Queiroz. All rights reserved.
//

#include "SSScheduler.h"

Register_Class(SSScheduler)
;

SSScheduler::SSScheduler() {

}

SSScheduler::~SSScheduler() {
	// TODO Auto-generated destructor stub
}

void SSScheduler::startRun() {
	ModbusGate *gate;
	gate = new ModbusGate;
	gates.push_back(gate);
	gettimeofday(&baseTime, NULL);
	cout << "SSScheduler started at " << baseTime.tv_sec << endl;
	setupConnector();
}

void SSScheduler::setupConnector() {
	vector<SSGate *>::iterator iter;
	for (iter = gates.begin(); iter != gates.end(); ++iter) {
		(*iter)->Open();
	}
}

void SSScheduler::executionResumed() {
	gettimeofday(&baseTime, NULL);
	baseTime = timeval_substract(baseTime, SIMTIME_DBL(simTime()));
}

void SSScheduler::endRun() {
	vector<SSGate *>::iterator iter;
	for (iter = gates.begin(); iter != gates.end(); ++iter) {
		(*iter)->Close();
		delete (*iter);
	}
}

timeval SSScheduler::getBaseTime() {
	return baseTime;
}

int SSScheduler::receiveUntil(const timeval& targetTime) {
	// if there's more than 200ms to wait, wait in 100ms chunks
	// in order to keep UI responsiveness by invoking ev.idle()
	timeval curTime;
	gettimeofday(&curTime, NULL);
	while (targetTime.tv_sec - curTime.tv_sec >= 2 || timeval_diff_usec(targetTime, curTime) >= 200000) {
		vector<SSGate *>::iterator iter;
		for (iter = gates.begin(); iter != gates.end(); ++iter) {
			int retcode =(*iter)->nextMessage(100); //100 ms
			if (retcode == 0)
				return 1;
		}
		if (ev.idle())
			return -1;
		gettimeofday(&curTime, NULL);
	}

	// difference is now at most 100ms, do it at once
	long usec = timeval_diff_usec(targetTime, curTime);
	if (usec > 0) {
		vector<SSGate *>::iterator iter;
		for (iter = gates.begin(); iter != gates.end(); ++iter) {
			if((*iter)->nextMessage(usec) == 0);
				return 1;
		}
	}
	return 0;
}

cMessage *SSScheduler::getNextEvent() {
	//cout << "getNextEvent" << endl;
	//assert that we've been configured
	if (gates.size() == 0)
		throw cRuntimeError(
				"SSScheduler: setGate() not called: it must be called from a module's initialize() function");

	//	// calculate target time
	timeval targetTime;
	cMessage *msg = sim->msgQueue.peekFirst();
	if (!msg) {
		// if there are no events, wait until something comes from outside
		// TBD: obey simtimelimit, cpu-time-limit
		targetTime.tv_sec = LONG_MAX;
		targetTime.tv_usec = 0;
	} else {
		// use time of next event
		simtime_t eventSimtime = msg->getArrivalTime();
		targetTime = timeval_add(baseTime, SIMTIME_DBL(eventSimtime));
	}
	//cout << "getting here" << endl;
	//
	// if needed, wait until that time arrives
	timeval curTime;
	gettimeofday(&curTime, NULL);
	if (timeval_greater(targetTime, curTime)) {
		int status = receiveUntil(targetTime);
		if (status == -1)
			return NULL; // interrupted by user
		if (status == 1)
			msg = sim->msgQueue.peekFirst(); // received something
	} else {
		// we're behind -- customized versions of this class may
		// alert if we're too much behind, whatever that means
		cout << "------ very behind -----" << endl;
	}

	// ok, return the message
	return msg;
}

SSGate *SSScheduler::getGateway(int gateType) {
	return gates[gateType];
}

