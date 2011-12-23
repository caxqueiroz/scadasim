cplusplus {{
#include "IPDatagram.h"
#include "ReaSEDefs.h"
}}

packet IPDatagram;

packet IPDatagram_hacked extends IPDatagram
{
	// ReaSE: for attack tracing each IP packet contains an attackTag
	short attackTag = 0;
}
