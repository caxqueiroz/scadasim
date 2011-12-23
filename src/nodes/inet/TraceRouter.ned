package scadasim.nodes.inet;

//
//Router-Module that traces packet
//

import inet.base.NotificationBoard;
import inet.linklayer.ethernet.EthernetInterface;
import inet.linklayer.ppp.PPPInterface;
import inet.networklayer.common.InterfaceTable;
import inet.networklayer.ipv4.RoutingTable;
import inet.util.NAMTraceWriter;


//
// \IP router that is able to trace packets
// Tracing means that the number of packets, observed within a certain
// time period, is written to a file. This observation is done 
// periodically within fix interval lengths.
//
module TraceRouter
{
    parameters:
        @node();
        string routingFile;
    gates:
        inout pppg[];
        inout ethg[];
    submodules:
        namTrace: NAMTraceWriter {
            parameters:
                namid = -1; // auto
                @display("p=330,60;i=block/sink");
        }
        notificationBoard: NotificationBoard {
            parameters:
                @display("p=60,60;i=block/control");
        }
        interfaceTable: InterfaceTable {
            parameters:
                @display("p=150,60;i=block/table");
        }
        routingTable: RoutingTable {
            parameters:
                IPForward = true;
                routerId = "auto";
                routingFile = routingFile;
                @display("p=240,60;i=block/table");
        }
        networkLayer: NetworkLayer_hack {
            parameters:
                tracingOn = true;
                spoofingOn = false;
                @display("p=200,141;i=block/fork;q=queue");
            gates:
                ifIn[sizeof(pppg)+sizeof(ethg)];
                ifOut[sizeof(pppg)+sizeof(ethg)];
        }
        ppp[sizeof(pppg)]: PPPInterface {
            parameters:
                @display("p=90,257,row,110;q=l2queue;i=block/ifcard");
        }
        eth[sizeof(ethg)]: EthernetInterface {
            parameters:
                @display("p=145,257,row,110;q=l2queue;i=block/ifcard");
        }
    connections allowunconnected:
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

