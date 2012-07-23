package scadasim.applications.generic;

//
// Simple module for implementation of the actual client host.
// Clients are used for generation of realistic background traffic by
// starting transmissions based on random traffic profile selection.
//
simple InetUser
{
    parameters:
        //
        // Time to start the first transmission
        //
        double startTime @unit(s);
}
