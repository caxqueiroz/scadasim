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

/*
 * SSProxy.cpp
 *
 *  Created on: 21/04/2011
 *      Author: cax
 */

#include "SSProxy.h"
#include "SSScheduler.h"

Define_Module(SSProxy);

SSProxy::SSProxy() {

}

SSProxy::~SSProxy() {
	// TODO Auto-generated destructor stub
}

void SSProxy::initialize(){
	extAddress = par("extAddress");
	notificationMsg = new cMessage(extAddress);
	SSScheduler *ssScheduler = check_and_cast<SSScheduler *> (simulation.getScheduler());
	gate = ssScheduler->getGateway(0);
	gate->proxyBinding(this);
}

const char * SSProxy::getExtAddress(){
	return extAddress;
}
void SSProxy::setGate(SSGate *gate){
	this->gate = gate;
}

cMessage * SSProxy::getNotificationMsg(){
	return notificationMsg;
}

void SSProxy::handleMessage(cMessage *msg){

	if(msg==notificationMsg){
		cMessage *msg0 = new cMessage("sender");
		send(msg0,"out");
	}else{
		cout << "msg received from : " << msg << endl;
		delete msg;
	}
}

