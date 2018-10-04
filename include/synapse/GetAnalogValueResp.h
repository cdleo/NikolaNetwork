/* 
 * File:   GetAnalogValueResp.h
 * Author: cleo
 *
 * Created on August 8, 2014, 2:32 PM
 */

#ifndef GETANALOGVALUERESP_H
#define	GETANALOGVALUERESP_H

#include <synapse/GetValueResp.h>

using namespace Synapse;

class GetAnalogValueResp : public GetValueResp {	
public:    
    GetAnalogValueResp(CapabilityDefinition capability, std::string userHash, GetDataResult result, long value);
    GetAnalogValueResp(Header& header) : GetValueResp(header) {};
    virtual ~GetAnalogValueResp() {};
    
    long getValue() { return value_; } 
    
    virtual std::string encode( );
    virtual bool decode(const void* buffer, size_t len);

private:
    long                value_;
};

#endif	/* GETANALOGVALUERESP_H */

