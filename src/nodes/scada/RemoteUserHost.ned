package scadasim.nodes.scada;

import scadasim.applications.modbusApp.ModbusUser;
import inet.base.NotificationBoard;
import inet.linklayer.ethernet.EthernetInterface;
import inet.linklayer.ppp.PPPInterface;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.nodes.inet.NetworkLayer;
import scadasim.transport.tcp.TCP_hack;
import scadasim.applications.generic.InetUser;
import scadasim.applications.pingApp.GenericICMPPingApplication;
import scadasim.applications.tcpApp.GenericTCPApplication;

import scadasim.transport.udp.UDP_hack;
import inet.transport.tcp.TCP;
import inet.transport.udp.UDP;
//
// Default IP host which replaces the standard host of the original
// INET framework.
// This host system consists of TCP, UDP layers and applications.
// In comparison to StandardHost TCP_hack and UDP_hack are used.
// In addition the GenericXXApplications are used for traffic generation
// instead of the original XXApp implementations.
// Thus, only a single application is necessary per protocol type.
//
// Finally, the simple module InetUser is contained that actually requests
// traffic profiles at random times and therefore, creates self-similar
// background traffic.
//
module RemoteUserHost
{
    parameters:
        @node();
        int namid;
        int maxTCPThreadCount;
    gates:
        inout pppg[];
        inout ethg[];
    submodules:
        inetUser: ModbusUser {
            parameters:
                @display("p=60,390;i=abstract/person");

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
        clientTCP: GenericTCPApplication {
            parameters:
                isServer = false;
                profileNumber = 2;
                port = 0;
                @display("p=163,67;i=block/app");

        }
        tcp: TCP {
            parameters:
                //maxThreadCount = maxTCPThreadCount;
                @display("p=163,154;i=block/wheelbarrow");

        }
        //        udpApp: GenericUDPApplication {
        //            parameters:
        //                isServer = false;
        //                profileNumber = 13;
        //                port = 2101;
        //                noThreads = 0;
        //                @display("i=block/app;p=272,67");
        //
        //        }
        //        udp: UDP {
        //            parameters:
        //                @display("p=272,154;i=block/transport");
        //
        //        }
        //        pingApp: GenericICMPPingApplication {
        //            parameters:
        //                isServer = false;
        //                profileNumber = 21;
        //                port = 0;
        //                noThreads = 0;
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
                @display("p=163,350,row,90;q=txQueue;i=block/ifcard");

        }
        eth[sizeof(ethg)]: EthernetInterface {
            parameters:
                @display("p=297,350,row,90;q=txQueue;i=block/ifcard");

        }
    connections allowunconnected:
        clientTCP.tcpOut --> tcp.appIn++;
        clientTCP.tcpIn <-- tcp.appOut++;
        tcp.ipOut --> networkLayer.tcpIn;
        tcp.ipIn <-- networkLayer.tcpOut;

//        udpApp.udpOut --> udp.appIn++;
//        udpApp.udpIn <-- udp.appOut++;
//
//        udp.ipOut --> networkLayer.udpIn;
//        udp.ipIn <-- networkLayer.udpOut;
//
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

