/* 
 * File:   Header.h
 * Author: cleo
 *
 * Created on July 3, 2014, 10:19 AM
 */

#ifndef HEADER_H
#define	HEADER_H

#include <synapse/GlobalDefinitions.h>
#include <stdexcept>
#include <string.h>
#include <stdio.h>
#include <Log.h>
#include <ios>

using namespace Synapse;

//Header comun a todas las primitivas
class Header{	
public:
    Header (MsgType msgType=MsgType_Undefined, CommandID commandID=CommandID_Undefined, SubCommandID subcommandID=SubCommandID_Undefined, unsigned int length = sizeof(Header) )
    :  msgType_(msgType), commId_(commandID), subCommId_(subcommandID), commLength_(length){};

    Header(const Header& other) {
        msgType_ = other.msgType_;
        commId_ = other.commId_;
        subCommId_ = other.subCommId_;
        commLength_ = other.commLength_;
    }
		
    MsgType getMsgType()  {return msgType_; }    
    
    CommandID getCommandID() { return commId_; }    
    
    SubCommandID getSubCommandID() { return subCommId_; }  
    
    void setLength ( unsigned int length ) {commLength_=length;}
    unsigned int getLength() { return commLength_; }
    
    std::string serialize() {      
        return std::string( (char*) this, sizeof(Header) );
    }       
    
private:
    MsgType             msgType_;
    CommandID           commId_;
    SubCommandID        subCommId_;
    unsigned int        commLength_;    
};

#endif	/* HEADER_H */

