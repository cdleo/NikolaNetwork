/* 
 * File:   GetAnalogValue.h
 * Author: cleo
 *
 * Created on August 8, 2014, 2:32 PM
 */

#ifndef GETANALOGVALUE_H
#define	GETANALOGVALUE_H

#include <synapse/GetValue.h>

class GetAnalogValue : public GetValue {	
public:    
    GetAnalogValue(CapabilityDefinition capability, std::string userHash)
    : GetValue(SubCommandID_AnalogValue, capability, userHash){};
    
    GetAnalogValue(GetAnalogValue* other)
    : GetValue(SubCommandID_AnalogValue, other->getCapability(), other->getUserHash()){};
    
    GetAnalogValue(Header& header) : GetValue(header) {};
    virtual ~GetAnalogValue(){};

    std::string encode() { return GetValue::encode(); }
    bool decode(const void* buffer, size_t len) { return GetValue::decode(buffer, len); }

};

#endif	/* GETANALOGVALUE_H */

