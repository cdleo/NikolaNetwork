/* 
 * File:   GetDigitalValue.h
 * Author: cleo
 *
 * Created on August 8, 2014, 2:31 PM
 */

#ifndef GETDIGITALVALUE_H
#define	GETDIGITALVALUE_H

#include <synapse/GetValue.h>

class GetDigitalValue : public GetValue {	
public:    
    GetDigitalValue(CapabilityDefinition capability, std::string userHash)
    : GetValue(SubCommandID_DigitalValue, capability, userHash){};
    
    GetDigitalValue(GetDigitalValue* other)
    : GetValue(SubCommandID_DigitalValue, other->getCapability(), other->getUserHash()){};
    
    GetDigitalValue(Header& header) : GetValue(header) {};
    virtual ~GetDigitalValue(){};

    std::string encode() { return GetValue::encode(); }
    bool decode(const void* buffer, size_t len) { return GetValue::decode(buffer, len); }

};


#endif	/* GETDIGITALVALUE_H */

