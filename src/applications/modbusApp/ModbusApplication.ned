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

//
// ABSTRACT application
//
simple ModbusApplication
{
    parameters:
        //
        // should this application act as a slave (true) or a master (false)
        //
        bool isSlave;
        //
        // if it acts as a slave, the slave must know his traffic profile number.
        // If it acts as a master, the master can specify a profileNumber not equal -1
        // to register as a receiver for the specific profile
        //
        
        //
        // at least a udp or tcp server has to specify how many concurrently running
        // threads he will create. A Value of 0 say, that theres no limit for the
        // number of thread. (This value is ignored for the ICMP Application)
        //
        int noThreads;
}
