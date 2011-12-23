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

package scadasim.applications.modbusApp;

simple ModbusTCP {
    
    parameters:
        //
        // should this application act as a server (true) or a client (false)
        //
        bool isServer;
        //
        // if it acts as a server, the server must know his traffic profile number.
        // If it acts as a client, the client can specify a profileNumber not equal -1
        // to register as a receiver for the specific profile
        //
        int profileNumber;
        //
        // and he has to know at which port he has to listen. Like above, a client
        // which set a profileNumber not equal to -1 has to specify a valid port
        // (for ICMP Ping the port is uninteresting)
        //
        int port;
        //
        // at least a udp or tcp server has to specify how many concurrently running
        // threads he will create. A Value of 0 say, that theres no limit for the
        // number of thread. (This value is ignored for the ICMP Application)
        //
        int noThreads;

    gates:
        input tcpIn;
        output tcpOut;
}
