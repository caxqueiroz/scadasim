/*
 * ModbusFunctionHandler.h
 *
 *  Created on: 14/03/2012
 *      Author: cax
 */

#ifndef MODBUSFUNCTIONHANDLER_H_
#define MODBUSFUNCTIONHANDLER_H_
#include <omnetpp.h>
#include "ModbusTCP.h"


class ModbusFunctionHandler : public cPolymorphic{


protected:
    ModbusTCP *modbus;

public:
    ModbusFunctionHandler();
    virtual ~ModbusFunctionHandler();
    virtual cMessage * createRandomMessage() =0;
    virtual void init() = 0;
    virtual void processMessage(cMessage * message) =0;
};

#endif /* MODBUSFUNCTIONHANDLER_H_ */
