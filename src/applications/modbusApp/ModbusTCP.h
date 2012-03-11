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
 * Adapted from https://libmodbus.org Stephane Raimbault <stephane.raimbault@gmail.com>
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
//#include <errno.h>

#include "INETDefs.h"
#include "modbus.h"


#define MODBUS_TCP_DEFAULT_PORT 502

#define HEADER_LENGTH_RTU           0
#define PRESET_QUERY_LENGTH_RTU     6
#define PRESET_RESPONSE_LENGTH_RTU  2

#define HEADER_LENGTH_TCP           6
#define PRESET_QUERY_LENGTH_TCP    12
#define PRESET_RESPONSE_LENGTH_TCP  8

#define CHECKSUM_LENGTH_RTU      2
#define CHECKSUM_LENGTH_TCP      0

/* It's not really the minimal length (the real one is report slave ID
 * in RTU (4 bytes)) but it's a convenient size to use in RTU or TCP
 * communications to read many values or write a single one.
 * Maximum between :
 * - HEADER_LENGTH_TCP (6) + slave (1) + function (1) + address (2) +
 *   number (2)
 * - HEADER_LENGTH_RTU (0) + slave (1) + function (1) + address (2) +
 *   number (2) + CRC (2)
 */
#define MIN_QUERY_LENGTH        12

/* Page 102, Application Notes of PI–MBUS–300:
 *  The maximum length of the entire message must not exceed 256
 *  bytes.
 */
#define MAX_MESSAGE_LENGTH     256

#define MAX_STATUS             800
#define MAX_REGISTERS          100

#define REPORT_SLAVE_ID_LENGTH 75

/* Time out between trames in microsecond */
#define TIME_OUT_BEGIN_OF_TRAME 500000
#define TIME_OUT_END_OF_TRAME   500000

#ifndef FALSE
#define FALSE 0
#endif

#ifndef TRUE
#define TRUE 1
#endif


#ifndef OFF
#define OFF 0
#endif

#ifndef ON
#define ON 1
#endif

typedef enum
{
    COIL, INPUT, INPUT_REG, HOLDING_REG
} tables;



/* Function codes */
#define FC_READ_COIL_STATUS          0x01  /* discretes inputs */
#define FC_READ_INPUT_STATUS         0x02  /* discretes outputs */
#define FC_READ_HOLDING_REGISTERS    0x03
#define FC_READ_INPUT_REGISTERS      0x04
#define FC_FORCE_SINGLE_COIL         0x05
#define FC_PRESET_SINGLE_REGISTER    0x06
#define FC_READ_EXCEPTION_STATUS     0x07
#define FC_FORCE_MULTIPLE_COILS      0x0F
#define FC_PRESET_MULTIPLE_REGISTERS 0x10
#define FC_REPORT_SLAVE_ID           0x11

/* Protocol exceptions */
#define ILLEGAL_FUNCTION        -0x01
#define ILLEGAL_DATA_ADDRESS    -0x02
#define ILLEGAL_DATA_VALUE      -0x03
#define SLAVE_DEVICE_FAILURE    -0x04
#define SERVER_FAILURE          -0x04
#define ACKNOWLEDGE             -0x05
#define SLAVE_DEVICE_BUSY       -0x06
#define SERVER_BUSY             -0x06
#define NEGATIVE_ACKNOWLEDGE    -0x07
#define MEMORY_PARITY_ERROR     -0x08
#define GATEWAY_PROBLEM_PATH    -0x0A
#define GATEWAY_PROBLEM_TARGET  -0x0B

/* Local */
#define COMM_TIME_OUT           -0x0C
#define PORT_SOCKET_FAILURE     -0x0D
#define SELECT_FAILURE          -0x0E
#define TOO_MANY_DATA           -0x0F
#define INVALID_CRC             -0x10
#define INVALID_EXCEPTION_CODE  -0x11
#define CONNECTION_CLOSED       -0x12

