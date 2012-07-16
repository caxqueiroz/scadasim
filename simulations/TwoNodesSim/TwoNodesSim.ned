//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package TwoNodesSim;

import scadasim.base.ConnectionManager;
import scadasim.applications.scada.SCADATrafficProfileManager;
import scadasim.base.TrafficProfileManager;
import scadasim.nodes.inet.InteractiveServer;
import scadasim.nodes.inet.DatabaseServer;
import scadasim.nodes.scada.Historian;
import scadasim.nodes.scada.MTU;
import scadasim.nodes.scada.RTU;
import scadasim.nodes.scada.PLC;
import scadasim.nodes.scada.IED;
import scadasim.nodes.inet.WebServer;
import scadasim.networklayer.autorouting.TGMNetworkConfigurator;
import inet.networklayer.autorouting.FlatNetworkConfigurator;
import scadasim.nodes.inet.InetUserHost;
import scadasim.nodes.scada.RemoteUserHost;
import inet.nodes.inet.Router;

channel edge2gateway extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 1000Mbps;
}

channel host2edge extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 1000Mbps;
}

channel host2gateway extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 10Mbps;
}

channel edge2edge extends ned.DatarateChannel
{
    parameters:
        delay = 1us;
        datarate = 0.768Mbps;
}

//
// SCADA System
//
network TwoNodesSim
{
    @display("bgb=858,655,white");
    submodules:
        flatNetworkConfigurator: FlatNetworkConfigurator {
            @display("p=554,99;i=abstract/table_s");

        }
        connectionManager: ConnectionManager {
            @display("p=705,164;i=block/cogwheel;is=s");
        }

        Internet: Router {
            @display("i=misc/cloud;p=445,127;is=s");
            @AS();
        }
        brf00: Router {
            @display("p=288,164;is=vs");
            @AS();
        }
        f00s01: Router {
            @display("p=122,247;i=abstract/switch;is=vs");
        }
        brc: Router {
            @display("p=601,298;is=vs");
            @AS();
        }
        cs02: Router {
            @display("p=686,380;i=abstract/switch;is=vs");
        }
        f00p01: PLC {
            @display("p=77,374;i=device/device;is=vs");
        }
        css01: MTU {
            @display("p=785,496;i=device/pc3;is=vs");
        }
        trafficProfileManager: TrafficProfileManager {
            @display("p=396,247;i=block/network2;is=s");
        }
    //        scadaTrafficProfileManager: SCADATrafficProfileManager {
    //            @display("p=645,12;is=vs;i=abstract/table");
    //        }
    connections:
        Internet.pppg++ <--> edge2edge <--> brf00.pppg++;
        Internet.pppg++ <--> edge2edge <--> brc.pppg++;
        brf00.pppg++ <--> edge2gateway <--> f00s01.pppg++;
        f00s01.pppg++ <--> host2gateway <--> f00p01.pppg++;
        brc.pppg++ <--> edge2gateway <--> cs02.pppg++;
        cs02.pppg++ <--> host2gateway <--> css01.pppg++;
}
