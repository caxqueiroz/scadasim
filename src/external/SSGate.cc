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

#include "SSGate.h"
#include "SSScheduler.h"

SSGate::SSGate() {
	cout << "Hi gate" << endl;
	scheduler = check_and_cast<SSScheduler *> (simulation.getScheduler());
}

SSGate::~SSGate() {
	cout << "Bye gate" << endl;
}

SSProxy *SSGate::getProxyDestination(const char * extAddress) {
	vector<SSProxy *>::iterator iter;

	for (iter = proxies.begin(); iter != proxies.end(); ++iter) {

		const char * extAddr = (*iter)->getExtAddress();
		if (strcmp(extAddr, extAddress) == 0) {
			return *iter;
		}
	}
	return NULL;
}

int SSGate::nextMessage(long ms) {

	SSThread::lock();
	int retcode;
	if (!nextMsg) {
		retcode = SSThread::wait(ms);
	}

	nextMsg = false;
	SSThread::notify();
	SSThread::unlock();
	return retcode;
}
void SSGate::nextMessage() {
	SSThread::lock();
	if (!nextMsg) {
		SSThread::wait();
	}
	nextMsg = false;
	SSThread::notify();
	SSThread::unlock();
}

void SSGate::proxyBinding(SSProxy *proxy) {
	proxies.push_back(proxy);
	cout << "proxy added" << endl;
}

void SSGate::processMessage(ssMsg *msg) {
	SSThread::lock();
	if (nextMsg) {
		SSThread::wait();
	}
	SSProxy *dest = getProxyDestination(msg->extAddress);
	if (dest != NULL) {
		timeval curTime;
		gettimeofday(&curTime, NULL);
		curTime = timeval_substract(curTime, scheduler->getBaseTime());
		simtime_t t = curTime.tv_sec + curTime.tv_usec * 1e-6;
		cMessage *notificationMsg = dest->getNotificationMsg();

		notificationMsg->setArrival(dest, -1, t);

		simulation.msgQueue.insert(notificationMsg);
		nextMsg = true;
		SSThread::notify();

		//notify
	}
	SSThread::unlock();
	// TBD assert that it's somehow not smaller than previous event's time
	//	notificationMsg->setArrival(module, -1, t);
	//	simulation.msgQueue.insert(notificationMsg);
}
