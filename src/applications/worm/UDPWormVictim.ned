package scadasim.applications.worm;


//
// This simple module implements a generator for a worm propagation.
// The infected hosts start sending probing packets to random addresses
// of the given address range. If another UDPWormVictim, that is not yet
// infected, receives such a probing packet it switches its state to 
// infected and starts itself sending probing packets. Having sent a
// maximum number of probing packets, the host switch to inactive.
//
simple UDPWormVictim

{
    parameters:
        double timeBetweenProbingPackets @unit(s);
        int attackTag;
        int maxProbingPackets;
        int sourcePort;
        int listenAndVictimPort;
        string addressRangeStart;
        string addressRangeEnd;
        double startProbing @unit(s);
        double startProbingDiff @unit(s);
        double startProbingProbability;
        int probingPacketLength @unit(B);

    gates:
        input ipIn;
        output ipOut;
}
