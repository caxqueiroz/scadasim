package general_simulation;

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
                totalCountOfAS = 4;
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
                @display("i=misc/cloud_s;p=100,88");
        }
        remote: Remote {
            parameters:
                @AS();
                @display("i=misc/cloud_s;p=250,88");
        }
        field0: Field0 {
            parameters:
                @AS();
                @display("i=misc/globe_s;p=100,177");
        }
        field1: Field1 {
            parameters:
                @AS();
                @display("i=misc/globe_s;p=250,177");
        }

    connections:
        corporate.pppg++ <--> stub2stub <--> remote.pppg++;
        corporate.pppg++ <--> stub2stub <--> field0.pppg++;
        corporate.pppg++ <--> stub2stub <--> field1.pppg++;

}

network SCADA extends Internet
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
                @display("i=abstract/switch");
        }
        gw1: Router {
            parameters:
                @RL();
                @display("i=abstract/router2");
        }
        gw2: Router {
            parameters:
                @RL();
                @display("i=abstract/router2;p=318,340");
        }
        edge3: Router {
            parameters:
                @RL();
                @display("i=abstract/router;p=301,533");
        }
        edge5: Router {
            parameters:
                @RL();
                @display("i=abstract/router;p=561,472");
        }
        edge8: Router {
            parameters:
                @RL();
                @display("i=abstract/router;p=246,232");
        }
        edge10: Router {
            parameters:
                @RL();
                @display("i=abstract/router;p=127,390");
        }
        host13: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop;p=301,753");
        }
        host14: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop;p=262,702");
        }
        host18: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop;p=283,617");
        }
        host_MTU29: MTU {
            parameters:
                @RL();
                @display("i=device/laptop;p=629,533");
        }
        host_MTU30: MTU {
            parameters:
                @RL();
                @display("i=device/laptop;p=461,556");
        }
        host_WebServer47: WebServer {
            parameters:
                @RL();
                @display("i=device/laptop;p=97,199");
        }
        host48: WebServer {
            parameters:
                @RL();
                @display("i=device/laptop;p=333,232");
        }
        host_WebServer49: WebServer {
            parameters:
                @RL();
                @display("i=device/laptop;p=207,117");
        }
        host50: WebServer {
            parameters:
                @RL();
                @display("i=device/laptop;p=415,82");
        }
        host52: WebServer {
            parameters:
                @RL();
                @display("i=device/laptop;p=318,170");
        }
        host53: WebServer {
            parameters:
                @RL();
                @display("i=device/laptop;p=136,123");
        }
        host_MailServer64: MailServer {
            parameters:
                @RL();
                @display("i=device/laptop;p=194,498");
        }
        host_Historian6: Historian {
            parameters:
                @RL();
                @display("i=device/laptop;p=97,508");
        }
    connections:
        pppg++ <--> core0.pppg++;
        pppg++ <--> core0.pppg++;
        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;

        core0.pppg++ <--> gateway2core <--> gw1.pppg++;
        core0.pppg++ <--> gateway2core <--> gw2.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge3.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge5.pppg++;
        gw2.pppg++ <--> edge2gateway <--> edge8.pppg++;
        gw2.pppg++ <--> edge2gateway <--> edge10.pppg++;
        host13.pppg$o++ --> host2edge --> edge3.pppg$i++;
        edge3.pppg$o++ --> edge2host --> host13.pppg$i++;
        host14.pppg$o++ --> host2edge --> edge3.pppg$i++;
        edge3.pppg$o++ --> edge2host --> host14.pppg$i++;
        host18.pppg$o++ --> host2edge --> edge3.pppg$i++;
        edge3.pppg$o++ --> edge2host --> host18.pppg$i++;
        host_MTU29.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host_MTU29.pppg$i++;
        host_MTU30.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host_MTU30.pppg$i++;
        host_WebServer47.pppg$o++ --> server2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> server2edge --> host_WebServer47.pppg$i++;
        host48.pppg$o++ --> host2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> edge2host --> host48.pppg$i++;
        host_WebServer49.pppg$o++ --> server2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> server2edge --> host_WebServer49.pppg$i++;
        host50.pppg$o++ --> host2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> edge2host --> host50.pppg$i++;
        host52.pppg$o++ --> host2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> edge2host --> host52.pppg$i++;
        host53.pppg$o++ --> host2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> edge2host --> host53.pppg$i++;
        host_MailServer64.pppg$o++ --> server2edge --> edge10.pppg$i++;
        edge10.pppg$o++ --> server2edge --> host_MailServer64.pppg$i++;
        host_Historian6.pppg$o++ --> host2edge --> edge10.pppg$i++;
        edge10.pppg$o++ --> edge2host --> host_Historian6.pppg$i++;
}

