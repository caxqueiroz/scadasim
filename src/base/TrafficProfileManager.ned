package scadasim.base;

//
// Defines simple module for implementation of traffic profile manager.
// The traffic profile manager reads all available traffic profiles from
// a configuration file and returns a random traffic profile if requested.
// In addition it decides if the server communication endpoints lies in
// the current AS or in a foreign AS.
//
simple TrafficProfileManager
{
	parameters:
		// name of the config file containing availabe traffic profiles
		string configFileName;
}
