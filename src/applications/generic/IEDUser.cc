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

#include "IEDUser.h"

using namespace std;

#define MSGKIND_IEDSTART 333

Define_Module(IEDUser);

void IEDUser::initialize() {

    cMessage *startMessage = new cMessage("IEDUser wakeup");
    startMessage->setKind(MSGKIND_IEDSTART);
    scheduleAt((simtime_t) par("startTime"), startMessage);

    string peer = par("peer");

}

void IEDUser::handleMessage(cMessage *msg) {
    if(msg->isSelfMessage()){
        scheduleAt((simtime_t) par("startTime"), msg);
    }else{
        delete msg;
    }
}

void IEDUser::finish() {
    // TODO - Generated method body
}
