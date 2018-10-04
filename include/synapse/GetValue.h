/* 
 * File:   GetValue.h
 * Author: cleo
 *
 * Created on July 7, 2014, 9:46 PM
 */

#ifndef GETVALUE_H
#define	GETVALUE_H

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/Request.h>

class GetValue : public Request {	
public:    
    GetValue(SubCommandID subCommandId, CapabilityDefinition capability, std::string userHash);
    GetValue(Header& header) : Request(header) {};
    virtual ~GetValue(){};
    
    CapabilityDefinition getCapability() { return capability_; }
    
    std::string getUserHash() { return userHash_; } 
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    CapabilityDefinition        capability_;
    std::string                 userHash_;
};

#endif	/* GETVALUE_H */