module Remote
{
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
                @display("i=abstract/switch;p=407,206");
        }
        gw1: Router {
            parameters:
                @RL();
                @display("i=abstract/router2;p=245,307");
        }
        edge5: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        edge6: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        host26: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop;p=284,495");
        }
        host29: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop;p=466,445");
        }
        host30: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop;p=284,415");
        }
        host32: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop;p=446,320");
        }
        host33: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop;p=60,480");
        }
        host34: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host35: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host36: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop;p=124,403");
        }
        host38: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop;p=163,460");
        }
        host39: InetUserHost {
            parameters:
                @RL();
                @display("i=device/laptop;p=60,421");
        }
    connections:
        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;

        core0.pppg++ <--> gateway2core <--> gw1.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge5.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge6.pppg++;
        host26.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host26.pppg$i++;
        host29.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host29.pppg$i++;
        host30.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host30.pppg$i++;
        host32.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host32.pppg$i++;
        host33.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host33.pppg$i++;
        host34.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host34.pppg$i++;
        host35.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host35.pppg$i++;
        host36.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host36.pppg$i++;
        host38.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host38.pppg$i++;
        host39.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host39.pppg$i++;
}

module Field0
{
    @display("bgb=717,616");
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
                @display("i=abstract/switch;p=378,161");
        }
        gw1: Router {
            parameters:
                @RL();
                @display("i=abstract/router2;p=434,316");
        }
        edge2: Router {
            parameters:
                @RL();
                @display("i=abstract/router;p=438,253");
        }
        edge3: Router {
            parameters:
                @RL();
                @display("i=abstract/router;p=534,343");
        }
        edge4: Router {
            parameters:
                @RL();
                @display("i=abstract/router;p=334,472");
        }
        edge5: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        edge6: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        edge7: Router {
            parameters:
                @RL();
                @display("i=abstract/router;p=340,316");
        }
        host8: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host9: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host_WebServer10: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host11: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host12: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host13: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host16: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=582,419");
        }
        host17: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=628,384");
        }
        host_WebServer19: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=554,472");
        }
        host20: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=628,316");
        }
        host21: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=606,281");
        }
        host22: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host23: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host24: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host26: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=497,550");
        }
        host27: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host28: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host29: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host_WebServer30: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=193,540");
        }
        host32: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host34: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host_InteractiveServer35: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host36: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host38: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host39: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=248,85");
        }
        host_MailServer40: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=114,95");
        }
        host41: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=114,161");
        }
        host42: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=107,235");
        }
        host43: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=176,114");
        }
        host44: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host45: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host46: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host47: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host_WebServer48: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=172,343");
        }
        host49: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host51: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host52: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }

    connections:
        pppg++ <--> core0.pppg++;
        //pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;

        core0.pppg++ <--> gateway2core <--> gw1.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge2.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge3.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge4.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge5.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge6.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge7.pppg++;
        host8.pppg$o++ --> host2edge --> edge2.pppg$i++;
        edge2.pppg$o++ --> edge2host --> host8.pppg$i++;
        host9.pppg$o++ --> host2edge --> edge2.pppg$i++;
        edge2.pppg$o++ --> edge2host --> host9.pppg$i++;
        host_WebServer10.pppg$o++ --> server2edge --> edge2.pppg$i++;
        edge2.pppg$o++ --> server2edge --> host_WebServer10.pppg$i++;
        host11.pppg$o++ --> host2edge --> edge2.pppg$i++;
        edge2.pppg$o++ --> edge2host --> host11.pppg$i++;
        host12.pppg$o++ --> host2edge --> edge2.pppg$i++;
        edge2.pppg$o++ --> edge2host --> host12.pppg$i++;
        host13.pppg$o++ --> host2edge --> edge2.pppg$i++;
        edge2.pppg$o++ --> edge2host --> host13.pppg$i++;
        host16.pppg$o++ --> host2edge --> edge3.pppg$i++;
        edge3.pppg$o++ --> edge2host --> host16.pppg$i++;
        host17.pppg$o++ --> host2edge --> edge3.pppg$i++;
        edge3.pppg$o++ --> edge2host --> host17.pppg$i++;
        host_WebServer19.pppg$o++ --> host2edge --> edge3.pppg$i++;
        edge3.pppg$o++ --> edge2host --> host_WebServer19.pppg$i++;
        host20.pppg$o++ --> host2edge --> edge3.pppg$i++;
        edge3.pppg$o++ --> edge2host --> host20.pppg$i++;
        host21.pppg$o++ --> host2edge --> edge3.pppg$i++;
        edge3.pppg$o++ --> edge2host --> host21.pppg$i++;
        host22.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host22.pppg$i++;
        host23.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host23.pppg$i++;
        host24.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host24.pppg$i++;
        host26.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host26.pppg$i++;
        host27.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host27.pppg$i++;
        host28.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host28.pppg$i++;
        host29.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host29.pppg$i++;
        host_WebServer30.pppg$o++ --> server2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> server2edge --> host_WebServer30.pppg$i++;
        host32.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host32.pppg$i++;
        host34.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host34.pppg$i++;
        host_InteractiveServer35.pppg$o++ --> server2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> server2edge --> host_InteractiveServer35.pppg$i++;
        host36.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host36.pppg$i++;
        host38.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host38.pppg$i++;
        host39.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host39.pppg$i++;
        host_MailServer40.pppg$o++ --> server2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> server2edge --> host_MailServer40.pppg$i++;
        host41.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host41.pppg$i++;
        host42.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host42.pppg$i++;
        host43.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host43.pppg$i++;
        host44.pppg$o++ --> host2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> edge2host --> host44.pppg$i++;
        host45.pppg$o++ --> host2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> edge2host --> host45.pppg$i++;
        host46.pppg$o++ --> host2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> edge2host --> host46.pppg$i++;
        host47.pppg$o++ --> host2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> edge2host --> host47.pppg$i++;
        host_WebServer48.pppg$o++ --> server2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> server2edge --> host_WebServer48.pppg$i++;
        host49.pppg$o++ --> host2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> edge2host --> host49.pppg$i++;
        host51.pppg$o++ --> host2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> edge2host --> host51.pppg$i++;
        host52.pppg$o++ --> host2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> edge2host --> host52.pppg$i++;
}

