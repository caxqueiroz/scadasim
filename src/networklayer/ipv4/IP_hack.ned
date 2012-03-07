package scadasim.networklayer.ipv4;

import inet.networklayer.ipv4.IPv4;

//
// An extension of inet.networklayer.ipv4.IP
// This module additionally allows for address spoofing and
// packet tracing
//

simple IP_hack extends IPv4
{
    parameters:
        @class(IP_hack);
        bool tracingOn;
        double tracingInterval @unit(s);
        double traceStartTime @unit(s);
        bool spoofingOn; // allow IP-Address Spoofing
}

