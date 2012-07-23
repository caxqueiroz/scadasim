package scadasim.applications.ddos;

//
// many of the following parameters are a string-type. You can specify attack characteristics
// as a char-list, seperated by whitespaces. So you can define more than one attack for the
// current simulation
// example:
// attackStartTime = "10 15.3 40 52"  --> means at sec. 10 a first attack begins, followed in sec 15.3...

//
// This simple module implements a generator for realistic DDoS attacks.
// Multiple attack waves can be simulated as well as multiple attacks
// on different victims. The zombies apply ramp-up behavior and can take
// part in only one attack at a time.
//
simple TribeFloodNetwork

{
    parameters:
        //
        // time when flooding begins and the delta value for each Zombie
        //
        string attackStartTime;
        double attackStartDiff @unit(s);
        //
        //  total flooding packet amount. A negative value means flooding4livetime
        //
        string maxPacket;
        //
        //  waittime between packets. Waittime = min-value + diff-value
        //
        double timeBetweenPacketMin @unit(s);
        double timeBetweenPacketDiff @unit(s);
        //
        // packet size for UDP or ICMP flooding
        //
        string packetSize;
        //
        // attack type:
        // SYN-Flood : 1
        // SYN-Ack-Flood: 2 //not yet implemented
        // RST-Flood: 3     //not yet implemented
        // ICMP-Flood: 4
        // SMURF-Flood: 5   //not yet implemented
        // UDP-Flood: 6
        //
        string attackType;
        //
        // ip-address of the victim. Instead of using the IP-Address use the module's fullPath
        // simulation.as.node
        //
        string victimAddress;
        //
        // value -1 means walk through possible ports
        //
        string destinationPort;
        //
        // activated addressspoofing needs a network prefix
        //
        bool addressSpoofingOn;
        string sourceAddressStart;
        string sourceAddressEnd;
        //
        // value -1 means walk through possible ports
        //
        string sourcePort;

        //
        // if you run multiple attacks, at each startTime (defined above attackStartTime) the zombie system
        // determines, if he starts, stops or switches to another attack type.
        //
        double attackOnProbability;
        double attackSwitchProbability;
        double attackOffProbability;

    //
    // only one way connection from DDoS-Application to network is used.
    // all incoming packets are just discarded
    //
    gates:
        input ipIn[];
        input ipv6In[];
        output to_ip_tcp;
        output to_ip_udp;
        output to_ip_ping;
        output to_ipv6_tcp;
        output to_ipv6_udp;
        output to_ipv6_ping;
}
