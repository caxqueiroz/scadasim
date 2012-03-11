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

package InfoDivSimulation;

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
network CaseStudy
{
    @display("bgb=858,655,white");
    submodules:
        flatNetworkConfigurator: FlatNetworkConfigurator {
            @display("p=739,8;i=abstract/table_s");

        }
        connectionManager: ConnectionManager {
            @display("p=759,8;i=block/cogwheel;is=s");
        }

        brf01: Router {
            @display("p=324,213;i=systems/router;is=vs");
            @AS();
        }
        Internet: Router {
            @display("i=systems/cloud;p=438,266;is=s");
            @AS();
        }
        brf00: Router {
            @display("p=411,367;i=systems/router;is=vs");
            @AS();
        }
        brf02: Router {
            @display("p=590,252;i=systems/router;is=vs");
            @AS();
        }
        f00s01: Router {
            @display("p=292,367;i=systems/switch;is=vs");
        }
        brc: Router {
            @display("p=611,367;i=systems/router;is=vs");
            @AS();
        }
        f01s02: Router {
            @display("p=181,184;i=systems/switch;is=vs");
        }
        f02s01: Router {
            @display("p=657,213;i=systems/switch;is=vs");
        }
        f00s02: Router {
            @display("p=324,436;i=systems/switch;is=vs");
        }
        rc04: InetUserHost {
            @display("p=549,171;i=systems/laptop;is=vs");
        }
        f01p10: PLC {
            @display("p=233,287;i=systems/rtu;is=vs");
        }
        crs01: InteractiveServer {
            @display("p=810,383;i=systems/server;is=vs");
        }
        f01p04: PLC {
            @display("p=65,115;i=systems/rtu;is=vs");
        }
        cs02: Router {
            @display("p=642,489;i=systems/switch;is=vs");
        }
        chs01: WebServer {
            @display("p=577,545;i=systems/web_server;is=vs");
        }
        f02r03: RTU {
            @display("p=758,199;i=systems/plc;is=vs");
        }
        f01p05: PLC {
            @display("p=71,156;i=systems/rtu;is=vs");
        }
        cs03: Router {
            @display("p=505,545;i=systems/switch;is=vs");
        }
        c02: InetUserHost {
            @display("p=438,541;i=systems/pc;is=vs");
        }
        f01p11: PLC {
            @display("p=277,266;i=systems/rtu;is=vs");
        }
        f01s01: Router {
            @display("p=347,128;i=systems/switch;is=vs");
        }
        f01p15: PLC {
            @display("p=439,87;i=systems/rtu;is=vs");
        }
        f00r02: RTU {
            @display("p=233,463;i=systems/plc;is=vs");
        }
        f02sr03: IED {
            @display("p=808,184;i=systems/ied;is=vs");
        }
        f02r02: RTU {
            @display("p=708,127;i=systems/plc;is=vs");
        }
        f00r01: RTU {
            @display("p=183,453;i=systems/plc;is=vs");
        }
        f00r06: RTU {
            @display("p=274,540;i=systems/plc;is=vs");
        }
        f00sr01: RTU {
            @display("p=109,488;i=systems/ied;is=vs");
        }
        f00r03: RTU {
            @display("p=339,513;i=systems/plc;is=vs");
        }
        f02sr02: IED {
            @display("p=723,87;i=systems/ied;is=vs");
        }
        c04: InetUserHost {
            @display("p=562,606;i=systems/pc;is=vs");
        }
        f02r01: RTU {
            @display("p=686,297;i=systems/plc;is=vs");
        }
        rc03: InetUserHost {
            @display("p=521,138;i=systems/laptop;is=vs");
        }
        f02p04: PLC {
            @display("p=645,124;i=systems/rtu;is=vs");
        }
        ch01: Historian {
            @display("p=760,508;i=systems/historian;is=vs");
        }
        rc01: InetUserHost {
            @display("p=426,157;i=systems/laptop;is=vs");
        }
        f00p03: PLC {
            @display("p=124,432;i=systems/rtu;is=vs");
        }
        f01p01: PLC {
            @display("p=209,107;i=systems/rtu;is=vs");
        }
        f00sr03: IED {
            @display("p=340,573;i=systems/ied;is=vs");
        }
        f02sr01: IED {
            @display("p=761,320;i=systems/ied;is=vs");
        }
        rc02: InetUserHost {
            @display("p=479,138;i=systems/laptop;is=vs");
        }
        f02p02: PLC {
            @display("p=776,127;i=systems/rtu;is=vs");
        }
        f00sr02: IED {
            @display("p=165,513;i=systems/ied;is=vs");
        }
        f01p14: PLC {
            @display("p=370,65;i=systems/rtu;is=vs");
        }
        f01p12: PLC {
            @display("p=277,156;i=systems/rtu;is=vs");
        }
        f01p09: PLC {
            @display("p=181,276;i=systems/rtu;is=vs");
        }
        f01p02: PLC {
            @display("p=165,65;i=systems/rtu;is=vs");
        }
        c01: InetUserHost {
            @display("p=452,606;i=systems/pc;is=vs");
        }
        f00p05: PLC {
            @display("p=396,471;i=systems/rtu;is=vs");
        }
        f00p01: PLC {
            @display("p=183,342;i=systems/rtu;is=vs");
        }
        css02: MTU {
            @display("p=532,488;i=systems/server;is=vs");
        }
        f01p13: PLC {
            @display("p=293,76;i=systems/rtu;is=vs");
        }
        f00p04: PLC {
            @display("p=218,536;i=systems/plc;is=vs");
        }
        f00sr06: IED {
            @display("p=244,605;i=systems/ied;is=vs");
        }
        css01: MTU {
            @display("p=577,446;i=systems/server;is=vs");
        }
        css03: MTU {
            @display("p=657,559;i=systems/server;is=vs");
        }
        brr: Router {
            @display("p=479,197;is=vs;i=systems/router");
            @AS();
        }
        f01p06: PLC {
            @display("p=71,199;i=systems/rtu;is=vs");
        }
        f01p07: PLC {
            @display("p=65,265;i=systems/rtu;is=vs");
        }
        f00p02: PLC {
            @display("p=137,377;i=systems/rtu;is=vs");
        }
        f01p03: PLC {
            @display("p=109,81;i=systems/rtu;is=vs");
        }
        f01p08: PLC {
            @display("p=118,266;i=systems/rtu;is=vs");
        }
        f02p01: PLC {
            @display("p=761,252;i=systems/rtu;is=vs");
        }
        c03: InetUserHost {
            @display("p=505,620;i=systems/pc;is=vs");
        }
        cdb01: DatabaseServer {
            @display("p=717,554;i=systems/database;is=vs");
        }
        crs02: InteractiveServer {
            @display("p=810,446;i=systems/server;is=vs");
        }
        cs01: Router {
            @display("p=738,407;i=systems/switch;is=vs");
        }


        trafficProfileManager: TrafficProfileManager {
            @display("p=801,8;i=block/network2;is=s");
        }
        scadaTrafficProfileManager: SCADATrafficProfileManager {
            @display("p=645,12;is=vs;i=abstract/table");
        }
    connections:
        f01s02.pppg++ <--> host2gateway <--> f01p09.pppg++;
        brf01.pppg++ <--> edge2edge <--> Internet.pppg++;
        f01s02.pppg++ <--> host2gateway <--> f01p11.pppg++;
        brf01.pppg++ <--> edge2gateway <--> f01s02.pppg++;
        f01s02.pppg++ <--> host2gateway <--> f01p07.pppg++;
        brf01.pppg++ <--> edge2gateway <--> f01s01.pppg++;
        f01s02.pppg++ <--> host2gateway <--> f01p10.pppg++;
        Internet.pppg++ <--> edge2edge <--> brf00.pppg++;
        Internet.pppg++ <--> edge2edge <--> brf02.pppg++;
        Internet.pppg++ <--> edge2edge <--> brc.pppg++;
        brf00.pppg++ <--> edge2gateway <--> f00s01.pppg++;
        brf00.pppg++ <--> edge2gateway <--> f00s02.pppg++;
        cs01.pppg++ <--> edge2gateway <--> brc.pppg++;
        brf02.pppg++ <--> edge2gateway <--> f02s01.pppg++;
        f00s01.pppg++ <--> host2gateway <--> f00p01.pppg++;
        f00s01.pppg++ <--> host2gateway <--> f00r02.pppg++;
        f00s01.pppg++ <--> host2gateway <--> f00r01.pppg++;
        cs01.pppg++ <--> host2gateway <--> crs01.pppg++;
        brc.pppg++ <--> edge2gateway <--> cs02.pppg++;
        f01s02.pppg++ <--> host2gateway <--> f01p02.pppg++;
        f01s02.pppg++ <--> host2gateway <--> f01p05.pppg++;
        f01s02.pppg++ <--> host2gateway <--> f01p06.pppg++;
        f01s02.pppg++ <--> host2gateway <--> f01p01.pppg++;
        f01s02.pppg++ <--> host2gateway <--> f01p04.pppg++;
        f02s01.pppg++ <--> host2gateway <--> f02r01.pppg++;
        f02s01.pppg++ <--> host2gateway <--> f02r03.pppg++;
        f02s01.pppg++ <--> host2gateway <--> f02r02.pppg++;
        f00s02.pppg++ <--> host2gateway <--> f00p04.pppg++;
        f00s02.pppg++ <--> host2gateway <--> f00r06.pppg++;
        rc04.pppg++ <--> host2edge <--> brr.pppg++;
        cs01.pppg++ <--> host2gateway <--> cs02.pppg++;
        f01s02.pppg++ <--> host2gateway <--> f01p03.pppg++;
        cs02.pppg++ <--> host2gateway <--> ch01.pppg++;
        cs02.pppg++ <--> host2gateway <--> css02.pppg++;
        cs02.pppg++ <--> host2gateway <--> css03.pppg++;
        cs02.pppg++ <--> host2gateway <--> css01.pppg++;
        f02r03.pppg++ <--> host2gateway <--> f02sr03.pppg++;
        cs03.pppg++ <--> host2gateway <--> c04.pppg++;
        cs03.pppg++ <--> host2gateway <--> c02.pppg++;
        cs03.pppg++ <--> host2gateway <--> cs02.pppg++;
        f01s02.pppg++ <--> host2gateway <--> f01p08.pppg++;
        f01s01.pppg++ <--> host2gateway <--> f01p15.pppg++;
        f01s01.pppg++ <--> host2gateway <--> f01p14.pppg++;
        f00r02.pppg++ <--> host2gateway <--> f00sr02.pppg++;
        f02s01.pppg++ <--> host2gateway <--> f02p01.pppg++;
        f02s01.pppg++ <--> host2gateway <--> f02p04.pppg++;
        f02r02.pppg++ <--> host2gateway <--> f02sr02.pppg++;
        f00r01.pppg++ <--> host2gateway <--> f00sr01.pppg++;
        f00s02.pppg++ <--> host2gateway <--> f00p05.pppg++;
        f00r06.pppg++ <--> host2gateway <--> f00sr06.pppg++;
        f00s02.pppg++ <--> host2gateway <--> f00r03.pppg++;
        f00s01.pppg++ <--> host2gateway <--> f00p02.pppg++;
        f00s01.pppg++ <--> host2gateway <--> f00p03.pppg++;
        f00r03.pppg++ <--> host2gateway <--> f00sr03.pppg++;
        f02s01.pppg++ <--> host2gateway <--> f02p02.pppg++;
        cs03.pppg++ <--> host2gateway <--> c01.pppg++;
        cs03.pppg++ <--> host2gateway <--> c03.pppg++;
        f02r01.pppg++ <--> host2gateway <--> f02sr01.pppg++;
        rc03.pppg++ <--> host2edge <--> brr.pppg++;
        brr.pppg++ <--> host2edge <--> rc01.pppg++;
        brr.pppg++ <--> host2edge <--> rc02.pppg++;
        f01s01.pppg++ <--> host2gateway <--> f01p12.pppg++;
        f01s01.pppg++ <--> host2gateway <--> f01p13.pppg++;
        cs02.pppg++ <--> host2gateway <--> cdb01.pppg++;
        cs01.pppg++ <--> host2gateway <--> crs02.pppg++;
        chs01.pppg++ <--> host2gateway <--> cs02.pppg++;
        brr.pppg++ <--> edge2edge <--> Internet.pppg++;
}
