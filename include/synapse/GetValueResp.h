/* 
 * File:   GetValueResp.h
 * Author: cleo
 *
 * Created on July 16, 2014, 11:55 AM
 */

#ifndef GETVALUERESP_H
#define	GETVALUERESP_H

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/Response.h>

using namespace Synapse;

class GetValueResp : public Response {	
public:    
    GetValueResp(SubCommandID subCommandId, CapabilityDefinition capability, std::string userHash, GetDataResult result);
    GetValueResp(Header& header) : Response(header) {};
    virtual ~GetValueResp() {};
    
    CapabilityDefinition getCapability() { return capability_; }
    
    std::string getUserHash() { return userHash_; }     
    
    GetDataResult getResult() { return result_; } 
    
    virtual std::string encode( );
    virtual bool decode(const void* buffer, size_t len);
    
    virtual std::string encode( std::string currentData = std::string("") );
    virtual bool decode(const void* buffer, size_t len, unsigned int& pos);
    

private:
    CapabilityDefinition      capability_;
    std::string               userHash_; 
    GetDataResult             result_;       
};

#endif	/* GETVALUERESP_H */

