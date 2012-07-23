package scadasim.nodes.inet;

//
// Network layer of an \IP node that includes IP_hack instead of the original
// IP implementation.
// IP_hack allows for spoofing, packet tracing, etc.
// 
//


import inet.networklayer.arp.ARP;
import inet.networklayer.ipv4.ErrorHandling;
import inet.networklayer.ipv4.ICMP;
import inet.networklayer.ipv4.IGMP;
import scadasim.networklayer.ipv4.IP_hack;


//
// Network layer of an \IP node.
//
// Interfaces to transport layer: TCP, UDP, echo/ping, RSVP
//
module NetworkLayer_hack
{
    parameters:
        bool proxyARP;
        bool tracingOn;
        bool spoofingOn;
    gates:
        input ifIn[];
        input tcpIn;
        input udpIn;
        input sctpIn;   //I.R.
        input rsvpIn;
        input ospfIn;
        input pingIn;
        output ifOut[];
        output tcpOut;
        output udpOut;
        output sctpOut; //I.R.
        output rsvpOut;
        output ospfOut;
        output pingOut;
    submodules:
        ip: IP_hack {
            parameters:
                timeToLive = 125;
                multicastTimeToLive = 32;
                fragmentTimeout = 60s;
                protocolMapping = "6:0,17:1,1:2,2:3,46:4,89:5,132:6,254:7,135:7";
                spoofingOn = spoofingOn;
                tracingOn = tracingOn;
                @display("p=85,95;i=block/routing;q=queue");
            gates:
                transportIn[8];
                transportOut[8];
                queueIn[sizeof(ifIn)];
        }
        arp: ARP {
            parameters:
                proxyARP = proxyARP;
                @display("p=163,206;i=block/layer;q=pendingQueue");
            gates:
                nicOut[sizeof(ifOut)];
        }
        icmp: ICMP {
            parameters:
                @display("p=160,63;i=block/control_s");
        }
        igmp: IGMP {
            parameters:
                @display("p=160,122;i=block/cogwheel_s");
        }
        errorHandling: ErrorHandling {
            parameters:
                @display("p=239,63;i=block/process_s");
        }
    connections allowunconnected:
        // transport Layer
        ip.transportOut[0] --> tcpOut;
        ip.transportIn[0] <-- tcpIn;

        ip.transportOut[1] --> udpOut;
        ip.transportIn[1] <-- udpIn;

        ip.transportOut[2] --> icmp.localIn;
        ip.transportIn[2] <-- icmp.sendOut;

        ip.transportOut[3] --> igmp.localIn;
        ip.transportIn[3] <-- igmp.sendOut;

        ip.transportOut[4] --> rsvpOut;
        ip.transportIn[4] <-- rsvpIn;

        ip.transportOut[5] --> ospfOut;
        ip.transportIn[5] <-- ospfIn;
        
        ip.transportOut[6] --> sctpOut;     //I.R.
        ip.transportIn[6] <-- sctpIn;

        icmp.pingOut --> pingOut;
        icmp.pingIn <-- pingIn;

        icmp.errorOut --> errorHandling.in;

        ip.queueOut --> arp.ipIn;

        // L2 interfaces to IP and from ARP
        for i=0..sizeof(ifOut)-1 {
            ifIn[i] --> { @display("m=s"); } --> ip.queueIn[i];
            ifOut[i] <-- { @display("m=s"); } <-- arp.nicOut[i];
        }
}

