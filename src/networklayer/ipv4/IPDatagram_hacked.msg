cplusplus {{
#include "IPv4Datagram_m.h"
#include "SCADASIMDefs.h"
}}

packet IPv4Datagram;

packet IPDatagram_hacked extends IPv4Datagram
{
	// ReaSE: for attack tracing each IP packet contains an attackTag
	short attackTag = 0;
}
