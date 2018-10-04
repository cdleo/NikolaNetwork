/* 
 * File:   Update.h
 * Author: cleo
 *
 * Created on August 8, 2014, 4:01 PM
 */

#ifndef UPDATE_H
#define	UPDATE_H

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/Response.h>

class Update : public Response {	
public:    
    Update(SubCommandID subCommandId, CapabilityDefinition def);
    Update(Header& header) : Response(header) {};
    virtual ~Update(){};
    
    CapabilityDefinition getCapability() { return capability_; }

    std::string encode() { return ""; }
    bool decode(const void* buffer, size_t len) { return false; }

    virtual std::string encode( std::string currentData = std::string("") );
    virtual bool decode(const void* buffer, size_t len, unsigned int& pos);    
        
    
private:
    CapabilityDefinition        capability_;
};

#endif	/* UPDATE_H */

