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
// InteractiveServer: Server system for interactive traffic profile using 
// TCP as transport protocol.
// This profile generates a similar number of equally large client requests
// and server replies.
//
// \IP host with TCP, UDP layers and applications.
//
module RemoteServer
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
        remoteServer: GenericTCPApplication {
            parameters:
                isServer = true;
                profileNumber = 2;
                port = 413;
                @display("p=240,59;i=block/app");

        }
        tcp: TCP {
            parameters:
                //      maxThreadCount = maxTCPThreadCount;
                @display("p=163,154;i=block/wheelbarrow");

        }

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
                @display("p=205,350,row,90;q=txQueue;i=block/ifcard");

        }
        eth[sizeof(ethg)]: EthernetInterface {
            parameters:
                @display("p=240,350,row,90;q=txQueue;i=block/ifcard");

        }
        clientTCP: GenericTCPApplication {
            parameters:
                isServer = false;
                profileNumber = 3;
                port = 0;
                @display("p=145,53;i=block/app");

        }
        inetUser: InetUser {
            parameters:
                @display("p=60,390;i=abstract/person");

        }
    connections allowunconnected:
        remoteServer.tcpOut --> tcp.appIn++;
        remoteServer.tcpIn <-- tcp.appOut++;

        clientTCP.tcpOut --> tcp.appIn++;
        clientTCP.tcpIn <-- tcp.appOut++;

        tcp.ipOut --> networkLayer.tcpIn;
        tcp.ipIn <-- networkLayer.tcpOut;

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

