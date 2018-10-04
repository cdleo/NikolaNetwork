/* 
 * File:   DigitalValueUpdate.h
 * Author: cleo
 *
 * Created on July 7, 2014, 9:46 PM
 */

#ifndef DIGITALVALUEUPDATE_H
#define	DIGITALVALUEUPDATE_H

#include <synapse/Update.h>

class DigitalValueUpdate : public Update {	
public:    
    DigitalValueUpdate(CapabilityDefinition def, bool value);
    DigitalValueUpdate(Header& header) : Update(header) {};
    virtual ~DigitalValueUpdate(){};

    bool getValue() {
        return value_;
    }

    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    bool                        value_;
};

#endif	/* DIGITALVALUEUPDATE_H */

