cplusplus {{
#include "IPControlInfo.h"
#include "SCADASIMDefs.h"
}}

class noncobject IPControlInfo;

class IPControlInfo_hacked extends IPControlInfo
{
	// ReaSE: for attack tracing, each IP packet contains an attackTag
	short attackTag = 0;
}
