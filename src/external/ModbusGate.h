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
/*
 * ModbusGate.h
 *
 *  Created on: 09/05/2011
 *      Author: cax
 */

#ifndef MODBUSGATE_H_
#define MODBUSGATE_H_
#include "modbus.h"
#include "SSGate.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <pthread.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <termios.h>

class ModbusGate: public SSGate {
	
private:
	int socket_conn;
	modbus_param_t mb_param;
	modbus_mapping_t mb_mapping;

	void listen();


protected:
	virtual void execute(void * arg_);
	
public:
	ModbusGate();
	virtual ~ModbusGate();
	virtual void processMessage(ssMsg *msg);
	virtual void Open();
	virtual void Close();

};

#endif /* MODBUSGATE_H_ */
