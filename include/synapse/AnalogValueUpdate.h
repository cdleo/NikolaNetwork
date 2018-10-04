/* 
 * File:   AnalogValueUpdate.h
 * Author: cleo
 *
 * Created on July 7, 2014, 9:46 PM
 */

#ifndef ANALOGVALUEUPDATE_H
#define	ANALOGVALUEUPDATE_H

#include <synapse/Update.h>

class AnalogValueUpdate : public Update {	
public:    
    AnalogValueUpdate(CapabilityDefinition def, long value);
    AnalogValueUpdate(Header& header) : Update(header) {};
    virtual ~AnalogValueUpdate(){};

    long getValue() {
        return value_;
    }

    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    long                        value_;
};

#endif	/* ANALOGVALUEUPDATE_H */

