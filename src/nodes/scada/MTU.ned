package scadasim.nodes.scada;

import inet.base.NotificationBoard;
import inet.linklayer.ethernet.EthernetInterface;
import inet.linklayer.ppp.PPPInterface;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.nodes.inet.NetworkLayer;
import scadasim.applications.pingApp.GenericICMPPingApplication;
import scadasim.applications.tcpApp.GenericTCPApplication;
import scadasim.transport.tcp.TCP_hack;
import scadasim.transport.udp.UDP_hack;
import inet.transport.tcp.TCP;
import scadasim.applications.generic.InetUser;


//
// WebServer: Server system for web traffic profile using TCP as 
// transport protocol.
// This profile generates small client requests and many large
// server replies.
// 
// \IP host with TCP, UDP layers and applications.
//
module MTU
{
    parameters:
        @node();
        int namid;
        int maxTCPThreadCount;

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

        inetUser: InetUser {
            parameters:
                @display("p=60,390;i=abstract/person");

        }


//        mtuServer: GenericTCPApplication {
//            parameters:
//                isServer = true;
//                profileNumber = 3;
//                port = 80;
//                
//                @display("p=163,67;i=block/app");
//
//        }

        mtuClient: GenericTCPApplication {
            parameters:
                isServer = false;
                profileNumber = 5;
                port = 0;
                @display("p=267,67;i=block/app");

        }

        tcp: TCP {
            parameters:
                //maxThreadCount = maxTCPThreadCount;

                @display("p=163,154;i=block/wheelbarrow");

        }
        //        udp: UDP_hack {
        //            parameters:
        //                @display("p=272,154;i=block/transport");
        //
        //        }
        //        pingApp: GenericICMPPingApplication {
        //            parameters:
        //                isServer = true;
        //                profileNumber = 21;
        //                port = 0;
        //                @display("i=block/app;p=343,200");
        //
        //        }
        networkLayer: NetworkLayer {
            parameters:
                proxyARP = false;
                @display("p=248,247;i=block/fork;q=queue");

            gates:
                ifIn[sizeof(pppg)+sizeof(ethg)];
                ifOut[sizeof(pppg)+sizeof(ethg)];
        }
        ppp[sizeof(pppg)]: PPPInterface {
            parameters:
                @display("p=147,350,row,90;q=txQueue;i=block/ifcard");

        }
        eth[sizeof(ethg)]: EthernetInterface {
            parameters:
                @display("p=331,343,row,90;q=txQueue;i=block/ifcard");

        }
    connections allowunconnected:
//        mtuServer.tcpOut --> tcp.appIn++;
//        mtuServer.tcpIn <-- tcp.appOut++;

        mtuClient.tcpIn <-- tcp.appOut++;
        mtuClient.tcpOut --> tcp.appIn++;

        tcp.ipOut --> networkLayer.tcpIn;
        tcp.ipIn <-- networkLayer.tcpOut;

//        udp.ipOut --> networkLayer.udpIn;
//        udp.ipIn <-- networkLayer.udpOut;

//        networkLayer.pingOut --> pingApp.pingIn;
//        networkLayer.pingIn <-- pingApp.pingOut;

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

