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
 * This class was adapted from the libmodbus library (http://libmodbus.org/) in order to support INET/OMNET++.
 * The libmodbus is designed to send and receive data from a device that communicate via the Modbus protocol.
 */

#include "ModbusTCP.h"
//#include "modbus_m.h"
#include "TCPSocket.h"
#include "IPAddressResolver.h"

//Define_Module(ModbusTCP)
//;

ModbusTCP::ModbusTCP() {
    mb_mapping = new modbus_mapping_t;
}

ModbusTCP::~ModbusTCP() {
    delete mb_mapping;
}

/* Treats errors and flush or close connection if necessary */
void ModbusTCP::errorTreat(int code, const char *string) {
    printf("\nERROR %s (%d)\n", string, code);

    if (error_handling == FLUSH_OR_RECONNECT_ON_ERROR) {
        switch (code) {
        case ILLEGAL_DATA_VALUE:
        case ILLEGAL_DATA_ADDRESS:
        case ILLEGAL_FUNCTION:
            break;
        default: {
            //TODO to implement
            //modbus_close(mbEP);
            //modbus_connect(mbEP);
            break;
        }
        }
    }
}

/* Computes the length of the expected response */
unsigned int ModbusTCP::computeResponseLength(uint8_t *query) {
    int resp_length;
    int offset;

    offset = HEADER_LENGTH_TCP;

    switch (query[offset + 1]) {
    case FC_READ_COIL_STATUS:
    case FC_READ_INPUT_STATUS: {
        /* Header + nb values (code from force_multiple_coils) */
        int nb = (query[offset + 4] << 8) | query[offset + 5];
        resp_length = 3 + (nb / 8) + ((nb % 8) ? 1 : 0);
    }
        break;
    case FC_READ_HOLDING_REGISTERS:
    case FC_READ_INPUT_REGISTERS:
        /* Header + 2 * nb values */
        resp_length = 3 + 2 * (query[offset + 4] << 8 | query[offset + 5]);
        break;
    case FC_READ_EXCEPTION_STATUS:
        resp_length = 4;
        break;
    default:
        resp_length = 6;
        break;
    }

    resp_length += offset + CHECKSUM_LENGTH_TCP;

    return resp_length;
}

int ModbusTCP::buildQueryBasis(int slave, int function, int start_addr, int nb,
        uint8_t *query) {

    /* Extract from MODBUS Messaging on TCP/IP Implementation
     Guide V1.0b (page 23/46):
     The transaction identifier is used to associate the future
     response with the request. So, at a time, on a TCP
     connection, this identifier must be unique.
     */
    static uint16_t t_id = 0;

    /* Transaction ID */
    if (t_id < UINT16_MAX)
        t_id++;
    else
        t_id = 0;
    query[0] = t_id >> 8;
    query[1] = t_id & 0x00ff;

    /* Protocol Modbus */
    query[2] = 0;
    query[3] = 0;

    /* Length to fix later with set_query_length_tcp (4 and 5) */

    query[6] = slave;
    query[7] = function;
    query[8] = start_addr >> 8;
    query[9] = start_addr & 0x00ff;
    query[10] = nb >> 8;
    query[11] = nb & 0x00ff;

    return PRESET_QUERY_LENGTH_TCP;
}

/* Builds a TCP response header */
int ModbusTCP::buildResponseBasis(sft_t *sft, uint8_t *response) {
    /* Extract from MODBUS Messaging on TCP/IP Implementation
     Guide V1.0b (page 23/46):
     The transaction identifier is used to associate the future
     response with the request. */
    response[0] = sft->t_id >> 8;
    response[1] = sft->t_id & 0x00ff;

    /* Protocol Modbus */
    response[2] = 0;
    response[3] = 0;

    /* Length to fix later with set_message_length_tcp (4 and 5) */

    response[6] = sft->slave;
    response[7] = sft->function;

    return PRESET_RESPONSE_LENGTH_TCP;
}

void ModbusTCP::setMessageLength(uint8_t *msg, int msg_length) {
    /* Substract the header length to the message length */
    int mbap_length = msg_length - 6;

    msg[4] = mbap_length >> 8;
    msg[5] = mbap_length & 0x00FF;
}

/* Computes the length of the header following the function code */
uint8_t ModbusTCP::computeQueryLengthHeader(int function) {
    int length;

    if (function <= FC_FORCE_SINGLE_COIL
            || function == FC_PRESET_SINGLE_REGISTER)
        /* Read and single write */
        length = 4;
    else if (function == FC_FORCE_MULTIPLE_COILS
            || function == FC_PRESET_MULTIPLE_REGISTERS)
        /* Multiple write */
        length = 5;
    else
        length = 0;

    return length;
}

/* Computes the length of the data to write in the query */
int ModbusTCP::computeQueryLengthData(uint8_t *msg) {
    int function = msg[HEADER_LENGTH_TCP + 1];
    int length;

    if (function == FC_FORCE_MULTIPLE_COILS
            || function == FC_PRESET_MULTIPLE_REGISTERS)
        length = msg[HEADER_LENGTH_TCP + 6];
    else
        length = 0;

    length += CHECKSUM_LENGTH_TCP;

    return length;
}

int ModbusTCP::receive(uint8_t *query, uint8_t *response){
    return receiveMessage(query,response,0);
}

int ModbusTCP::receiveMessage(uint8_t *query, uint8_t *response, int ret) {

    int response_length;
    int response_length_computed;
    int offset = HEADER_LENGTH_TCP;

    response_length_computed = computeResponseLength(query);

    if (ret == 0) {
        /* GOOD RESPONSE */
        int query_nb_value;
        int response_nb_value;

        /* The number of values is returned if it's corresponding
         * to the query */
        switch (response[offset + 1]) {
        case FC_READ_COIL_STATUS:
        case FC_READ_INPUT_STATUS:
            /* Read functions, 8 values in a byte (nb
             * of values in the query and byte count in
             * the response. */
            query_nb_value = (query[offset + 4] << 8) + query[offset + 5];
            query_nb_value = (query_nb_value / 8)
                    + ((query_nb_value % 8) ? 1 : 0);
            response_nb_value = response[offset + 2];
            break;
        case FC_READ_HOLDING_REGISTERS:
        case FC_READ_INPUT_REGISTERS:
            /* Read functions 1 value = 2 bytes */
            query_nb_value = (query[offset + 4] << 8) + query[offset + 5];
            response_nb_value = (response[offset + 2] / 2);
            break;
        case FC_FORCE_MULTIPLE_COILS:
        case FC_PRESET_MULTIPLE_REGISTERS:
            /* N Write functions */
            query_nb_value = (query[offset + 4] << 8) + query[offset + 5];
            response_nb_value = (response[offset + 4] << 8)
                    | response[offset + 5];
            break;
        case FC_REPORT_SLAVE_ID:
            /* Report slave ID (bytes received) */
            query_nb_value = response_nb_value = response_length;
            break;
        default:
            /* 1 Write functions & others */
            query_nb_value = response_nb_value = 1;
            break;
        }

        if (query_nb_value == response_nb_value) {
            ret = response_nb_value;
        } else {
            char *s_error = (char *) malloc(64 * sizeof(char));
            sprintf(s_error,
                    "Quantity (%d) not corresponding to the query (%d)",
                    response_nb_value, query_nb_value);
            ret = ILLEGAL_DATA_VALUE;
            errorTreat(ILLEGAL_DATA_VALUE, s_error);
            free(s_error);
        }
    } else if (ret == COMM_TIME_OUT) {

        if (response_length == (offset + 3 + CHECKSUM_LENGTH_TCP)) {
            /* EXCEPTION CODE RECEIVED */

            /* Optimization allowed because exception response is
             the smallest trame in modbus protocol (3) so always
             raise a timeout error */

            /* Check for exception response.
             0x80 + function is stored in the exception
             response. */
            if (0x80 + query[offset + 1] == response[offset + 1]) {

                int exception_code = response[offset + 2];
                // FIXME check test
                if (exception_code < NB_TAB_ERROR_MSG) {
                    errorTreat(-exception_code,
                            TAB_ERROR_MSG[response[offset + 2]]);
                    /* RETURN THE EXCEPTION CODE */
                    /* Modbus error code is negative */
                    return -exception_code;
                } else {
                    /* The chances are low to hit this
                     case but it can avoid a vicious
                     segfault */
                    char *s_error = (char *) malloc(64 * sizeof(char));
                    sprintf(s_error, "Invalid exception code %d",
                            response[offset + 2]);
                    errorTreat(INVALID_EXCEPTION_CODE, s_error);
                    free(s_error);
                    return INVALID_EXCEPTION_CODE;
                }
            }
            /* If doesn't return previously, return as
             TIME OUT here */
        }

        /* COMMUNICATION TIME OUT */
        errorTreat(ret, "Communication time out");
        return ret;
    }

    return ret;
}

int ModbusTCP::responseIOStatusU8(int address, int nb, tables tab_id,
        uint8_t *response, int offset) {
    int shift = 0;
    int byte = 0;
    int i;

    uint8_t *tab_io_status;

    switch (tab_id) {
    case COIL:
        tab_io_status = mb_mapping->tab_coil_status;
        break;
    case INPUT:
        tab_io_status = mb_mapping->tab_input_status;
        break;
    default:
        return -1;
    }

    for (i = address; i < address + nb; i++) {
        byte |= tab_io_status[i] << shift;
        if (shift == 7) {
            /* Byte is full */
            response[offset++] = byte;
            byte = shift = 0;
        } else {
            shift++;
        }
    }

    if (shift != 0)
        response[offset++] = byte;

    return offset;
}

int ModbusTCP::responseException(sft_t *sft, int exception_code,
        uint8_t *response) {
    int response_length;

    sft->function = sft->function + 0x80;
    response_length = buildResponseBasis(sft, response);

    /* Positive exception code */
    response[response_length++] = -exception_code;

    return response_length;
}

/* By default, the error handling mode used is FLUSH_OR_RECONNECT_ON_ERROR.

 With FLUSH_OR_RECONNECT_ON_ERROR, the library will flush to I/O
 port in RTU mode or attempt an immediate reconnection which may
 hang for several seconds if the network to the remote target unit
 is down in TCP mode.

 With NOP_ON_ERROR, it is expected that the application will
 check for error returns and deal with them as necessary.
 */
void ModbusTCP::setErrorHandling(error_handling_t _error_handling) {
    if (error_handling == FLUSH_OR_RECONNECT_ON_ERROR
            || error_handling == NOP_ON_ERROR) {
        error_handling = _error_handling;
    } else {
        printf("Invalid setting for error handling (not changed)\n");
    }
}

/* Allocates 4 arrays to store coils, input status, input registers and
 holding registers. The pointers are stored in modbus_mapping structure.

 Returns: TRUE if ok, FALSE on failure
 */
bool ModbusTCP::memoryAllocation(int nb_coil_status, int nb_input_status,
        int nb_holding_registers, int nb_input_registers) {
    /* 0X */
    mb_mapping->nb_coil_status = nb_coil_status;
    mb_mapping->tab_coil_status = (uint8_t *) malloc(nb_coil_status * sizeof(uint8_t));
    memset(mb_mapping->tab_coil_status, 0, nb_coil_status * sizeof(uint8_t));
    if (mb_mapping->tab_coil_status == NULL)
        return false;

    /* 1X */
    mb_mapping->nb_input_status = nb_input_status;
    mb_mapping->tab_input_status = (uint8_t *) malloc(nb_input_status * sizeof(uint8_t));
    memset(mb_mapping->tab_input_status, 0, nb_input_status * sizeof(uint8_t));
    if (mb_mapping->tab_input_status == NULL) {
        free(mb_mapping->tab_coil_status);
        return false;
    }

    /* 4X */
    mb_mapping->nb_holding_registers = nb_holding_registers;
    mb_mapping->tab_holding_registers = (uint16_t *) malloc(nb_holding_registers * sizeof(uint16_t));
    memset(mb_mapping->tab_holding_registers, 0,
            nb_holding_registers * sizeof(uint16_t));
    if (mb_mapping->tab_holding_registers == NULL) {
        free(mb_mapping->tab_coil_status);
        free(mb_mapping->tab_input_status);
        return false;
    }

    /* 3X */
    mb_mapping->nb_input_registers = nb_input_registers;
    mb_mapping->tab_input_registers = (uint16_t *) malloc( nb_input_registers * sizeof(uint16_t));
    memset(mb_mapping->tab_input_registers, 0,
            nb_input_registers * sizeof(uint16_t));
    if (mb_mapping->tab_input_registers == NULL) {
        free(mb_mapping->tab_coil_status);
        free(mb_mapping->tab_input_status);
        free(mb_mapping->tab_holding_registers);
        return false;
    }

    return true;
}

/* Frees the 4 arrays */
void ModbusTCP::freeAllocatedMemory() {

    free(mb_mapping->tab_coil_status);
    free(mb_mapping->tab_input_status);
    free(mb_mapping->tab_holding_registers);
    free(mb_mapping->tab_input_registers);
}

/* Initialises data structures */
void ModbusTCP::initMemory() {

    if (memoryAllocation(_UT_COIL_STATUS_ADDRESS + UT_COIL_STATUS_NB_POINTS,
            UT_INPUT_STATUS_ADDRESS + UT_INPUT_STATUS_NB_POINTS,
            UT_HOLDING_REGISTERS_ADDRESS + UT_HOLDING_REGISTERS_NB_POINTS,
            UT_INPUT_REGISTERS_ADDRESS + UT_INPUT_REGISTERS_NB_POINTS)) {

        /** INPUT STATUS **/
        setBitsFromBytesU8(UT_INPUT_STATUS_ADDRESS, UT_INPUT_STATUS_NB_POINTS,
                UT_INPUT_STATUS_TAB, INPUT);

        /** INPUT REGISTERS **/
        for (int i = 0; i < UT_INPUT_REGISTERS_NB_POINTS; i++) {
            mb_mapping->tab_input_registers[UT_INPUT_REGISTERS_ADDRESS + i] =  UT_INPUT_REGISTERS_TAB[i];
        }

    } else {
        ev << "Memory not allocated. Quitting..." << endl;
    }

}

/** Utils **/

void ModbusTCP::setTable(int fc, int p, uint16_t v) {
    switch (fc) {
    case FC_READ_COIL_STATUS:
        setTabCoilStatus(p, (uint8_t) v);
        break;
    case FC_READ_INPUT_STATUS:
        setTabInputStatus(p, (uint8_t) v);
        break;
    }
}

///* Sets many input/coil status from a single byte value (all 8 bits of
// the byte value are setted) */
//void ModbusTCP::setBitsFromBytes(uint8_t *dest, int address, const uint8_t value)
//{
//	int i;
//
//	for (i = 0; i < 8; i++)
//	{
//		dest[address + i] = (value & (1 << i)) ? ON : OFF;
//	}
//}

/* Sets many input/coil status from a table of bytes (only the bits
 between address and address + nb_bits are setted) */
void ModbusTCP::setBitsFromBytesU8(int address, int nb_bits,
        const uint8_t tab_byte[], tables tab_id) {
    int i;
    int shift = 0;
    uint8_t *dest;

    switch (tab_id) {
    case COIL:
        dest = mb_mapping->tab_coil_status;
        break;
    case INPUT:
        dest = mb_mapping->tab_input_status;
        break;
    default:
        return;
    }

    for (i = address; i < address + nb_bits; i++) {
        dest[i] = tab_byte[(i - address) / 8] & (1 << shift) ? ON : OFF;
        /* gcc doesn't like: shift = (++shift) % 8; */
        shift++;
        shift %= 8;
    }
}

/* Gets the byte value from many input/coil status.
 To obtain a full byte, set nb_bits to 8. */
uint8_t ModbusTCP::getByteFromBits(const uint8_t *src, int address,
        int nb_bits) {
    int i;
    uint8_t value = 0;

    if (nb_bits > 8) {
        printf("Error: nb_bits is too big\n");
        nb_bits = 8;
    }

    for (i = 0; i < nb_bits; i++) {
        value |= (src[address + i] << i);
    }

    return value;
}

int ModbusTCP::getNBCoilStatus() {
    return mb_mapping->nb_coil_status;
}

int ModbusTCP::getNBInputStatus() {
    return mb_mapping->nb_input_status;
}

int ModbusTCP::getNBInputRegisters() {
    return mb_mapping->nb_input_registers;
}
int ModbusTCP::getNBHoldingRegisters() {
    return mb_mapping->nb_holding_registers;
}

uint8_t ModbusTCP::getTabCoilStatus(int i) {
    return mb_mapping->tab_coil_status[i];
}

uint8_t ModbusTCP::getTabInputStatus(int i) {
    return mb_mapping->tab_input_status[i];
}
uint16_t ModbusTCP::getTabInputRegisters(int i) {
    return mb_mapping->tab_input_registers[i];
}
uint16_t ModbusTCP::getTabHoldingRegisters(int i) {
    return mb_mapping->tab_holding_registers[i];
}

void ModbusTCP::setTabCoilStatus(int p, uint8_t v) {
    mb_mapping->tab_coil_status[p] = v;
}

void ModbusTCP::setTabInputStatus(int p, uint8_t v) {
    mb_mapping->tab_input_status[p] = v;
}

void ModbusTCP::setTabInputRegisters(int p, uint16_t v) {
    mb_mapping->tab_input_registers[p] = v;
}

void ModbusTCP::setTabHoldingRegisters(int p, uint16_t v) {
    mb_mapping->tab_holding_registers[p] = v;
}

