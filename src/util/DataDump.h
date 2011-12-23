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

#ifndef __DATADUMP_H__
#define __DATADUMP_H__

#include <omnetpp.h>
#include <mysql.h>
#include "TCPConnection.h"
#include <string>
#include <iostream>
#include <sstream>

/**
 * TODO - Generated class
 */
class DataDump : public cSimpleModule
{
private :

	const char *user;
	const char *passwd;
	const char *database;
	const char *addr;
	char *socket;
	unsigned int port;
	void tcpDump(cMessage *msg, MYSQL *conn, bool received, TCPConnection *tcpConn);
  protected:
    virtual void initialize();
    virtual void finish();
  public:
    void handleMessage(cMessage *msg,TCPConnection *tcpConn, bool received);
    const char * printPath(TCPConnection *tcpConn) ;
};

#endif