/* Internal using */
#define MSG_LENGTH_UNDEFINED -1

#define UNKNOWN_ERROR_MSG "Not defined in modbus specification"

static const uint8_t NB_TAB_ERROR_MSG = 12;
static const char *TAB_ERROR_MSG[] =
{
/* 0x00 */UNKNOWN_ERROR_MSG,
/* 0x01 */"Illegal function code",
/* 0x02 */"Illegal data address",
/* 0x03 */"Illegal data value",
/* 0x04 */"Slave device or server failure",
/* 0x05 */"Acknowledge",
/* 0x06 */"Slave device or server busy",
/* 0x07 */"Negative acknowledge",
/* 0x08 */"Memory parity error",
/* 0x09 */UNKNOWN_ERROR_MSG,
/* 0x0A */"Gateway path unavailable",
/* 0x0B */"Target device failed to respond" };
//
///* Table of CRC values for high-order byte */
//static uint8_t table_crc_hi[] =
//{ 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
//      0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//      0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
//      0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00,
//      0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40,
//      0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80,
//      0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
//      0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00,
//      0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
//      0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//      0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
//      0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00,
//      0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
//      0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80,
//      0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
//      0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00,
//      0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40 };
//
///* Table of CRC values for low-order byte */
//static uint8_t table_crc_lo[] =
//{ 0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4, 0x04,
//      0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09, 0x08,
//      0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD, 0x1D,
//      0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3, 0x11,
//      0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7, 0x37,
//      0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A, 0x3B,
//      0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE, 0x2E,
//      0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26, 0x22,
//      0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2, 0x62,
//      0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F, 0x6E,
//      0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB, 0x7B,
//      0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5, 0x77,
//      0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91, 0x51,
//      0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C, 0x5D,
//      0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88, 0x48,
//      0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C, 0x44,
//      0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80, 0x40 };

const uint16_t _UT_COIL_STATUS_ADDRESS = 0x13;
const uint16_t UT_COIL_STATUS_NB_POINTS = 0x25;
const uint8_t UT_COIL_STATUS_TAB[] = { 0xCD, 0x6B, 0xB2, 0x0E, 0x1B };

const uint16_t UT_INPUT_STATUS_ADDRESS = 0xC4;
const uint16_t UT_INPUT_STATUS_NB_POINTS = 0x16;
const uint8_t UT_INPUT_STATUS_TAB[] = { 0xAC, 0xDB, 0x35 };

const uint16_t UT_HOLDING_REGISTERS_ADDRESS = 0x6B;
const uint16_t UT_HOLDING_REGISTERS_NB_POINTS = 0x3;
const uint16_t UT_HOLDING_REGISTERS_TAB[] = { 0x022B, 0x0000, 0x0064 };
/* If the following value is used, a bad response is sent.
 It's better to test with a lower value than
 UT_HOLDING_REGISTERS_NB_POINTS to try to raise a segfault. */
const uint16_t UT_HOLDING_REGISTERS_NB_POINTS_SPECIAL = 0x2;

const uint16_t UT_INPUT_REGISTERS_ADDRESS = 0x08;
const uint16_t UT_INPUT_REGISTERS_NB_POINTS = 0x1;
const uint16_t UT_INPUT_REGISTERS_TAB[] = { 0x000A };



const short START_SIM = 100;


/* This structure reduces the number of params in functions and so
 * optimizes the speed of execution (~ 37%). */
typedef struct
{
        int slave;
        int function;
        int t_id;
} sft_t;

typedef struct
{
        // statistics
        int numSessions;
        int numBroken;
        int packetsSent;
        int packetsRcvd;
        int bytesSent;
        int bytesRcvd;
} mb_stats_t;

typedef struct
{

        int ret;
        int slave;
        int function;
        int start_addr;
        int nb;
        bool sent;
        uint8_t * query;
        uint8_t * response;
        int response_length;
        int query_length;
        bool inTrans;
} mb_trans_t;


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
