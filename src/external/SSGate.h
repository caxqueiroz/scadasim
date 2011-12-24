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

#ifndef SSGATE_H_
#define SSGATE_H_
#include <vector>
#include "SSProxy.h"
#include "SSThread.h"
#include <iostream>
#include <omnetpp.h>
using namespace std;
class SSScheduler;

struct ssMsg {
	void * payload;
	char * extAddress;
};

class SSGate: public SSThread {

private:

	SSScheduler *scheduler;

protected:
	int port;

	vector<SSProxy *> proxies;
	SSProxy *getProxyDestination(const char * extAddress);
public:

	SSGate();

	virtual ~SSGate();

	virtual void proxyBinding(SSProxy *proxy);

	virtual void Open() = 0;

	virtual void Close() = 0;
	virtual void processMessage(ssMsg *msg);

	void nextMessage();
	int nextMessage(long ms);
};

#endif /* SSGATE_H_ */
