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
 * ModbusGate.cpp
 *
 *  Created on: 09/05/2011
 *      Author: cax
 */

#include "ModbusGate.h"

ModbusGate::ModbusGate() :
	SSGate() {
	port = 1502;
	modbus_init_tcp(&mb_param, "localhost", port);
	modbus_set_debug(&mb_param, FALSE);
	bool ret = modbus_mapping_new(&mb_mapping, 500, 500, 500, 500);
	if (ret == FALSE) {
		printf("Memory allocation failed\n");
		exit(1);
	}
	cout << "modbus db initialised" << endl;
}

ModbusGate::~ModbusGate() {
	modbus_mapping_free(&mb_mapping);
	modbus_close(&mb_param);
	cout << "modbus memory freed" << endl;
}

void ModbusGate::Open() {
	cout << "just got started" << endl;
	start(NULL);
	cout << "thread created." << endl;
}
void ModbusGate::execute(void * arg) {

	this->listen();
}

void ModbusGate::processMessage(ssMsg * msg) {
	SSGate::processMessage(msg);

}

void ModbusGate::listen() {
	struct sockaddr_in addr;
	socklen_t addrlen;

	addr.sin_family = AF_INET;
	/* If the modbus port is < to 1024, we need the setuid root. */
	addr.sin_port = htons(mb_param.port);
	addr.sin_addr.s_addr = INADDR_ANY;
	memset(&(addr.sin_zero), '\0', 8);
	addrlen = sizeof(struct sockaddr_in);

	socket_conn = modbus_init_listen_tcp(&mb_param);
	if (socket_conn) {
		while (1) {
			uint8_t query[MAX_MESSAGE_LENGTH];
			int query_size;

			int ret = modbus_listen(&mb_param, query, &query_size);
			if (ret == 0) {
				ssMsg msg;
				msg.extAddress = mb_param.remote_ip;
				msg.payload = query;
				processMessage(&msg);
				modbus_manage_query(&mb_param, query, query_size, &mb_mapping);
			} else if (ret == CONNECTION_CLOSED) {
				/* Connection closed by the client, end of server */
				mb_param.fd = accept(socket_conn, (struct sockaddr *) &addr,
						&addrlen);
				if (mb_param.fd < 0) {
					perror("accept");
					close(socket_conn);
					socket_conn = 0;
					break;
				} else {
					printf("The client %s is connected\n",
							inet_ntoa(addr.sin_addr));
				}

			} else {
				printf("Error in modbus_listen (%d)\n", ret);
			}
		}
	}

}

void ModbusGate::Close() {
	cout << "stop just got called" << endl;
	join();
	cout << "thread finished" << endl;
	//close(socket_conn);
}
