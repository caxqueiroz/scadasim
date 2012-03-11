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

#include "DataDump.h"
//#include "ModbusPkt_m.h"
#include "TCPSegment.h"
#include "IPDatagram_m.h"
#include <stdio.h>
#include "GenericApplicationMessage_m.h"
#include "TCPSocket.h"
#include "TCPCommand_m.h"
#include "TCP_hack.h"
#include "TCPConnection_hack.h"
#include "IPControlInfo.h"
#include "RoutingTableAccess.h"
#include "ARP.h"
#include "TCPSegment.h"
#include "IPAddressResolver.h"
#include "InterfaceEntry.h"

using namespace std;

Define_Module(DataDump);

void DataDump::initialize() {
	addr = this->par("databaseAddress");
	user = this->par("user");
	passwd = this->par("passwd");
	database = this->par("database");
	socket = NULL;
	port = this->par("port");

}

/**
 *
 */
void DataDump::handleMessage(cMessage *msg, TCPConnection *tcpConn,
		bool received) {
	Enter_Method_Silent();
	MYSQL *db;
	db = NULL;
	if (strcmp(addr, "") != 0 && strcmp(user, "") != 0
			&& strcmp(passwd, "") != 0 && strcmp(database, "") != 0) {

		if (!(db = mysql_init(db))) {
			mysql_close(db);
			EV << "MySQL error " << mysql_error(db) << endl;
		} else {
			db = mysql_real_connect(db, addr, user, passwd, database, 0, NULL,
					0);
			if (db == NULL)
				EV << "MySQL error " << mysql_error(db) << endl;
		}

	} else {
		db = NULL;
	}
	if (db) {

		DataDump::tcpDump(msg, db, received, tcpConn);

	}
	//	if (msg->getKind() == TCP_I_DATA || msg->getKind() == TCP_I_URGENT_DATA) {
	//
	//	}

}
/**
 * CREATE TABLE `webserver` (
 `id` int(11) NOT NULL auto_increment,
 `sim_time` datetime NOT NULL,
 `name` varchar(255) default NULL,
 `src` varchar(255) default NULL,
 `dest` varchar(255) default NULL,
 `direction` varchar(255) default NULL,
 `comment` varchar(255) default NULL,
 `protocol` varchar(255) default NULL,
 `queue_len` double default NULL,
 `drop_pkts` int default NULL,
 `proc_time` double default NULL,
 `lat_time` double default NULL,
 `open_conn` int default NULL,
 PRIMARY KEY  (`id`)
 ) ENGINE=MyISAM DEFAULT CHARSET=latin1
 */
