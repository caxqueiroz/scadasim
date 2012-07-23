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

#include "ModbusApplicationSlaveThread.h"
#include "ModbusMessage_m.h"
#include "ModbusTCPApplication.h"
#include "ModbusFunctionHandler.h"
#include "SimTrace.h"

ModbusApplicationSlaveThread::ModbusApplicationSlaveThread() {
    // TODO Auto-generated constructor stub

}

ModbusApplicationSlaveThread::~ModbusApplicationSlaveThread() {
    // TODO Auto-generated destructor stub
}

void ModbusApplicationSlaveThread::timerExpired(cMessage *msg) {
    // time to respond to request
    if (!replies.empty()) {
        ModbusMessage *mbmsg = dynamic_cast<ModbusMessage *>(replies.front());
            if (!mbmsg)
                opp_error("Message (%s) %s is not a ModbusMessage", msg->getClassName(),
                        msg->getName());
        ostringstream oss;
        SimTrace simtrace;
        int len = mbmsg->getPduArraySize();
        for (int i = 0; i < len; ++i) {

            uint8_t d = mbmsg->getPdu(i);
            oss << simtrace.hexC(d);
        }
        string source = socket->getLocalAddress().get4().str();
        string dest = socket->getRemoteAddress().get4().str();
        simtrace.dump(oss.str(), source, dest, string("server"));
        socket->send(mbmsg);
        replies.pop();
    }

    // if this was the last packet, then close the socket (on my side)
    //
    if (doClose && (socket->getState() != TCPSocket::LOCALLY_CLOSED))
        socket->close();
}

void ModbusApplicationSlaveThread::socketDataArrived(int connId, void *youtPtr,
        cPacket *msg, bool urgent) {
    ModbusMessage *mbmsg = dynamic_cast<ModbusMessage *>(msg);
    if (!mbmsg)
        opp_error("Message (%s) %s is not a ModbusMessage", msg->getClassName(),
                msg->getName());

    delete mbmsg->removeControlInfo();
    SimTrace simtrace;
    ModbusTCPApplication *app = (ModbusTCPApplication *) this->ownerModule;
    ModbusFunctionHandler *mfh = app->getModbusMessageHandler();
    mfh->processMessage(mbmsg);
    doClose = mbmsg->getCloseConn();

    // delay the reply if required by the profile
    if (mbmsg->getReplyDelay() > 0) {
        replies.push(mbmsg);
        double timeToRespond = mbmsg->getReplyDelay();
        scheduleAt(simTime() + timeToRespond, selfMsg);
    } else {
        int len = mbmsg->getPduArraySize();
        ostringstream oss;
        for (int i = 0; i < len; ++i) {

            uint8_t d = mbmsg->getPdu(i);
            oss << simtrace.hexC(d);
        }
        string source = socket->getLocalAddress().get4().str();
        string dest = socket->getRemoteAddress().get4().str();
        simtrace.dump(oss.str(), source, dest, string("slave"));

        socket->send(mbmsg);

        // if this was the last packet and no delay is needed,
        // close the socket
        if (doClose && (socket->getState() != TCPSocket::LOCALLY_CLOSED))
            socket->close();
    }
}
