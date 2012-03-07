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


#ifndef __DATASTRUCT_H
#define __DATASTRUCT_H

#include <termios.h>

typedef enum
{
	RTU, TCP
} type_com_t;
typedef enum
{
	FLUSH_OR_RECONNECT_ON_ERROR, NOP_ON_ERROR
} error_handling_t;

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
		int nb_coil_status;
		int nb_input_status;
		int nb_input_registers;
		int nb_holding_registers;
		uint8_t *tab_coil_status;
		uint8_t *tab_input_status;
		uint16_t *tab_input_registers;
		uint16_t *tab_holding_registers;
} modbus_mapping_t;

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

#endif

