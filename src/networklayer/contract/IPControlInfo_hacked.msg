cplusplus {{
#include "IPv4ControlInfo.h"
#include "SCADASIMDefs.h"
}}

class noncobject IPv4ControlInfo;

class IPControlInfo_hacked extends IPv4ControlInfo
{
	// ReaSE: for attack tracing, each IP packet contains an attackTag
	short attackTag = 0;
}
