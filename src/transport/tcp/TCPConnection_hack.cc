#include "TCPConnection_hack.h"
#include "TCPSegment.h"
#include "TCP_hack.h"
#include "TCPReceiveQueue.h"
#include "TCPSACKRexmitQueue.h"
#include "TCPCommand_m.h"
#include "TCPSendQueue.h"
#include "TCPAlgorithm.h"

/**
 * @brief Extends the original TCP implementation by limited number of open
 *        TCP connections
 *
 * This class is derived from the original TCPConnection class.
 * The extension allows for simulation of a limited number of open TCP
 * connections per host system. Thus, overload situations at endsystems
 * can be simulated, too.
 *
 * @class TCPConnection_hack
 */
TCPConnection_hack::~TCPConnection_hack()
{
	((TCP_hack *) tcpMain)->canceledHalfOpenConnection();
}

//
// overwritten method of TCPConnection. As parameter you specify
// the num of concurrent active sockets (means the number of
// connection attempts that are accepted)
//
int TCPConnection_hack::getTCPQueueDrops(){
	return state->tcpRcvQueueDrops;
}

int TCPConnection_hack::getTCPRecvQueueLen(){
	return receiveQueue->getQueueLength();
}


const char * TCPConnection_hack::getTCPRecvQueueInfo(){
	return receiveQueue->info().c_str();
}

int TCPConnection_hack::getMaxRecvQueueLimit(){
	return  state->maxRcvBuffer;
}

int TCPConnection_hack::getFreeSpaceRecvQueue(){
	return  state->freeRcvBuffer;
}

/**
 * log data.
 */
void TCPConnection_hack::sendToApp(cMessage *msg)
{
//	if(msg->getKind()==  TCP_I_DATA)
//		((TCP_hack *) tcpMain)->dump(msg,this,true);
    tcpMain->send(msg, "appOut", appGateIndex);
}

/**
 * Process incoming TCP segment
 *
 * This method overwrites the original method of TCPConnection.
 * The only difference is that only a limited number of
 * connections is accepted.
 *
 * @return Returns a specific event code (e.g. TCP_E_RCV_SYN),
 *         which will drive the state machine.
 */
