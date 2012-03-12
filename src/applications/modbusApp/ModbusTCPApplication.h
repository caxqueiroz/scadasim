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

#ifndef __SCADASIM_MODBUSAPPLICATION_H_
#define __SCADASIM_MODBUSAPPLICATION_H_

#include <omnetpp.h>
#include "SCADASIMDefs.h"
#include "ModbusUser.h"
#include "GenericTCPApplication.h"
#include "ModbusTCP.h"
/**
 * Modbus application.
 */
class SCADASIM_API ModbusTCPApplication : public GenericTCPApplication
{

  protected:
    ModbusTCP modbus;
    ModbusUser *mbu;
    void initialize(int stages);
    void handleMessage(cMessage *msg);
    void transmissionDone(TransmissionStatistics s);

  public:
    ModbusTCPApplication();
    virtual ~ModbusTCPApplication();
    void transmissionStart(TrafficProfile &p, TargetInfo &i);
    ModbusTCP getModbusTCPStack();

};

#endif
