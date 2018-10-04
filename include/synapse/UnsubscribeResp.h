/* 
 * File:   UnsubscribeResp.h
 * Author: cleo
 *
 * Created on July 16, 2014, 11:55 AM
 */

#ifndef UNSUBSCRIBERESP_H
#define	UNSUBSCRIBERESP_H

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/Response.h>

using namespace Synapse;

class UnsubscribeResp : public Response {	
public:    
    UnsubscribeResp(CapabilityDefinition capability, std::string userHash, UnsubscribeResult result);
    UnsubscribeResp(Header& header) : Response(header) {};
    virtual ~UnsubscribeResp(){};
    
    CapabilityDefinition getCapability() { return capability_; }
    
    std::string getUserHash() { return userHash_; }     
    
    UnsubscribeResult getResult() { return result_; } 
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    CapabilityDefinition        capability_;
    std::string                 userHash_; 
    UnsubscribeResult           result_;       
};

#endif	/* UNSUBSCRIBERESP_H */

