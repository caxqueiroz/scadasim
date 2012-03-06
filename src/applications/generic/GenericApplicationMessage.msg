cplusplus {{
#include "INETDefs.h"
#include "SCADASIMDefs.h"
}}

//
// Defines a message format for the packets exchanged
// during a client/server communication. These packets
// contain all information necessary for the server to
// create an appropriate reply.
//

packet GenericApplicationMessage
{
    int replyLength; //bytes
    int replyPerRequest; //how often respond to incoming request
    double timeToRespond; //reply after this many seconds
    bool last; //this indicates the last packet --> close socket
    int packetNumber; //increasing number for each packet
}
