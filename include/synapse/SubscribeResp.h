/* 
 * File:   SubscribeResp.h
 * Author: cleo
 *
 * Created on July 16, 2014, 11:55 AM
 */

#ifndef SUBSCRIBERESP_H
#define	SUBSCRIBERESP_H

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/Response.h>

using namespace Synapse;

class SubscribeResp : public Response {	
public:    
    SubscribeResp(CapabilityDefinition capability, std::string userHash, SubscribeResult result);
    SubscribeResp(Header& header) : Response(header) {};
    virtual ~SubscribeResp(){};
    
    CapabilityDefinition getCapability() { return capability_; }
    
    std::string getUserHash() { return userHash_; } 
    
    SubscribeResult getResult() { return result_; } 
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    CapabilityDefinition        capability_;
    std::string                 userHash_; 
    SubscribeResult             result_;       
};

#endif	/* SUBSCRIBERESP_H */

