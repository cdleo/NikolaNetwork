/* 
 * File:   Response.h
 * Author: cleo
 *
 * Created on July 18, 2014, 9:32 AM
 */

#ifndef RESPONSE_H
#define	RESPONSE_H

#include <synapse/Packet.h>

class Response : public Packet {	
public:    
    Response(CommandID commandID, SubCommandID subcommandID = SubCommandID_Undefined)
    : Packet(MsgType_Response, commandID, subcommandID) {};
    Response(Header& header) : Packet(header) {};
    
    virtual ~Response(){};

};

#endif	/* RESPONSE_H */

