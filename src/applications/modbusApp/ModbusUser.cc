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

#include "ModbusUser.h"

Define_Module(ModbusUser);

void ModbusUser::initialize() {
    // TODO - Generated method body
}

void ModbusUser::handleMessage(cMessage *msg) {
    if (msg->getKind() == MSGKIND_START) {
        cancelAndDelete(msg);

        // start first transmission
        transmissionDone();
    } else
        opp_error("error: unexpected msg arrived at ModbusUser\n");
}

void ModbusUser::transmissionDone(TransmissionStatistics t) {
    communicationStatistics.updateTcpStatistics(t);
    communicationStatistics.updateStatistics(t);

    ModbusUser::transmissionDone();
}

void ModbusUser::transmissionDone() {
    applications[TCP_APP]->transmissionStart(curTrafficProfile, curTargetInfo);
}

void ModbusUser::setApplication(int applicationType, GenericApplication *a, int attachedProfileNumber) {
    applications[applicationType] = a;
    if (attachedProfileNumber > 0) {
        attachedProfileId = attachedProfileNumber;
        curTrafficProfile.profileID = attachedProfileId;
    }
}

void ModbusUser::finish() {
    recordScalar("Total bytes sent", communicationStatistics.total.bytesSent);
    recordScalar("Total packets sent",
            communicationStatistics.total.packetSent);
    recordScalar("Total bytes received",
            communicationStatistics.total.bytesReceived);
    recordScalar("Total packets received",
            communicationStatistics.total.packetReceived);
    recordScalar("Initiated sessions", communicationStatistics.totalSessions);

    recordScalar("Total TCP bytes sent", communicationStatistics.tcp.bytesSent);
    recordScalar("Total TCP packets sent",
            communicationStatistics.tcp.packetSent);
    recordScalar("Total TCP bytes received",
            communicationStatistics.tcp.bytesReceived);
    recordScalar("Total TCP packets received",
            communicationStatistics.tcp.packetReceived);
    recordScalar("Initiated TCP sessions", communicationStatistics.tcpSessions);
}
