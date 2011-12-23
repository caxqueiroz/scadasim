#include "InetUser.h"

#include "GenericICMPPingApplication.h"
#include "GenericUDPApplication.h"
#include "GenericTCPApplication.h"

/**
 * @brief Implementation of a client that generates background traffic.
 *
 * This class implements the client functionality. Clients start a first
 * transmission at startTime. Each time a new transmission is started
 * a random traffic profile and a server are requested at the connection
 * manager.
 *
 * @class InetUser
 */

using namespace std;

#define MSGKIND_START 133
#define GET_APP_TYPE(x) \
		(x < 0)?NOTSET: \
		(x < 11)?TCP_APP: \
		(x < 21)?UDP_APP: \
		(x < 31)?ICMP_APP:NOTSET

Define_Module(InetUser);

InetUser::InetUser() {
	noICMPProfile = noUDPProfile = noTCPProfile = 0;
	previousProfileId = -1;
	attachedProfileId = -1;
}

InetUser::~InetUser() {
}

/**
 * Collects some statistics of the previous transmission and
 * starts a new one.
 */
void InetUser::transmissionDone(TransmissionStatistics t) {
	Enter_Method_Silent();
	//
	// first log the transmission statistics
	//

	// first log the transmission statistics

//  std::cout<<this->fullPath()<<" --- "<<endl<<" current values: ";
//  t.printStatistics();
	switch (GET_APP_TYPE(curTrafficProfile.profileID)) {
	case TCP_APP:
		communicationStatistics.updateTcpStatistics(t);
//            std::cout<<"  tcp: ";
//            communicationStatistics.tcp.printStatistics();
		break;
	case UDP_APP:
		communicationStatistics.updateUdpStatistics(t);
//            std::cout<<"  udp: ";
//            communicationStatistics.udp.printStatistics();
		break;
	case ICMP_APP:
		communicationStatistics.updateIcmpStatistics(t);
//            std::cout<<"  icmp: ";
//            communicationStatistics.icmp.printStatistics();
		break;
	default:
		break;
	}

	communicationStatistics.updateStatistics(t);

	transmissionDone();
}

/**
 * Actually starts a new transmission.
 *
 * This method requests a traffic profile and a connection endpoint from the
 * connection manager. Dependent of the profile's transport protocol, a new
 * transmission is started.
 * In addition, it is assured that the same profile ID is not used twice in a row.
 */
void InetUser::transmissionDone() {
	if (attachedProfileId == -1) {
		int runs = 0;
		int exceptId = previousProfileId;
		do {
			runs++;
			// if configuration file for traffic profiles is edited correctly, this should not happen
			// else it is possible that a profile is returned but no server exists for this profile
			// in this case: retry profile request (this could take a while)
			if (runs == 50)
				exceptId = -1;
			cm->getServer(curTargetInfo, curTrafficProfile, exceptId, getId());

		} while (curTargetInfo.address.isUnspecified() && runs < 100);

//	std::cout << "InetUser (" << this->getFullPath() << "): starting communication with profile " << curTrafficProfile.profileID << ", target: " << curTargetInfo.address.str() << ", port: " << curTargetInfo.port << std::endl;

		if (runs == 100 && curTargetInfo.address.isUnspecified())
			opp_error(
					"No valid server could be requested for randomly selected traffic profiles (tried 100 times)");
	}else{
		cm->getServer(curTargetInfo,curTrafficProfile,getId());
	}
	// start transmission
	int appType = GET_APP_TYPE(curTrafficProfile.profileID);
	if (appType == NOTSET
		)
		opp_error("invalid traffic profile configuration!");

	switch (appType) {
	case UDP_APP:
		noUDPProfile++;
		if (curTargetInfo.port > 0) {
//			    std::cout << curTargetInfo.address.str() << ": starting udp communication for profile " << curTrafficProfile.profileID << " on port " << curTargetInfo.port << std::endl;
			applications[UDP_APP]->transmissionStart(curTrafficProfile,
					curTargetInfo);
		}
//			else 
//			    std::cout << curTargetInfo.address.str() << ": trying to start udp communication for profile " << curTrafficProfile.profileID << " on port " << curTargetInfo.port << std::endl;
		break;
	case ICMP_APP:
		noICMPProfile++;
		applications[ICMP_APP]->transmissionStart(curTrafficProfile,
				curTargetInfo);
		break;
	case TCP_APP:
		noTCPProfile++;
		if (curTargetInfo.port > 0) {
//			    std::cout << curTargetInfo.address.str() << ": starting tcp communication for profile " << curTrafficProfile.profileID << " on port " << curTargetInfo.port << std::endl;
			applications[TCP_APP]->transmissionStart(curTrafficProfile,
					curTargetInfo);
		}
//			else 
//			    std::cout << curTargetInfo.address.str() << ": trying to start tcp communication for profile " << curTrafficProfile.profileID << " on port " << curTargetInfo.port << std::endl;
		break;
	default:
		opp_error("invalid application type!");
		break;
	}
	updateDisplay();
}

