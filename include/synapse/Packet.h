/* 
 * File:   Packet.h
 * Author: cleo
 *
 * Created on July 3, 2014, 10:26 AM
 */

#ifndef PACKET_H
#define	PACKET_H

#include <synapse/items/SynapseItem.h>
#include <synapse/Header.h>
#include <sstream>

class Packet {		
public:   
    virtual ~Packet(){ 
    }
    
    MsgType getMsgType(){ return h_.getMsgType(); }
    
    CommandID getCommandID(){ return h_.getCommandID(); }
    
    SubCommandID getSubCommandID(){ return h_.getSubCommandID(); }
    
    unsigned int getLength(){ return h_.getLength(); }
    
    virtual std::string encode(){ return h_.serialize(); };
    virtual bool decode(const void* buffer, size_t len){ return false; };
    
    virtual std::string encode( const std::string currentData );
    virtual bool decode(const void* buffer, size_t len, unsigned int& pos);

protected:	
    Packet(Header& header);
    Packet(MsgType msgType, CommandID commandID, SubCommandID subCommandID);

    bool getField(std::string aux, unsigned int& pos, SynapseItem* field );
    bool getField(std::string aux, unsigned int& pos, void* field );
    bool getField(std::string aux, unsigned int& pos, std::string& field );
    bool getField(std::string aux, unsigned int& pos, unsigned int& field );
    bool getField(std::string aux, unsigned int& pos, long& field );
    bool getField(std::string aux, unsigned int& pos, bool& field );
    
    Header	h_;  
};

#endif	/* PACKET_H */

