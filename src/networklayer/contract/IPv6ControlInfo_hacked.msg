cplusplus {{
#include "IPv6ControlInfo.h"
#include "SCADASIMDefs.h"
}}

class noncobject IPv6ControlInfo;

class IPv6ControlInfo_hacked extends IPv6ControlInfo
{
	// ReaSE: for attack tracing, each IP packet contains an attackTag
	short attackTag = 0;
}
