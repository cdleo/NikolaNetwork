/* 
 * File:   GetDigitalValueResp.h
 * Author: cleo
 *
 * Created on August 8, 2014, 2:31 PM
 */

#ifndef GETDIGITALVALUERESP_H
#define	GETDIGITALVALUERESP_H

#include <synapse/GetValueResp.h>

using namespace Synapse;

class GetDigitalValueResp : public GetValueResp {	
public:    
    GetDigitalValueResp(CapabilityDefinition capability, std::string userHash, GetDataResult result, bool value);
    GetDigitalValueResp(Header& header) : GetValueResp(header) {};
    virtual ~GetDigitalValueResp() {};
    
    bool getValue() { return value_; } 
    
    virtual std::string encode( );
    virtual bool decode(const void* buffer, size_t len);

private:
    bool                value_;
};

#endif	/* GETDIGITALVALUERESP_H */

