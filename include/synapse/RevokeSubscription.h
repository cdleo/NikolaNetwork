/* 
 * File:   RevokeSubscription.h
 * Author: cleo
 *
 * Created on September 8, 2014, 4:09 PM
 */

#ifndef REVOKE_H
#define	REVOKE_H

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/Request.h>

class RevokeSubscription : public Request {	
public:    
    RevokeSubscription(CapabilityDefinition capability, std::string userHash, std::string peerIDRevoked);
    RevokeSubscription(Header& header) : Request(header) {};
    RevokeSubscription(RevokeSubscription* other);
    virtual ~RevokeSubscription(){};
    
    CapabilityDefinition getCapability() { return capability_; }
    
    std::string getUserHash() { return userHash_; } 
    
    std::string getPeerIDRevoked() { return peerIDRevoked_; } 
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    CapabilityDefinition        capability_;
    std::string                 userHash_;  
    std::string                 peerIDRevoked_;  
};

#endif	/* REVOKE_H */

