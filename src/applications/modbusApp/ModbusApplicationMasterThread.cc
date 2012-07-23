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
#include "ModbusFunctionHandler.h"
#include "SimTrace.h"
#include <iostream>
#include <sstream>

ModbusApplicationMasterThread::ModbusApplicationMasterThread(TrafficProfile &p,
        TargetInfo &i) :
        GenericTCPApplicationClientThread(p, i) {
    // TODO Auto-generated constructor stub

}

ModbusApplicationMasterThread::~ModbusApplicationMasterThread() {
    // TODO Auto-generated destructor stub
}

/**
 * Process data received from server.
 * In case no more requests must be sent, wait timeBetweenRequests and close the socket.
 */
void ModbusApplicationMasterThread::socketDataArrived(int connId, void* yourPtr,
        cPacket *msg, bool urgent) {
    noBytesReceived += msg->getByteLength();
    noPacketReceived++;

    ModbusTCPApplication *app = (ModbusTCPApplication *) this->ownerModule;
    ModbusFunctionHandler *mfh = app->getModbusMessageHandler();
    mfh->processMessage(msg);

    delete msg;

    if (noRequestsToSend <= 0) {
//        if (socket->getState() == TCPSocket::PEER_CLOSED) {
//            socket->close();
//            threadState = FINISH;
//            EV << "remote TCP closed, closing here as well\n";
//        }
        scheduleAt(simTime() + SELF_CALL_DELAY, selfMsg);

    } else
        scheduleAt(simTime() + curProfile.getTimeBetweenRequests(false),
                selfMsg);
}

void ModbusApplicationMasterThread::sendRequest() {
    noRequestsToSend--;
    if (noRequestsToSend <= 0)
        threadState = DISCONNECTED;

    noPacketSend++;

    ModbusTCPApplication *app = (ModbusTCPApplication *) this->ownerModule;
    ModbusFunctionHandler *mfh = app->getModbusMessageHandler();
    ModbusMessage *mbmsg = dynamic_cast<ModbusMessage *> (mfh->createRandomMessage());
    noBytesSend += mbmsg->getByteLength();

    double timeToRespond = curProfile.getTimeToRespond(true);
    mbmsg->setReplyDelay(timeToRespond);
    mbmsg->setCloseConn(threadState == DISCONNECTED);
    SimTrace simtrace;

    int len = mbmsg->getPduArraySize();
    ostringstream stringbuilder;
    for (int i = 0; i < len; ++i) {
        if(i>5)
            stringbuilder << simtrace.hexC(mbmsg->getPdu(i));
    }
    string source = socket->getLocalAddress().get4().str();
    string dest = socket->getRemoteAddress().get4().str();

    simtrace.dump(stringbuilder.str(),source,dest,string("client"));
    socket->send(mbmsg);

}