TCPEventCode TCPConnection_hack::processSegmentInListen(TCPSegment *tcpseg, IPvXAddress srcAddr, IPvXAddress destAddr)
{
	tcpEV2<< "Processing segment in LISTEN\n";

	//"
	// first check for an RST
	//   An incoming RST should be ignored.  Return.
	//"
	if (tcpseg->getRstBit())
	{
		tcpEV << "RST bit set: dropping segment\n";
		return TCP_E_IGNORE;
	}

	//"
	// second check for an ACK
	//    Any acknowledgment is bad if it arrives on a connection still in
	//    the LISTEN state.  An acceptable reset segment should be formed
	//    for any arriving ACK-bearing segment.  The RST should be
	//    formatted as follows:
	//
	//      <SEQ=SEG.ACK><CTL=RST>
	//
	//    Return.
	//"
	if (tcpseg->getAckBit())
	{
		tcpEV << "ACK bit set: dropping segment and sending RST\n";
		sendRst(tcpseg->getAckNo(),destAddr,srcAddr,tcpseg->getDestPort(),tcpseg->getSrcPort());
		return TCP_E_IGNORE;
	}

	//"
	// third check for a SYN
	//"
	if (tcpseg->getSynBit())
	{
		if (tcpseg->getFinBit())
		{
			// Looks like implementations vary on how to react to SYN+FIN.
			// Some treat it as plain SYN (and reply with SYN+ACK), some send RST+ACK.
			// Let's just do the former here.
			tcpEV << "SYN+FIN received: ignoring FIN\n";
		}

		tcpEV << "SYN bit set: filling in foreign socket and sending SYN+ACK\n";

		//"
		// If the listen was not fully specified (i.e., the foreign socket was not
		// fully specified), then the unspecified fields should be filled in now.
		//"
		//
		// Also, we may need to fork, in order to leave another connection
		// LISTENing on the port. Note: forking will change our connId.
		//
		if (state->fork)
		{
			// ReaSE:
			// we are in forking state - this means for each incoming request
			// we create a new Thread until we reach threadMax!
			if( ((TCP_hack *)tcpMain)->acceptAnotherConnection())
			{
				TCPConnection *conn = cloneListeningConnection(); // "conn" is the clone which will stay LISTENing, while "this" gets updated with the remote address

				// ReaSE: for worker Threads we use another State-Machine
				setWorkerThread();
				tcpMain->addForkedConnection(this, conn, destAddr, srcAddr, tcpseg->getDestPort(), tcpseg->getSrcPort());
				tcpEV << "Connection forked: this connection got new connId=" << connId << ", "
				"spinoff keeps LISTENing with connId=" << conn->connId << "\n";
			}
			else
			{
				tcpEV << "---> MaxThreadCount reached!!!\nNo more connections will be accepted - ignore packet\n";
				return TCP_E_IGNORE;
			}
		}
		else
		{
			tcpMain->updateSockPair(this, destAddr, srcAddr, tcpseg->getDestPort(), tcpseg->getSrcPort());
		}

		//"
		//  Set RCV.NXT to SEG.SEQ+1, IRS is set to SEG.SEQ and any other
		//  control or text should be queued for processing later.  ISS
		//  should be selected and a SYN segment sent of the form:
		//
		//    <SEQ=ISS><ACK=RCV.NXT><CTL=SYN,ACK>
		//
		//  SND.NXT is set to ISS+1 and SND.UNA to ISS.  The connection
		//  state should be changed to SYN-RECEIVED.
		//"
		state->rcv_nxt = tcpseg->getSequenceNo()+1;
        state->rcv_adv = state->rcv_nxt + state->rcv_wnd;
        if (rcvAdvVector) rcvAdvVector->record(state->rcv_adv);
		state->irs = tcpseg->getSequenceNo();
		receiveQueue->init(state->rcv_nxt); // FIXME may init twice...
		selectInitialSeqNum();

		// although not mentioned in RFC 793, seems like we have to pick up
		// initial snd_wnd from the segment here.
        updateWndInfo(tcpseg);

        if (tcpseg->getHeaderLength() > TCP_HEADER_OCTETS) // Header options present? TCP_HEADER_OCTETS = 20
            readHeaderOptions(tcpseg);

        state->ack_now = true;
		sendSynAck();
		startSynRexmitTimer();
		if (!connEstabTimer->isScheduled())
			scheduleTimeout(connEstabTimer, TCP_TIMEOUT_CONN_ESTAB);

		//"
		// Note that any other incoming control or data (combined with SYN)
		// will be processed in the SYN-RECEIVED state, but processing of SYN
		// and ACK should not be repeated.
		//"
		// We don't send text in SYN or SYN+ACK, but accept it. Otherwise
		// there isn't much left to do: RST, SYN, ACK, FIN got processed already,
		// so there's only URG and PSH left to handle.
		//
        if (tcpseg->getPayloadLength()>0)
        {
            updateRcvQueueVars();
            if (state->freeRcvBuffer >= tcpseg->getPayloadLength()) // enough freeRcvBuffer in rcvQueue for new segment?
            {
                receiveQueue->insertBytesFromSegment(tcpseg);
            }
            else    // not enough freeRcvBuffer in rcvQueue for new segment
            {
                state->tcpRcvQueueDrops++; // update current number of tcp receive queue drops
                if (tcpRcvQueueDropsVector)
                    tcpRcvQueueDropsVector->record(state->tcpRcvQueueDrops);

                tcpEV << "RcvQueueBuffer has run out, dropping segment\n";
                return TCP_E_IGNORE;
            }
        }
		if (tcpseg->getUrgBit() || tcpseg->getPshBit())
			tcpEV << "Ignoring URG and PSH bits in SYN\n"; // TBD

		return TCP_E_RCV_SYN; // this will take us to SYN_RCVD
	}

	//"
	//  fourth other text or control
	//   So you are unlikely to get here, but if you do, drop the segment, and return.
	//"
	tcpEV << "Unexpected segment: dropping it\n";
	return TCP_E_IGNORE;
}

	/**
	 * This method is copied from original TCPConnectionUtil implementation
	 */
