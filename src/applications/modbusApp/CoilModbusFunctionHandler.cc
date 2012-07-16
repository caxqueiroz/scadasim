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

#include "CoilModbusFunctionHandler.h"
#include "ModbusMessage_m.h"
#include <iostream>
#include <sstream>
using namespace std;
Register_Class(CoilModbusFunctionHandler);

CoilModbusFunctionHandler::CoilModbusFunctionHandler() {
    //base class handling this.
}

CoilModbusFunctionHandler::~CoilModbusFunctionHandler() {
    //base class handling this.
}

void CoilModbusFunctionHandler::init() {

}

cMessage * CoilModbusFunctionHandler::createRandomMessage() {
    ModbusMessage *msg = new ModbusMessage("coil-data");
    uint8_t query[MIN_QUERY_LENGTH];
    int slave = genk_intrand(0, 255);
    int nb = genk_intrand(0, 99);
    int start_addr = genk_intrand(0, 50);
    int len = modbus->buildQueryBasis(slave, FC_FORCE_SINGLE_COIL, start_addr,
            nb, query);

    msg->setByteLength(len);
    msg->setPduArraySize(len);
    for (int i = 0; i < len; i++) {
        msg->setPdu(i, query[i]);
    }
    return dynamic_cast<cMessage *>(msg);
}

void CoilModbusFunctionHandler::processMessage(cMessage * msg) {
    ModbusMessage *mbmsg = dynamic_cast<ModbusMessage *>(msg);
    int querySize = mbmsg->getPduArraySize();
    uint8_t query[querySize];
    int len = modbus->computeResponseLength(query);

    uint8_t response[len];
    modbus->receive(query, response);

    mbmsg->setPduArraySize(len);
    for (int i = 0; i < len; i++)
        mbmsg->setPdu(i, response[i]);
}
