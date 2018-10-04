/* 
 * File:   SetValueResp.h
 * Author: cleo
 *
 * Created on August 8, 2014, 2:09 PM
 */

#ifndef SETVALUERESP_H
#define	SETVALUERESP_H

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/Response.h>

using namespace Synapse;

class SetValueResp : public Response {	
public:    
    SetValueResp(SubCommandID subCommandId, CapabilityDefinition capability, std::string userHash, SetValueResult result);
    SetValueResp(Header& header) : Response(header) {};
    virtual ~SetValueResp();
    
    CapabilityDefinition getCapability() { return capability_; }
    
    std::string getUserHash() { return userHash_; }     
    
    SetValueResult getResult() { return result_; } 
    
    virtual std::string encode( );
    virtual bool decode(const void* buffer, size_t len);
    
private:
    CapabilityDefinition      capability_;
    std::string               userHash_; 
    SetValueResult            result_;       
};

#endif	/* SETVALUERESP_H */