/**
 * Initialization gets connection manager by access class.
 * Watches are set on the parameters of the traffic profiles.
 * Finally, the wakeup message is scheduled to startTime.
 */
void InetUser::initialize() {
	ConnectionManagerAccess cma;

	string asName = getParentModule()->getParentModule()->getFullName();
	if (asName.find("tas") != string::npos
			|| asName.find("sas") != string::npos) {
		// get AS-specific connection manager
		cm = cma.get(asName);
	} else {
		// get global connection manager
		cm = cma.get();
	}
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
	WATCH(noICMPProfile);
	WATCH(noUDPProfile);
	WATCH(noTCPProfile);

	// activate the InetUser at startTime
	cMessage *startMessage = new cMessage("InetUser wakeup");
	startMessage->setKind(MSGKIND_START);
	scheduleAt((simtime_t) par("startTime"), startMessage);
}

/*
 * Actually starts the client at startTime.
 * Then, the client immediately starts its first transmission.
 */
void InetUser::handleMessage(cMessage *msg) {
	if (msg->getKind() == MSGKIND_START)
	{
		cancelAndDelete(msg);

		// start first transmission
		transmissionDone();
	} else
		opp_error("error: unexpected msg arrived at InetUser\n");
}

/**
 * Sets application type to the given value.
 * This method is called by the application the InetUser should run.
 *
 * @param applicationType Application type
 * @param a Pointer to the generic application that called this method
 */
void InetUser::setApplication(int applicationType, GenericApplication *a, int attachedProfileNumber) {
	applications[applicationType] = a;
	if(attachedProfileNumber > 0) {
		attachedProfileId = attachedProfileNumber;
		curTrafficProfile.profileID = attachedProfileId;
	}

}

/*
 * Updates the information displayed in the GUI.
 */
void InetUser::updateDisplay() {
	if (!ev.isGUI())
		return;

	char buf[80];
	sprintf(buf, "%s connected \nto %s:%u", curTrafficProfile.label.data(),
			curTargetInfo.address.str().data(), curTargetInfo.port);
	getDisplayString().setTagArg("t", 0, buf);
}

/**
 * Output some statistics regarding the total number of communications with other entities.
 */
void InetUser::finish() {
	// write some stats
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

	recordScalar("Total UDP bytes sent", communicationStatistics.udp.bytesSent);
	recordScalar("Total UDP packets sent",
			communicationStatistics.udp.packetSent);
	recordScalar("Total UDP bytes received",
			communicationStatistics.udp.bytesReceived);
	recordScalar("Total UDP packets received",
			communicationStatistics.udp.packetReceived);
	recordScalar("Initiated UDP sessions", communicationStatistics.udpSessions);

	recordScalar("Total ICMP bytes sent",
			communicationStatistics.icmp.bytesSent);
	recordScalar("Total ICMP packets sent",
			communicationStatistics.icmp.packetSent);
	recordScalar("Total ICMP bytes received",
			communicationStatistics.icmp.bytesReceived);
	recordScalar("Total ICMP packets received",
			communicationStatistics.icmp.packetReceived);
	recordScalar("Initiated ICMP sessions",
			communicationStatistics.icmpSessions);
}

