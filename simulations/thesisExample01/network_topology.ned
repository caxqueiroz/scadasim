package thesisExample01;

import scadasim.nodes.scada.PLC;
import scadasim.nodes.scada.Historian;
import scadasim.nodes.scada.MTU;
import inet.nodes.inet.Router;
import scadasim.nodes.inet.DDoSZombie;
import scadasim.nodes.inet.InetUserHost;
import scadasim.nodes.inet.BackupServer;

import scadasim.nodes.inet.InteractiveServer;
import scadasim.nodes.inet.MailServer;
import scadasim.nodes.inet.WebServer;
import scadasim.nodes.inet.StreamingServer;
import scadasim.nodes.inet.TraceRouter;
import scadasim.base.ConnectionManager;
import scadasim.base.TrafficProfileManager;
import scadasim.networklayer.autorouting.TGMNetworkConfigurator;

channel server2edge extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 10Mbps;
}

channel host2edge extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 0.128Mbps;
}
channel edge2host extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 0.768Mbps;
}
channel edge2gateway extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 155Mbps;
}
channel gateway2core extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 1000Mbps;
}
channel core2core extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 2500Mbps;
}
channel stub2stub extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 5000Mbps;
}
channel stub2transit extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 5000Mbps;
}
channel transit2transit extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 10000Mbps;
}

module Internet
{
    parameters:
        @Internet();
        @display("bgb=356,218");
    submodules:
        tgmNetworkConfigurator: TGMNetworkConfigurator {
            parameters:
                @display("p=51,29;i=abstract/table_s");
                totalCountOfAS = 2;
        }
        connectionManager: ConnectionManager {
            parameters:
                @display("p=109,20;i=block/classifier_s");
        }
        trafficProfileManager: TrafficProfileManager {
            parameters:
                @display("p=239,20;i=block/cogwheel_s");
        }

        corporate: Corporate {
            parameters:
                @AS();
                @display("i=misc/building;p=253,134");
        }
        field: Field0 {
            parameters:
                @AS();
                @display("i=misc/globe_s;p=74,134");
        }
    connections:
        corporate.pppg++ <--> stub2stub <--> field.pppg++;
}

network Example01 extends Internet
{
    parameters:
}

module Corporate
{
    gates:
        inout pppg[];
    submodules:
        connectionManager: ConnectionManager {
            parameters:
                @display("p=135,24;i=block/classifier_s");
        }

        core0: Router {
            parameters:
                @RL();
                @CoreRouter();
                @display("i=abstract/switch;p=363,339");
        }
        gw1: Router {
            parameters:
                @RL();
                @display("i=abstract/router2;p=237,316");
        }
        edge1: Router {
            parameters:
                @RL();
                @display("i=abstract/router;p=246,232");
        }
        host_z: WebServer {
            parameters:
                @RL();
                @display("i=device/laptop;p=97,199");
        }
        host_y: WebServer {
            parameters:
                @RL();
                @display("i=device/laptop;p=216,107");
        }
        host_x: WebServer {
            parameters:
                @RL();
                @display("i=device/laptop;p=335,93");
        }
        host_w: WebServer {
            parameters:
                @RL();
                @display("i=device/laptop;p=391,170");
        }
    connections:
        
        pppg++ <--> core0.pppg++;

        core0.pppg++ <--> gateway2core <--> gw1.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge1.pppg++;
        host_z.pppg$o++ --> server2edge --> edge1.pppg$i++;
        edge1.pppg$o++ --> server2edge --> host_z.pppg$i++;
        host_y.pppg$o++ --> server2edge --> edge1.pppg$i++;
        edge1.pppg$o++ --> server2edge --> host_y.pppg$i++;
        host_x.pppg$o++ --> host2edge --> edge1.pppg$i++;
        edge1.pppg$o++ --> edge2host --> host_x.pppg$i++;
        host_w.pppg$o++ --> host2edge --> edge1.pppg$i++;
        edge1.pppg$o++ --> edge2host --> host_w.pppg$i++;
}



module Field0
{
    @display("bgb=503,425");
    gates:
        inout pppg[];
    submodules:
        connectionManager: ConnectionManager {
            parameters:
                @display("p=20,20;i=block/classifier_s");
        }

        core0: Router {
            parameters:
                @RL();
                @CoreRouter();
                @display("i=abstract/switch;p=260,66");
        }
        gw2: Router {
            parameters:
                @RL();
                @display("i=abstract/router2;p=295,155");
        }
        edge6: Router {
            parameters:
                @RL();
                @display("i=abstract/router;p=241,233");
        }
        host_a: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=163,172");
        }
        host_c: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=202,317");
        }
        host_d: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=309,298");
        }
        host_b: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=113,249");
        }
    connections:
        pppg++ <--> core0.pppg++;

        core0.pppg++ <--> gateway2core <--> gw2.pppg++;
        gw2.pppg++ <--> edge2gateway <--> edge6.pppg++;
        host_a.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host_a.pppg$i++;
        host_c.pppg$o++ --> server2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> server2edge --> host_c.pppg$i++;
        host_d.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host_d.pppg$i++;
        host_b.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host_b.pppg$i++;
}




