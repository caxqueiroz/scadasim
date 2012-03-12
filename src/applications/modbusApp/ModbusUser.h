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

#ifndef __SCADASIM_MODBUSUSER_H_
#define __SCADASIM_MODBUSUSER_H_

#include <omnetpp.h>
#include "SCADASIMDefs.h"
#include "InetUser.h"


/**
 * Handles modbus protocol communication.
 */
class SCADASIM_API ModbusUser : public InetUser
{
public:
    void transmissionDone(TransmissionStatistics t);
    void setApplication(int applicationType, GenericApplication *a, int attachedProfileNumber);
  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);
    virtual void finish();
    virtual void transmissionDone();

};

/**
 * @brief Gives access to the ModbusUser instance within the host (client/server).
 *
 * @class ModbusUserAccess
 */
class SCADASIM_API ModbusUserAccess : public ModuleAccess<ModbusUser>
{
  public:
    ModbusUserAccess() : ModuleAccess<ModbusUser>("modbusUser"){}
};

#endif
