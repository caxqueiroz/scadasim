package scadasim.base;

//
// Simple module for connection manager 
// This manager is responsible for requesting traffic profiles and
// server connection endpoints.
//
simple ConnectionManager
{
	parameters:
		//
		// Duration of the simlation run
		//
		double simulationDuration @unit(s);
}
