package scadasim.transport.udp;


//
// Overwrites original UDP.ned file.
// This module handles undeliverable GenericAppMsg messages tha otherwise
// would cause ICMP error messages due to socket connections that are closed 
// too early.
//
simple UDP_hack
{
    gates:
        input appIn[];
        input ipIn;
        input ipv6In;
        output appOut[];
        output ipOut;
        output ipv6Out;
}


