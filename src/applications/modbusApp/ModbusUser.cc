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

    ConnectionManagerAccess cma;

    cm = cma.get();
    if (cm == NULL)
        opp_error("couldn't get ConnectionManager");

    // set read/write watches for current traffic profile
    WATCH(curTrafficProfile.label);
    WATCH(curTrafficProfile.profileID);
    WATCH_RW(curTrafficProfile.requestLength);
    WATCH_RW(curTrafficProfile.requestsPerSession);
    WATCH_RW(curTrafficProfile.replyLength);
    WATCH_RW(curTrafficProfile.replyPerRequest);
    WATCH_RW(curTrafficProfile.timeBetweenRequests);
    WATCH_RW(curTrafficProfile.timeToRespond);
    WATCH_RW(curTrafficProfile.timeBetweenSessions);
    WATCH_RW(curTrafficProfile.probability);
    WATCH_RW(curTrafficProfile.WANprob);
    WATCH_RW(curTrafficProfile.ownPort);
    WATCH_RW(curTrafficProfile.hopLimit);

    //Set read watches for communication destination
    WATCH(curTargetInfo.address);
    WATCH(curTargetInfo.port);

    //Misc watches
    WATCH_PTRMAP(applications);
    WATCH(noTCPProfile);

    // activate the InetUser at startTime
    cMessage *startMessage = new cMessage("ModbusUser wakeup");
    startMessage->setKind(MSGKIND_START);
    scheduleAt((simtime_t) par("startTime"), startMessage);
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

    if(cm->getServerForProfile(curTargetInfo,curTrafficProfile,getId())){
        applications[TCP_APP]->transmissionStart(curTrafficProfile, curTargetInfo);
    }else
        opp_error("No valid server could be requested for selected traffic profiles (tried 100 times)");


}

void ModbusUser::setApplication(int applicationType, GenericApplication *a,
        int attachedProfileNumber) {
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