TCPConnection *TCPConnection_hack::cloneListeningConnection()
{
	TCPConnection_hack *conn = new TCPConnection_hack(tcpMain, appGateIndex, connId);

	// following code to be kept consistent with initConnection()
	const char *sendQueueClass = sendQueue->getClassName();
	conn->sendQueue = check_and_cast<TCPSendQueue *> (createOne(sendQueueClass));
	conn->sendQueue->setConnection(conn);

	const char *receiveQueueClass = receiveQueue->getClassName();
	conn->receiveQueue = check_and_cast<TCPReceiveQueue *> (createOne(receiveQueueClass));
	conn->receiveQueue->setConnection(conn);

    // create SACK retransmit queue
    rexmitQueue = new TCPSACKRexmitQueue();
    rexmitQueue->setConnection(this);

    const char *tcpAlgorithmClass = tcpAlgorithm->getClassName();
	conn->tcpAlgorithm = check_and_cast<TCPAlgorithm *> (createOne(tcpAlgorithmClass));
	conn->tcpAlgorithm->setConnection(conn);

	conn->state = conn->tcpAlgorithm->getStateVariables();
	configureStateVariables();
	conn->tcpAlgorithm->initialize();

	// put it into LISTEN, with our localAddr/localPort
	conn->state->active = false;
	conn->state->fork = true;
	conn->localAddr = localAddr;
	conn->localPort = localPort;
	FSM_Goto(conn->fsm, TCP_S_LISTEN);

	return conn;
}

/**
 * This method overwrites the original method of TCPConnectionBase.
 * An additional differentiation has to be made for worker threads
 * (a worker thread resembles a single open or half-open TCP connection).
 */
