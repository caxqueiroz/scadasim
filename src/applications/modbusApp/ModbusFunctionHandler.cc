/*
 * ModbusFunctionHandler.cpp
 *
 *  Created on: 14/03/2012
 *      Author: cax
 */

#include "ModbusFunctionHandler.h"

ModbusFunctionHandler::ModbusFunctionHandler() {
    modbus = new ModbusTCP();
    modbus->initMemory();

}

ModbusFunctionHandler::~ModbusFunctionHandler() {
    modbus->freeAllocatedMemory();
    delete modbus;
}

