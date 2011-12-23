package scadasim.nodes.inet;

import inet.base.NotificationBoard;
import inet.linklayer.ethernet.EthernetInterface;
import inet.linklayer.ppp.PPPInterface;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.util.NAMTraceWriter;
import scadasim.applications.ddos.TribeFloodNetwork;


//
// Compound module which behaves as zombie system in a DDoS attack.
// Therefore, TribeFloodNetwork is included in addition to the normal
// TCP/IP layers
//
// NetworkLayer_hack is used to spoof packets sent during the DDoS attack.
//
module DDoSZombie
{
    parameters:
        @node();
        int namid;
    gates:
        inout pppg[];
        inout ethg[];
    submodules:
        namTrace: NAMTraceWriter {
            parameters:
                namid = namid;
                @display("p=60,310;i=block/sink");

        }
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
        tribeFloodNetwork: TribeFloodNetwork {
            parameters:
                @display("p=272,154;i=old/telnet");

        }
        networkLayer: NetworkLayer_hack {
            parameters:
                proxyARP = false;
                tracingOn = false;
                spoofingOn = true;
                @display("p=248,247;i=block/fork;q=queue");

            gates:
                ifIn[sizeof(pppg)+sizeof(ethg)];
                ifOut[sizeof(pppg)+sizeof(ethg)];
        }
        ppp[sizeof(pppg)]: PPPInterface {
            parameters:
                @display("p=164,350,row,90;q=txQueue;i=block/ifcard");

        }
        eth[sizeof(ethg)]: EthernetInterface {
            parameters:
                @display("p=272,350,row,90;q=txQueue;i=block/ifcard");

        }
    connections allowunconnected:
        tribeFloodNetwork.to_ip_tcp --> networkLayer.tcpIn;
        tribeFloodNetwork.ipIn++ <-- networkLayer.tcpOut;

        tribeFloodNetwork.to_ip_udp --> networkLayer.udpIn;
        tribeFloodNetwork.ipIn++ <-- networkLayer.udpOut;

        tribeFloodNetwork.to_ip_ping --> networkLayer.pingIn;
        tribeFloodNetwork.ipIn++ <-- networkLayer.pingOut;

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

