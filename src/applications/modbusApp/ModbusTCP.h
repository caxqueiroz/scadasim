// ***************************************************************************
//
// SCADASim Project
//
// This file is a part of the SCADASim project. The project was created at RMIT University,
// Distributed Systems and Networking Discipline (DSN)
// Its purpose is to create a set of OMNeT++ components to simulate SCADA systems
// in a high-fidelity manner along with a highly configurable  SCADA system and network.
//
// Maintainer: Carlos Alexandre Queiroz cqueiroz@cs.rmit.edu.au
// Project home page: http://www.cs.rmit.edu.au/~cqueiroz/scadasim

// ***************************************************************************

/*
 * Copyright Carlos Alexandre Queiroz 2009
 * Adapted from https://launchpad.net/libmodbus Stephane Raimbault <stephane.raimbault@gmail.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser Public License for more details.
 *
 * You should have received a copy of the GNU Lesser Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef MODBUS_H_
#define MODBUS_H_

#include <stdint.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <termios.h>
#include <sys/time.h>
#include <unistd.h>
#include <errno.h>
#include <limits.h>
#include <fcntl.h>

#include "INETDefs.h"
#include "Constants.h"
#include "DataStruct.h"

class ModbusTCP
{

	private:

		error_handling_t error_handling;
		modbus_mapping_t * mb_mapping;

	public:

		ModbusTCP();
		~ModbusTCP();

		unsigned int computeResponseLength(uint8_t *query);

		/* All functions used for sending or receiving data return:
		 - the numbers of values (bits or word) if success (0 or more)
		 - less than 0 for exceptions errors
		 */
		void errorTreat(int code, const char *string);

		/* Builds a TCP query header */
		int buildQueryBasis(int slave, int function, int start_addr, int nb, uint8_t *query);

		int buildResponseBasis(sft_t *sft, uint8_t *response);

		/* Sets the length of TCP message in the message (query and response) */
		void setMessageLength(uint8_t *msg, int msg_length);

		/* Sends a query/response over a serial or a TCP communication */
		//int send_msg(ModbusEP * mbEP, uint8_t *query, int query_length);

		/* Computes the length of the header following the function code */
		uint8_t computeQueryLengthHeader(int function);

		/* Computes the length of the data to write in the query */
		int computeQueryLengthData(uint8_t *msg);

		int receive(uint8_t *query, uint8_t *response);

		int responseIOStatusU8(int address, int nb, tables tb, uint8_t *response,	int offset);

		int receiveMessage(uint8_t *query, uint8_t *response, int ret);

		/* Build the exception response */
		int responseException(sft_t *sft, int exception_code, uint8_t *response);

		/* By default, the error handling mode used is RECONNECT_ON_ERROR.

		 With RECONNECT_ON_ERROR, the library will attempt an immediate
		 reconnection which may hang for several seconds if the network to
		 the remote target unit is down.

		 With NOP_ON_ERROR, it is expected that the application will
		 check for network error returns and deal with them as necessary.

		 This function is only useful in TCP mode.
		 */
		void setErrorHandling(error_handling_t error_handling);

		/**
		 * SLAVE/CLIENT FUNCTIONS
		 **/

		/* Allocates 4 arrays to store coils, input status, input registers and
		 holding registers. The pointers are stored in modbus_mapping structure.

		 Returns: true if ok, false on failure
		 */

		void initMemory();
		bool memoryAllocation(int nb_coil_status, int nb_input_status, int nb_holding_registers,
				int nb_input_registers);

		/* Frees the 4 arrays */
		void freeAllocatedMemory();

		//getters and setters
		int getNBCoilStatus();

		int getNBInputStatus();
		int getNBInputRegisters();
		int getNBHoldingRegisters();
		uint8_t getTabCoilStatus(int i);
		void setTabCoilStatus(int p, uint8_t v);

		uint8_t getTabInputStatus(int i);
		void setTabInputStatus(int p, uint8_t v);

		uint16_t getTabInputRegisters(int i);
		void setTabInputRegisters(int p, uint16_t v);

		uint16_t getTabHoldingRegisters(int i);
		void setTabHoldingRegisters(int p, uint16_t v);

		/**
		 * Set tables based on the function.
		 */
		void setTable(int fc, int p, uint16_t v);


		/* Manages the received query.
		 Analyses the query and constructs a response.*/

		/**
		 * UTILS FUNCTIONS
		 **/

		/* Sets many input/coil status from a single byte value (all 8 bits of
		 the byte value are setted) */
		void setBitsFromByte(uint8_t *dest, int address, const uint8_t value);

		/* Sets many input/coil status from a table of bytes (only the bits
		 between address and address + nb_bits are setted) */
		void setBitsFromBytesU8(int address, int nb_bits, const uint8_t *tab_byte, tables tab_id);

		/* Gets the byte value from many input/coil status.
		 To obtain a full byte, set nb_bits to 8. */
		uint8_t getByteFromBits(const uint8_t *src, int address, int nb_bits);

};

#endif /* MODBUS_H_ */