module Field1
{
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
                @display("i=abstract/switch");
        }
        gw1: Router {
            parameters:
                @RL();
                @display("i=abstract/router2");
        }
        gw2: Router {
            parameters:
                @RL();
                @display("i=abstract/router2");
        }
        edge3: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        edge4: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        edge5: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        edge6: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        edge7: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        edge8: Router {
            parameters:
                @RL();
                @display("i=abstract/router;p=542,390");
        }
        edge9: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        edge10: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        edge11: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        edge12: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        edge13: Router {
            parameters:
                @RL();
                @display("i=abstract/router");
        }
        host14: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host15: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host16: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host19: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host20: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host_MailServer21: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=37,232");
        }
        host22: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host23: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host24: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host25: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host26: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=76,183");
        }
        host27: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=181,299");
        }
        host28: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host_InteractiveServer29: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host30: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host31: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host32: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host34: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host36: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host_WebServer38: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=169,535");
        }
        host39: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=160,403");
        }
        host40: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=332,551");
        }
        host42: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=248,544");
        }
        host43: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=181,463");
        }
        host_WebServer44: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=876,454");
        }
        host45: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host46: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=554,551");
        }
        host47: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=653,390");
        }
        host48: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host49: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=692,565");
        }
        host50: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=706,457");
        }
        host51: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=627,463");
        }
        host_WebServer53: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=199,65");
        }
        host54: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=415,38");
        }
        host55: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=217,135");
        }
        host56: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=535,100");
        }
        host58: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=144,89");
        }
        host59: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=1132,262");
        }
        host60: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host61: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host63: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host64: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host65: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host66: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host67: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=1223,90");
        }
        host68: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host69: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host70: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=892,124");
        }
        host71: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host74: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host75: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host76: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host77: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host78: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host79: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host80: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=1108,484");
        }
        host_WebServer81: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=1181,414");
        }
        host82: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }
        host83: PLC {
            parameters:
                @RL();
                @display("i=device/laptop;p=1027,484");
        }
        host84: PLC {
            parameters:
                @RL();
                @display("i=device/laptop");
        }

    connections:
        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;
