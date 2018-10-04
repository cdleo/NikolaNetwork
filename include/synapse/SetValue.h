/* 
 * File:   SetValue.h
 * Author: cleo
 *
 * Created on August 8, 2014, 2:09 PM
 */

#ifndef SETVALUE_H
#define	SETVALUE_H

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/Request.h>

class SetValue : public Request {	
public:    
    SetValue(SubCommandID subCommandId, CapabilityDefinition capability, std::string userHash);
    SetValue(Header& header) : Request(header) {};
    virtual ~SetValue(){};
    
    CapabilityDefinition getCapability() { return capability_; }
    
    std::string getUserHash() { return userHash_; } 
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

    virtual std::string encode( std::string currentData = std::string("") );
    virtual bool decode(const void* buffer, size_t len, unsigned int& pos);    
    
private:
    CapabilityDefinition        capability_;
    std::string                 userHash_;
};

#endif	/* SETVALUE_H */

