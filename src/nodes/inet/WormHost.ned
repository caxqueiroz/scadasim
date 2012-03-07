package scadasim.nodes.inet;

import inet.base.NotificationBoard;
import inet.linklayer.ethernet.EthernetInterface;
import inet.linklayer.ppp.PPPInterface;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;

import scadasim.applications.worm.UDPWormVictim;

//
// Compound module which behaves as infected or vulnerable worm host in a
// worm propagation.
// Therefore, UDPWormVictim is included in addition to the normal
// TCP/IP layers
//
module WormHost
{
    parameters:
        @node();
        int namid;
    gates:
        inout pppg[];
        inout ethg[];
    submodules:
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=60,70;i=block/control");
        }
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=60,150;i=block/table");
        }
        routingTable: RoutingTable {
            parameters:
                IPForward = false;
                routerId = "";
                routingFile = "";
                @display("p=60,230;i=block/table");
        }
        udpWormVictim: UDPWormVictim {
            parameters:
                @display("p=272,154;i=old/telnet");
        }
        networkLayer: NetworkLayer_hack {
            parameters:
                proxyARP = false;
                tracingOn = false;
                spoofingOn = false;
                @display("p=248,247;i=block/fork;q=queue");
            gates:
                ifIn[sizeof(pppg)+sizeof(ethg)];
                ifOut[sizeof(pppg)+sizeof(ethg)];
        }
        ppp[sizeof(pppg)]: PPPInterface {
            parameters:
                @display("p=205,350,row,90;q=txQueue;i=block/ifcard");

        }
        eth[sizeof(ethg)]: EthernetInterface {
            parameters:
                @display("p=240,350,row,90;q=txQueue;i=block/ifcard");

        }
    connections allowunconnected:
        udpWormVictim.ipOut --> networkLayer.udpIn;
        udpWormVictim.ipIn <-- networkLayer.udpOut;


        // connections to network outside
        for i=0..sizeof(pppg)-1 {
            pppg[i] <--> ppp[i].phys;
            ppp[i].netwOut --> networkLayer.ifIn[i];
            ppp[i].netwIn <-- networkLayer.ifOut[i];
        }

        for i=0..sizeof(ethg)-1 {
            ethg[i] <--> eth[i].phys;
            eth[i].netwOut --> networkLayer.ifIn[sizeof(pppg)+i];
            eth[i].netwIn <-- networkLayer.ifOut[sizeof(pppg)+i];
        }
}