bool TCPConnection_hack::performStateTransition(const TCPEventCode& event)
{
	ASSERT(fsm.getState() != TCP_S_CLOSED); // closed connections should be deleted immediately

	if (event == TCP_E_IGNORE) // e.g. discarded segment
	{
		tcpEV<< "Staying in state: " << stateName(fsm.getState()) << " (no FSM event)\n";
		return true;
	}

	// state machine
	// TBD add handling of connection timeout event (keepalive), with transition to CLOSED
	// Note: empty "default:" lines are for gcc's benefit which would otherwise spit warnings
	int oldState = fsm.getState();
	switch (fsm.getState())
	{
		case TCP_S_INIT:
		switch (event)
		{
			case TCP_E_OPEN_PASSIVE:FSM_Goto(fsm, TCP_S_LISTEN); break;
			case TCP_E_OPEN_ACTIVE: FSM_Goto(fsm, TCP_S_SYN_SENT); break;
			default:;
		}
		break;

		case TCP_S_LISTEN:
		switch (event)
		{
			case TCP_E_OPEN_ACTIVE: FSM_Goto(fsm, TCP_S_SYN_SENT); break;
			case TCP_E_SEND: FSM_Goto(fsm, TCP_S_SYN_SENT); break;
			case TCP_E_CLOSE: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_ABORT: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_SYN: FSM_Goto(fsm, TCP_S_SYN_RCVD);break;
			default:;
		}
		break;

		// ReaSE: for a WorkerThread we have to alter the behavior
		case TCP_S_SYN_RCVD:
		switch (event)
		{
			case TCP_E_CLOSE: FSM_Goto(fsm, TCP_S_FIN_WAIT_1); break;
			case TCP_E_ABORT: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_TIMEOUT_CONN_ESTAB:
			if(isWorkerThread())
			FSM_Goto(fsm, TCP_S_CLOSED);
			else
			FSM_Goto(fsm, state->active ? TCP_S_CLOSED : TCP_S_LISTEN);
			break;
			case TCP_E_RCV_RST:
			if(isWorkerThread())
			FSM_Goto(fsm, TCP_S_CLOSED);
			else
			FSM_Goto(fsm, state->active ? TCP_S_CLOSED : TCP_S_LISTEN);
			break;
			case TCP_E_RCV_ACK: FSM_Goto(fsm, TCP_S_ESTABLISHED); break;
			case TCP_E_RCV_FIN: FSM_Goto(fsm, TCP_S_CLOSE_WAIT); break;
			case TCP_E_RCV_UNEXP_SYN: FSM_Goto(fsm, TCP_S_CLOSED); break;
			default:;
		}
		break;

		case TCP_S_SYN_SENT:
		switch (event)
		{
			case TCP_E_CLOSE: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_ABORT: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_TIMEOUT_CONN_ESTAB: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_RST: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_SYN_ACK: FSM_Goto(fsm, TCP_S_ESTABLISHED); break;
			case TCP_E_RCV_SYN: FSM_Goto(fsm, TCP_S_SYN_RCVD); break;
			default:;
		}
		break;

		case TCP_S_ESTABLISHED:
		switch (event)
		{
			case TCP_E_CLOSE: FSM_Goto(fsm, TCP_S_FIN_WAIT_1); break;
			case TCP_E_ABORT: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_FIN: FSM_Goto(fsm, TCP_S_CLOSE_WAIT); break;
			case TCP_E_RCV_RST: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_UNEXP_SYN: FSM_Goto(fsm, TCP_S_CLOSED); break;
			default:;
		}
		break;

		case TCP_S_CLOSE_WAIT:
		switch (event)
		{
			case TCP_E_CLOSE: FSM_Goto(fsm, TCP_S_LAST_ACK); break;
			case TCP_E_ABORT: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_RST: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_UNEXP_SYN: FSM_Goto(fsm, TCP_S_CLOSED); break;
			default:;
		}
		break;

		case TCP_S_LAST_ACK:
		switch (event)
		{
			case TCP_E_ABORT: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_ACK: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_RST: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_UNEXP_SYN: FSM_Goto(fsm, TCP_S_CLOSED); break;
			default:;
		}
		break;

		case TCP_S_FIN_WAIT_1:
		switch (event)
		{
			case TCP_E_ABORT: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_FIN: FSM_Goto(fsm, TCP_S_CLOSING); break;
			case TCP_E_RCV_ACK: FSM_Goto(fsm, TCP_S_FIN_WAIT_2); break;
			case TCP_E_RCV_FIN_ACK: FSM_Goto(fsm, TCP_S_TIME_WAIT); break;
			case TCP_E_RCV_RST: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_UNEXP_SYN: FSM_Goto(fsm, TCP_S_CLOSED); break;
			default:;
		}
		break;

		case TCP_S_FIN_WAIT_2:
		switch (event)
		{
			case TCP_E_ABORT: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_FIN: FSM_Goto(fsm, TCP_S_TIME_WAIT); break;
			case TCP_E_TIMEOUT_FIN_WAIT_2: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_RST: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_UNEXP_SYN: FSM_Goto(fsm, TCP_S_CLOSED); break;
			default:;
		}
		break;

		case TCP_S_CLOSING:
		switch (event)
		{
			case TCP_E_ABORT: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_ACK: FSM_Goto(fsm, TCP_S_TIME_WAIT); break;
			case TCP_E_RCV_RST: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_UNEXP_SYN: FSM_Goto(fsm, TCP_S_CLOSED); break;
			default:;
		}
		break;

		case TCP_S_TIME_WAIT:
		switch (event)
		{
			case TCP_E_ABORT: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_TIMEOUT_2MSL: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_RST: FSM_Goto(fsm, TCP_S_CLOSED); break;
			case TCP_E_RCV_UNEXP_SYN: FSM_Goto(fsm, TCP_S_CLOSED); break;
			default:;
		}
		break;

		case TCP_S_CLOSED:
		break;
	}

	if (oldState!=fsm.getState())
	{
		tcpEV << "Transition: " << stateName(oldState) << " --> " << stateName(fsm.getState()) << "  (event was: " << eventName(event) << ")\n";
		testingEV << tcpMain->getName() << ": " << stateName(oldState) << " --> " << stateName(fsm.getState()) << "  (on " << eventName(event) << ")\n";

		// cancel timers, etc.
		stateEntered(fsm.getState());
	}
	else
	{
		tcpEV << "Staying in state: " << stateName(fsm.getState()) << " (event was: " << eventName(event) << ")\n";
	}

	return fsm.getState()!=TCP_S_CLOSED;
}

