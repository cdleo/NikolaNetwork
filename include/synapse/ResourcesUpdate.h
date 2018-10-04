/* 
 * File:   ResourcesUpdate.h
 * Author: cleo
 *
 * Created on September 28, 2014, 11:53 PM
 */

#ifndef RESOURCESUPDATE_H
#define	RESOURCESUPDATE_H

#include <synapse/Update.h>

class ResourcesUpdate : public Update {	
public:    
    ResourcesUpdate(CapabilityDefinition def, ResourceUpdateType type);
    ResourcesUpdate(Header& header) : Update(header) {};
    virtual ~ResourcesUpdate(){};

    ResourceUpdateType getUpdateType() { return type_; }
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    ResourceUpdateType          type_;
};

#endif	/* RESOURCESUPDATE_H */

