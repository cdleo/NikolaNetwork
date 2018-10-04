/* 
 * File:   Subscribe.h
 * Author: cleo
 *
 * Created on July 7, 2014, 9:46 PM
 */

#ifndef SUBSCRIBE_H
#define	SUBSCRIBE_H

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/Request.h>

class Subscribe : public Request {	
public:    
    Subscribe(CapabilityDefinition capability, std::string userHash);
    Subscribe(Header& header) : Request(header) {};
    Subscribe(Subscribe* other);
    virtual ~Subscribe(){};
    
    CapabilityDefinition getCapability() { return capability_; }
    
    std::string getUserHash() { return userHash_; } 
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    CapabilityDefinition        capability_;
    std::string                 userHash_;  
};

#endif	/* SUBSCRIBE_H */

