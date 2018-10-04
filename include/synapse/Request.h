/* 
 * File:   Request.h
 * Author: cleo
 *
 * Created on July 7, 2014, 9:50 PM
 */

#ifndef REQUEST_H
#define	REQUEST_H

#include <synapse/Packet.h>

class Request : public Packet {	
public:    
    Request(CommandID commandID, SubCommandID subcommandID = SubCommandID_Undefined)
    : Packet(MsgType_Request, commandID, subcommandID) {};
    Request(Header& header) : Packet(header) {};
    
    virtual ~Request(){};

};

#endif	/* REQUEST_H */

