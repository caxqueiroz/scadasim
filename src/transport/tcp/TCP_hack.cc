#include "TCP_hack.h"
#include "TCPConnection_hack.h"
#include "TCPSegment.h"
#include "IPv4ControlInfo.h"
#include "TCPCommand_m.h"
//#include "DataDump.h"
Define_Module( TCP_hack);

/**
 * @brief Extends the original TCP implementation by limited number of open
 *        TCP connections
 *
 * This class is derived from the original TCP class.
 * The extension allows for simulation of a limited number of open TCP
 * connections per host system. Thus, overload situations at endsystems
 * can be simulated, too.
 *
 * @class TCP_hack
 */

/**
 * Initializes maxThreadCount to the number given in the NED file.
 * If a negative value is given, it will be set to INT_MAX.
 */
void TCP_hack::initialize()
{
	TCP::initialize();

	maxThreadCount = par("maxThreadCount");
	currentConnectionCount = 0;
	droppedConnections = 0;
	WATCH(maxThreadCount);
	WATCH(currentConnectionCount);

	if (maxThreadCount <= 0)
		maxThreadCount = INT_MAX;
}

/**
 * Similar to the createConnection method of the original TCP implementation
 */
TCPConnection *TCP_hack::createConnection(int appGateIndex, int connId)
{
	return new TCPConnection_hack(this, appGateIndex, connId);
}

int TCP_hack::getCurrentNumberOfConnection() {
	return currentConnectionCount;
}

int TCP_hack::getCurrentNumberOfDroppedConnections() {
	return droppedConnections;
}

int TCP_hack::getMaxThreadCount() {
	return maxThreadCount;
}

//void TCP_hack::dump(cMessage *msg, TCPConnection *conn, bool received) {
//	cModuleType *moduleType = cModuleType::get("scadasim.util.DataDump");
//	cModule *mod = moduleType->createScheduleInit("datadump", this);
//	DataDump *dd = check_and_cast<DataDump *> (mod);
//	dd->handleMessage(msg, conn, received);
//	dd->deleteModule();
//}
/**
 * Handle method for message that arrivedOn "appIn".
 * Handling for other messages is done by original TCP implementation
 */
void TCP_hack::handleMessage(cMessage *msg)
{
	if (msg->arrivedOn("appIn"))
	{
		TCPCommand *controlInfo = check_and_cast<TCPCommand *> (msg->getControlInfo());
		int appGateIndex = msg->getArrivalGate()->getIndex();
		int connId = controlInfo->getConnId();

		TCPConnection *conn = findConnForApp(appGateIndex, connId);

		if (!conn)
		{
			//
			// create a new connection that only accepts threadCountMax
			// concurrently connections. This is for forking-sockets only
			//
			conn = createConnection(appGateIndex, connId);

			// add into appConnMap here; it'll be added to connMap during processing
			// the OPEN command in TCPConnection's processAppCommand().
			AppConnKey key;
			key.appGateIndex = appGateIndex;
			key.connId = connId;
			tcpAppConnMap[key] = conn;

			tcpEV<< "TCP connection created for " << msg << "\n";
		}
//		else
//			if(msg->getKind()== TCP_C_SEND)
//			TCP_hack::dump(msg,conn,false);

		try {
			bool ret = conn->processAppCommand(msg);

			if (!ret)
				removeConnection(conn);
		} catch (cException& e) {
			printf("error: %s\n",e.what());
		}
		if (ev.isGUI())
			updateDisplayString();
	}
	else
	TCP::handleMessage(msg);

}


			/**
			 * @return Returns true if another connection can be accepted and false if
			 *         connection limit already is reached.
			 */
bool TCP_hack::acceptAnotherConnection()
{
	if (maxThreadCount > currentConnectionCount) {
			currentConnectionCount++;
			droppedConnections = 0;
			return true;
		}else{
			droppedConnections++;
			return false;
		}
}

/**
 * decreases number of open connections in case a half open connection is
 * closed (called by TCPConnection_hack).
 */
bool TCP_hack::canceledHalfOpenConnection()
{
	if (currentConnectionCount <= 0)
	{
		return false;
	}
	else
	{
		currentConnectionCount--;
		return true;
	}
}

