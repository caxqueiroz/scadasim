package scadasim.networklayer.autorouting;

//
// This simple module is responsible for configuration of the simulated
// topology, i.e. assignment of IP addresses to all nodes of the topology. 
// Furthermore, routing tables have to be filled and default routing 
// paths must be created.
// Routing is separated into Intra-AS and Inter-AS routing.
//
simple TGMNetworkConfigurator
{
	parameters:
        	int totalCountOfAS;
}