void DataDump::tcpDump(cMessage *msg, MYSQL *conn, bool received,
		TCPConnection *tcpConn) {


	double simtime = SIMTIME_DBL(simTime());

	int localPort = tcpConn->localPort;
	int remotePort = tcpConn->remotePort;

	//IPControlInfo *ctrl = (IPControlInfo *) msg->getControlInfo();
	//	src = ctrl->getSrcAddr();
	//	dest = ctrl->getDestAddr();
	//short protocolInt = ctrl->getTimeToLive();

	stringstream sn;
	if (localPort == 502)
		sn << "field";
	else if (localPort == 80 || localPort == 1433 || localPort == 135)
		sn << "enterprise";
	else
		sn << "client";

	stringstream direction;
	stringstream protocol;
	if (received)
		direction << "received";
	else
		direction << "sent";

//	if (remotePort == 80 || localPort == 80)
//		protocol << "HTTP";
//	else if (remotePort == 502 || localPort == 502) {
//		protocol << "MODBUS";
//	} else if (remotePort == 135 || localPort == 135) {
//		protocol << "OPC";
//	} else if (remotePort == 1433 || localPort == 1433) {
//		protocol << "DB";
//	} else if (remotePort == 25 || localPort == 25) {
//		protocol << "SMTP";
//	} else if (remotePort == 1433 || localPort == 1433) {
//		protocol << "DB";
//	} else {
//		protocol << "local: " << localPort << "- remote: " << remotePort;
//	}
	protocol << "local: " << localPort << "- remote: " << remotePort;
	//
	//	int payLoadLen = tcpseg->getPayloadLength();

	//comment << "payload (len): " << payLoadLen << "flag: " < flag;

	//printf("%s\n",comment.str());
	/**
	 * +-----------+--------------+------+-----+---------+----------------+
	 | Field     | Type         | Null | Key | Default | Extra          |
	 +-----------+--------------+------+-----+---------+----------------+
	 | id        | int(11)      | NO   | PRI | NULL    | auto_increment |
	 | sim_time  | double       | NO   |     | NULL    |                |
	 | name      | varchar(255) | YES  |     | NULL    |                |
	 | src       | varchar(255) | YES  |     | NULL    |                |
	 | dest      | varchar(255) | YES  |     | NULL    |                |
	 | direction | varchar(255) | YES  |     | NULL    |                |
	 | comment   | varchar(255) | YES  |     | NULL    |                |
	 | protocol  | varchar(255) | YES  |     | NULL    |                |
	 | queue_len | int(11)      | YES  |     | NULL    |                |
	 | drop_pkts | int(11)      | YES  |     | NULL    |                |
	 | proc_time | double       | YES  |     | NULL    |                |
	 | lat_time  | double       | YES  |     | NULL    |                |
	 | open_conn | int(11)      | YES  |     | NULL    |                |
	 *
	 */

	char msgType[20];
	char sSQL[300];

	if (msg->getKind() == TCP_I_DATA)
		sprintf(msgType, "%s", " TCP_I_DATA");
	else if (msg->getKind() == TCP_I_ESTABLISHED) {
		sprintf(msgType, "%s", " TCP_I_ESTABLISHED");
	} else if (msg->getKind() == TCP_I_STATUS) {
		sprintf(msgType, "%s", " TCP_I_STATUS");
	} else if (msg->getKind() == TCP_I_CLOSED) {
		sprintf(msgType, "%s", " TCP_I_CLOSED");
	} else if (msg->getKind() == TCP_I_PEER_CLOSED) {
		sprintf(msgType, "%s", " TCP_I_PEER_CLOSED");
	} else if (msg->getKind() == TCP_I_CONNECTION_REFUSED) {
		sprintf(msgType, "%s", " TCP_I_CONNECTION_REFUSED");
	} else if (msg->getKind() == TCP_I_CONNECTION_RESET) {
		sprintf(msgType, "%s", " TCP_I_CONNECTION_RESET");
	}
	simtime_t proc_time = simulation.getSimTime() - msg->getArrivalTime();
	simtime_t lat_time = simulation.getSimTime() - msg->getCreationTime();
	stringstream ss;
	//ss << ret << "--- ";
	if (dynamic_cast<GenericApplicationMessage *>(msg)) {

		GenericApplicationMessage *gmsg = check_and_cast<
				GenericApplicationMessage *>(msg);
		ss << gmsg->getByteLength() << " : "
				<< ((TCPConnection_hack *) tcpConn)->getTCPRecvQueueInfo()
				<< " : " << msg->getName() << " - " << gmsg->getId();
		ss << " - " << msgType;

		//char * ret = printPath(tcpConn);
		//ss << ret << endl;
		//ss << " : " << ret << endl;
		//cout << ret << endl;
		string local = tcpConn->localAddr.get4().str();
		string remote = tcpConn->remoteAddr.get4().str();

//		const char *ret = printPath(tcpConn);
//		path = new string(ret);


		sprintf(
				sSQL,
				"insert into %s (sim_time,type,local,remote,txnid,comment,protocol,proc_time,lat_time,dropped_conn) values (%f,\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',\'%s\',%f,%f,%d)",
				"nodes",
				simtime,
				sn.str().c_str(),
				local.c_str(),
				remote.c_str(),
				msg->getName(),
				ss.str().c_str(),
				protocol.str().c_str(),
				SIMTIME_DBL(proc_time),
				SIMTIME_DBL(lat_time),
				((TCP_hack *) tcpConn->getTcpMain())->getCurrentNumberOfDroppedConnections());

	}

	int t = mysql_query(conn, sSQL);

	if (t) {

		ASSERT(0);
	} else {
		mysql_commit(conn);
	}

	mysql_close(conn);
}

/**
 * Reads complete path from attacker to victim and writes it to
 * standard out.
 */
const char * DataDump::printPath(TCPConnection *tcpConn) {
	int count = 0;
	stringstream path;
	//stringstream fpath;
	path << "<" << getParentModule()->getFullPath() << ">";
	IRoutingTable *rt = RoutingTableAccess().get();
	ARP *arp = check_and_cast<ARP*>(findModuleWherever("arp", this));
	cModule *next = NULL;
	while ((rt->getRouterId() != tcpConn->remoteAddr.get4()) && (count < 20)) {
		count++;
		next =
				(arp->gate(
						"nicOut",
						rt->getInterfaceForDestAddr(tcpConn->remoteAddr.get4())->getNetworkLayerGateIndex())->getPathEndGate())->getOwnerModule()->getParentModule();
		next =
				next->gate("phys$o")->getPathEndGate()->getOwnerModule()->getParentModule();
		path << "-->" << next->getParentModule()->getFullPath();
		rt = check_and_cast<IRoutingTable*>(
				findModuleSomewhereUp("routingTable", next));
		arp = check_and_cast<ARP*>(findModuleWherever("arp", next));
	}
	//path << endl;
	stringstream ss;
	ss << "SendMessagePath (" << simTime() << ") :" << path.str();
	return ss.str().c_str();
}

void DataDump::finish() {

}

