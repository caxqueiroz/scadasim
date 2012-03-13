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

#include "ModbusApplicationMasterThread.h"
#include "ModbusTCP.h"
#include "ModbusMessage_m.h"
#include "ModbusTCPApplication.h"

ModbusApplicationMasterThread::ModbusApplicationMasterThread(TrafficProfile &p, TargetInfo &i):GenericTCPApplicationClientThread(p, i) {
    // TODO Auto-generated constructor stub

}

ModbusApplicationMasterThread::~ModbusApplicationMasterThread() {
    // TODO Auto-generated destructor stub
}

void ModbusApplicationMasterThread::socketDataArrived(int connId, void* yourPtr, cPacket *msg, bool urgent) {
    noBytesReceived += msg->getByteLength();
    noPacketReceived++;

    delete msg;

    if (noRequestsToSend <= 0){
        scheduleAt(simTime() + SELF_CALL_DELAY, selfMsg);
    }
    else
        scheduleAt(simTime() + curProfile.getTimeBetweenRequests(false),
                selfMsg);
}

void ModbusApplicationMasterThread::sendRequest() {
    noRequestsToSend--;
    if (noRequestsToSend <= 0)
        threadState = DISCONNECTED;

    noPacketSend++;

    ModbusTCPApplication *app = (ModbusTCPApplication *) this->ownerModule;
    ModbusTCP *modbusApp = app->getModbusTCPStack();
//    uint8_t query;
    uint8_t query[MIN_QUERY_LENGTH];
    int len = modbusApp->buildQueryBasis(0,FC_FORCE_SINGLE_COIL,0,0,query);
    noBytesSend +=len;

    ModbusMessage *mbmsg = new ModbusMessage(this->socket->getLocalAddress().str().c_str());
    mbmsg->setByteLength(len);
    mbmsg->setPduArraySize(len);
    for(int i = 0; i < len ;i++)
        mbmsg->setPdu(i,query[i]);

    double timeToRespond = curProfile.getTimeToRespond(false);
    mbmsg->setReplyDelay(timeToRespond);

    mbmsg->setCloseConn(threadState == DISCONNECTED);

    socket->send(mbmsg);

}