//        pppg++ <--> core0.pppg++;

        core0.pppg++ <--> gateway2core <--> gw1.pppg++;
        core0.pppg++ <--> gateway2core <--> gw2.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge3.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge4.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge5.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge6.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge7.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge8.pppg++;
        gw1.pppg++ <--> edge2gateway <--> edge9.pppg++;
        gw2.pppg++ <--> edge2gateway <--> edge10.pppg++;
        gw2.pppg++ <--> edge2gateway <--> edge11.pppg++;
        gw2.pppg++ <--> edge2gateway <--> edge12.pppg++;
        gw2.pppg++ <--> edge2gateway <--> edge13.pppg++;
        host14.pppg$o++ --> host2edge --> edge3.pppg$i++;
        edge3.pppg$o++ --> edge2host --> host14.pppg$i++;
        host15.pppg$o++ --> host2edge --> edge3.pppg$i++;
        edge3.pppg$o++ --> edge2host --> host15.pppg$i++;
        host16.pppg$o++ --> host2edge --> edge3.pppg$i++;
        edge3.pppg$o++ --> edge2host --> host16.pppg$i++;
        host19.pppg$o++ --> host2edge --> edge3.pppg$i++;
        edge3.pppg$o++ --> edge2host --> host19.pppg$i++;
        host20.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host20.pppg$i++;
        host_MailServer21.pppg$o++ --> server2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> server2edge --> host_MailServer21.pppg$i++;
        host22.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host22.pppg$i++;
        host23.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host23.pppg$i++;
        host24.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host24.pppg$i++;
        host25.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host25.pppg$i++;
        host26.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host26.pppg$i++;
        host27.pppg$o++ --> host2edge --> edge4.pppg$i++;
        edge4.pppg$o++ --> edge2host --> host27.pppg$i++;
        host28.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host28.pppg$i++;
        host_InteractiveServer29.pppg$o++ --> server2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> server2edge --> host_InteractiveServer29.pppg$i++;
        host30.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host30.pppg$i++;
        host31.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host31.pppg$i++;
        host32.pppg$o++ --> host2edge --> edge5.pppg$i++;
        edge5.pppg$o++ --> edge2host --> host32.pppg$i++;
        host34.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host34.pppg$i++;
        host36.pppg$o++ --> host2edge --> edge6.pppg$i++;
        edge6.pppg$o++ --> edge2host --> host36.pppg$i++;
        host_WebServer38.pppg$o++ --> server2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> server2edge --> host_WebServer38.pppg$i++;
        host39.pppg$o++ --> host2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> edge2host --> host39.pppg$i++;
        host40.pppg$o++ --> host2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> edge2host --> host40.pppg$i++;
        host42.pppg$o++ --> host2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> edge2host --> host42.pppg$i++;
        host43.pppg$o++ --> host2edge --> edge7.pppg$i++;
        edge7.pppg$o++ --> edge2host --> host43.pppg$i++;
        host_WebServer44.pppg$o++ --> server2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> server2edge --> host_WebServer44.pppg$i++;
        host45.pppg$o++ --> host2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> edge2host --> host45.pppg$i++;
        host46.pppg$o++ --> host2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> edge2host --> host46.pppg$i++;
        host47.pppg$o++ --> host2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> edge2host --> host47.pppg$i++;
        host48.pppg$o++ --> host2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> edge2host --> host48.pppg$i++;
        host49.pppg$o++ --> host2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> edge2host --> host49.pppg$i++;
        host50.pppg$o++ --> host2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> edge2host --> host50.pppg$i++;
        host51.pppg$o++ --> host2edge --> edge8.pppg$i++;
        edge8.pppg$o++ --> edge2host --> host51.pppg$i++;
        host_WebServer53.pppg$o++ --> server2edge --> edge9.pppg$i++;
        edge9.pppg$o++ --> server2edge --> host_WebServer53.pppg$i++;
        host54.pppg$o++ --> host2edge --> edge9.pppg$i++;
        edge9.pppg$o++ --> edge2host --> host54.pppg$i++;
        host55.pppg$o++ --> host2edge --> edge9.pppg$i++;
        edge9.pppg$o++ --> edge2host --> host55.pppg$i++;
        host56.pppg$o++ --> host2edge --> edge9.pppg$i++;
        edge9.pppg$o++ --> edge2host --> host56.pppg$i++;
        host58.pppg$o++ --> host2edge --> edge9.pppg$i++;
        edge9.pppg$o++ --> edge2host --> host58.pppg$i++;
        host59.pppg$o++ --> host2edge --> edge10.pppg$i++;
        edge10.pppg$o++ --> edge2host --> host59.pppg$i++;
        host60.pppg$o++ --> host2edge --> edge10.pppg$i++;
        edge10.pppg$o++ --> edge2host --> host60.pppg$i++;
        host61.pppg$o++ --> host2edge --> edge10.pppg$i++;
        edge10.pppg$o++ --> edge2host --> host61.pppg$i++;
        host63.pppg$o++ --> host2edge --> edge10.pppg$i++;
        edge10.pppg$o++ --> edge2host --> host63.pppg$i++;
        host64.pppg$o++ --> host2edge --> edge10.pppg$i++;
        edge10.pppg$o++ --> edge2host --> host64.pppg$i++;
        host65.pppg$o++ --> host2edge --> edge10.pppg$i++;
        edge10.pppg$o++ --> edge2host --> host65.pppg$i++;
        host66.pppg$o++ --> host2edge --> edge10.pppg$i++;
        edge10.pppg$o++ --> edge2host --> host66.pppg$i++;
        host67.pppg$o++ --> host2edge --> edge10.pppg$i++;
        edge10.pppg$o++ --> edge2host --> host67.pppg$i++;
        host68.pppg$o++ --> host2edge --> edge10.pppg$i++;
        edge10.pppg$o++ --> edge2host --> host68.pppg$i++;
        host69.pppg$o++ --> host2edge --> edge11.pppg$i++;
        edge11.pppg$o++ --> edge2host --> host69.pppg$i++;
        host70.pppg$o++ --> host2edge --> edge11.pppg$i++;
        edge11.pppg$o++ --> edge2host --> host70.pppg$i++;
        host71.pppg$o++ --> host2edge --> edge11.pppg$i++;
        edge11.pppg$o++ --> edge2host --> host71.pppg$i++;
        host74.pppg$o++ --> host2edge --> edge12.pppg$i++;
        edge12.pppg$o++ --> edge2host --> host74.pppg$i++;
        host75.pppg$o++ --> host2edge --> edge12.pppg$i++;
        edge12.pppg$o++ --> edge2host --> host75.pppg$i++;
        host76.pppg$o++ --> host2edge --> edge12.pppg$i++;
        edge12.pppg$o++ --> edge2host --> host76.pppg$i++;
        host77.pppg$o++ --> host2edge --> edge12.pppg$i++;
        edge12.pppg$o++ --> edge2host --> host77.pppg$i++;
        host78.pppg$o++ --> host2edge --> edge12.pppg$i++;
        edge12.pppg$o++ --> edge2host --> host78.pppg$i++;
        host79.pppg$o++ --> host2edge --> edge13.pppg$i++;
        edge13.pppg$o++ --> edge2host --> host79.pppg$i++;
        host80.pppg$o++ --> host2edge --> edge13.pppg$i++;
        edge13.pppg$o++ --> edge2host --> host80.pppg$i++;
        host_WebServer81.pppg$o++ --> server2edge --> edge13.pppg$i++;
        edge13.pppg$o++ --> server2edge --> host_WebServer81.pppg$i++;
        host82.pppg$o++ --> host2edge --> edge13.pppg$i++;
        edge13.pppg$o++ --> edge2host --> host82.pppg$i++;
        host83.pppg$o++ --> host2edge --> edge13.pppg$i++;
        edge13.pppg$o++ --> edge2host --> host83.pppg$i++;
        host84.pppg$o++ --> host2edge --> edge13.pppg$i++;
        edge13.pppg$o++ --> edge2host --> host84.pppg$i++;
}


