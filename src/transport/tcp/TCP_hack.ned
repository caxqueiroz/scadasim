package scadasim.transport.tcp;

//
// Overwrites original TCP.ned file.
// This module allows for definition of a maximum number of open connections 
// that are accepted by a listening socket concurrently.
// Such a limitation is necessary to simulate overload situations.
//
simple TCP_hack extends inet.transport.tcp.TCP
{
    parameters:
        delayedAcksEnabled = default(false); // delayed ACKs enabled/disabled
        nagleEnabled = default(true); // Nagle's algorithm (RFC 896) enabled/disabled
        limitedTransmitEnabled = default(false); // Limited Transmit algorithm (RFC 3042) 
	     // enabled/disabled (can be used for TCPReno/TCPTahoe/TCPNewReno/TCPNoCongestionControl)
        increasedIWEnabled = default(false); // increased initial window (min(4*MSS, max (2*MSS, 4380 bytes))) 
	     // (RFC 3390) enabled/disabled
        sackSupport = default(false); // Selective Acknowledgment (RFC 2018, 2883, 3517) support (header option) 
	     // (SACK will be enabled for a connection if both endpoints support it)        
        mss = default(1024); // maximum segment size
        advertisedWindow = default(14*this.mss); // in bytes (Note: normally, NIC queues should be at least this size)
        tcpAlgorithmClass = default("TCPReno"); // TCPTahoe/TCPReno/TCPNoCongestionControl/DumbTCP
        sendQueueClass = default("TCPMsgBasedSendQueue");    // TCPVirtualDataSendQueue/TCPMsgBasedSendQueue
        receiveQueueClass = default("TCPMsgBasedRcvQueue"); // TCPVirtualDataRcvQueue/TCPMsgBasedRcvQueue
        recordStats = default(true); // recording seqNum etc. into output vectors on/off
        int maxThreadCount; // how many concurrent Threads are accepted
        @class(TCP_hack);
        @display("i=block/wheelbarrow");
}

