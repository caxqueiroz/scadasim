package scadasim.applications.pingApp;

//
// Simple module for implementation of an application based on ICMP
//
simple GenericICMPPingApplication
{
    parameters:
        //
        // should this application act as a server (true) or a client (false)
        //
        bool isServer;
        //
        // if it acts as a server, the server must know his traffic profile number.
        // If it acts as a client, the client can specify a profileNumber not equal -1
        // to register as a receiver for the specific profile
        //
        int profileNumber;
        //
        // and he has to know at which port he has to listen. Like above, a client
        // which set a profileNumber not equal to -1 has to specify a valid port
        // (for ICMP Ping the port is uninteresting)
        //
        int port;
        //
        // at least a udp or tcp server has to specify how many concurrently running
        // threads it will create. A Value of 0 means, that theres no limit for the
        // number of threads. (This value is ignored for the ICMP Application)
        //
        int noThreads;
    gates:
        input pingIn;
        output pingOut;
}
