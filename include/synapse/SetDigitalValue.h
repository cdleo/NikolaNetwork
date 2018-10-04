/* 
 * File:   SetDigitalValue.h
 * Author: cleo
 *
 * Created on August 8, 2014, 2:10 PM
 */

#ifndef SETDIGITALVALUE_H
#define	SETDIGITALVALUE_H

#include <synapse/SetValue.h>

class SetDigitalValue : public SetValue {	
public:    
    SetDigitalValue(CapabilityDefinition capability, std::string userHash, bool value);
    SetDigitalValue(Header& header) : SetValue(header) {};
    SetDigitalValue(SetDigitalValue* other);
            
    virtual ~SetDigitalValue(){};
        
    bool getValue() { return value_; } 
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    bool                 value_;
};

#endif	/* SETDIGITALVALUE_H */

