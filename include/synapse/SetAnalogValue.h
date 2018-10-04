/* 
 * File:   SetAnalogValue.h
 * Author: cleo
 *
 * Created on August 8, 2014, 2:10 PM
 */

#ifndef SETANALOGVALUE_H
#define	SETANALOGVALUE_H

#include <synapse/SetValue.h>

class SetAnalogValue : public SetValue {	
public:    
    SetAnalogValue(CapabilityDefinition capability, std::string userHash, long value);
    SetAnalogValue(Header& header) : SetValue(header) {};
    SetAnalogValue(SetAnalogValue* other);
    
    virtual ~SetAnalogValue(){};
        
    long getValue() { return value_; } 
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    long                 value_;
};

#endif	/* SETANALOGVALUE_H */

