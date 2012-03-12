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

#include "ModbusTCPApplication.h"
#include "ModbusApplicationSlaveThread.h"
#include "ModbusApplicationMasterThread.h"
#include "IPAddressResolver.h"
#include "ModbusUser.h"

Define_Module(ModbusTCPApplication);

ModbusTCPApplication::ModbusTCPApplication(){
    modbus.initMemory();
}

ModbusTCPApplication::~ModbusTCPApplication(){
    modbus.freeAllocatedMemory();
}

ModbusTCP ModbusTCPApplication::getModbusTCPStack(){
    return modbus;
}


void ModbusTCPApplication::initialize(int stages){

    if (stages != INITIALIZATION_STAGE_NECESSARY)
            return;

        // registers to InetUser and ConnectionManager
        if (stages == INITIALIZATION_STAGE_NECESSARY)
            {
                // checks if application was set correctly by sub-classes
                if (!applicationType)
                    opp_error("invalid application type: did you forget to set a proper application type in constructor?");

                isServer = par("isServer").boolValue();
                profileNumber = par("profileNumber").longValue();
                port = par("port").longValue();
                // get the connectionManager
                cm = NULL;
                ConnectionManagerAccess cac;
                cm = cac.get();

                if (cm == NULL)
                    opp_error("couldn't get ConnectionManager");

                // register servers at ConnectionManager
                if (isServer)
                    cm->registerServer(IPAddressResolver().resolve(getParentModule()->getFullPath().data()), port, profileNumber);
                // if i'am not a slave - this means a master, i have to
                // register myself to the current ModbusUser
                else
                {
                    ModbusUserAccess mua;
                    mbu = mua.get();
                    mbu->setApplication(applicationType, this,profileNumber);
                    // for client - create a TrafficProfile
                    // and set necessary watches
                    WATCH(curProfile.requestLength);
                    WATCH(curProfile.requestsPerSession);
                    WATCH(curProfile.replyLength);
                    WATCH(curProfile.replyPerRequest);
                    WATCH(curProfile.timeBetweenRequests);
                    WATCH(curProfile.timeToRespond);
                    WATCH(curProfile.timeBetweenSessions);
                    WATCH(curProfile.probability);
                    WATCH(curProfile.WANprob);
                    WATCH(curProfile.profileID);
                }
            }

        if (isServer)
        {
            // open the ServerSocket and start listening
            serverSocket = new TCPSocket();
            serverSocket->setOutputGate(gate("tcpOut"));
            serverSocket->bind(IPvXAddress(), port);
            serverSocket->listen();
            maxThreadCount = par("noThreads");
            cout << "Server Listening to new connections: " << this->getFullPath() << " threads: " << maxThreadCount << endl;
            // a server may have a limited number of concurrently running threads

        }


        if (maxThreadCount == 0)
            maxThreadCount = INT_MAX;
}

void ModbusTCPApplication::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        GenericTCPApplicationThreadBase *thread = (GenericTCPApplicationThreadBase *) msg->getContextPointer();
        thread->timerExpired(msg);
    }
    else
    {
        // a new connection attempt so create new thread for this communication
        TCPSocket *socket = socketMap.findSocketFor(msg);
        if (!socket)
        {
            if (!isServer)
                opp_error("received client message to unknown socket\n");
            if (threadCount >= maxThreadCount)
            {
                // if thread limit is hit - kill all incoming connection attempts
                // TODO: to do it correct the socket should be closed to indicate
                // the overload situation of the server. This, however, does not
                // work correctly with the INET framework. Thus, connection is
                // excepted and killed immediately.
                socket = new TCPSocket(msg);
                socket->setOutputGate(gate("tcpOut"));

                if (socket->getState() != TCPSocket::CLOSED)
                    socket->close();
                delete socket;
                delete msg;
                return;
            }

            // valid connection attempts get their own server thread
            socket = new TCPSocket(msg);
            socket->setOutputGate(gate("tcpOut"));
            GenericTCPApplicationThreadBase *thread = new ModbusApplicationSlaveThread();
            socket->setCallbackObject(thread);
            thread->init(this, socket);
            socketMap.addSocket(socket);
            threadCount++;
            threadList.push_back(thread);

            updateDisplay();
        }

        // process messages to already opened sockets
        socket->processMessage(msg);
    }
}

void ModbusTCPApplication::transmissionStart(TrafficProfile &p, TargetInfo &i)
{
    Enter_Method_Silent();
    //
    // this methode is only called for a client
    //
    ASSERT(!isServer);
    //
    // randomize the profile values
    //
    curProfile = p;
    //
    // set socket and gate
    //
    TCPSocket *socket = new TCPSocket();
    socket->bind(IPvXAddress(), curProfile.ownPort);
    socket->setOutputGate(gate("tcpOut"));
    //
    // create an active ClientThread
    //
    GenericTCPApplicationThreadBase *t = new ModbusApplicationMasterThread(curProfile, i);
    socket->setCallbackObject(t);
    t->init(this, socket);
    socketMap.addSocket(socket);
    threadCount++;
    threadList.push_back(t);

    updateDisplay();
}

void ModbusTCPApplication::transmissionDone(TransmissionStatistics s){
    if(mbu)
        mbu->transmissionDone(s);
}
