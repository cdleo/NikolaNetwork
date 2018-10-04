/* 
 * File:   RevokeSubscriptionResp.h
 * Author: cleo
 *
 * Created on September 8, 2014, 4:10 PM
 */

#ifndef REVOKERESP_H
#define	REVOKERESP_H

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/Response.h>

using namespace Synapse;

class RevokeSubscriptionResp : public Response {	
public:    
    RevokeSubscriptionResp(CapabilityDefinition capability, RevokeResult result);
    RevokeSubscriptionResp(Header& header) : Response(header) {};
    RevokeSubscriptionResp(RevokeSubscriptionResp* other);
    virtual ~RevokeSubscriptionResp(){};
    
    CapabilityDefinition getCapability() { return capability_; }
    
    RevokeResult getResult() { return result_; } 
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    CapabilityDefinition        capability_;
    RevokeResult                result_;       
};

#endif	/* REVOKERESP_H */

