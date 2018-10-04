/* 
 * File:   Unsubscribe.h
 * Author: cleo
 *
 * Created on July 7, 2014, 9:46 PM
 */

#ifndef UNSUBSCRIBE_H
#define	UNSUBSCRIBE_H

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/Request.h>

class Unsubscribe : public Request {	
public:    
    Unsubscribe(CapabilityDefinition capability, std::string userHash);
    Unsubscribe(Header& header) : Request(header) {};
    Unsubscribe(Unsubscribe* other);
    virtual ~Unsubscribe(){};
    
    CapabilityDefinition getCapability() { return capability_; }
    
    std::string getUserHash() { return userHash_; } 
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    CapabilityDefinition        capability_;  
    std::string                 userHash_;
};

#endif	/* UNSUBSCRIBE_H */

