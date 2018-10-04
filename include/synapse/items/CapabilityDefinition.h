/* 
 * File:   CapabilityDefinition.h
 * Author: cleo
 *
 * Created on August 24, 2014, 10:03 PM
 */

#ifndef CAPABILITYDEFINITION_H
#define	CAPABILITYDEFINITION_H

#include <synapse/items/SynapseItem.h>

using namespace Synapse;

class CapabilityDefinition : public SynapseItem {
public:
    CapabilityDefinition();
    CapabilityDefinition(const CapabilityDefinition& other);
    virtual ~CapabilityDefinition() {};
    
    virtual std::string encode();
    virtual bool decode(std::string buffer);

    bool operator<(const CapabilityDefinition &right) const {
        return ( id < right.id );
    } 
    
    bool operator==(const CapabilityDefinition &other) const {
        return ( (id == other.id) && (type == other.type) && (access == other.access) && 
               (minReadSecuritylevel == other.minReadSecuritylevel) && (minWriteSecuritylevel == other.minWriteSecuritylevel) && 
               (allowMultipleReadSubscriptions == other.allowMultipleReadSubscriptions) && 
               (allowMultipleWriteSubscriptions == other.allowMultipleWriteSubscriptions) && 
               (description.compare(other.description) == 0) );
    }     
    
    void setDescription(std::string description) {
        this->description = description;
    }

    std::string getDescription() const {
        return description;
    }

    void setAllowMultipleWriteSubscriptions(bool allowMultipleWriteSubscriptions) {
        this->allowMultipleWriteSubscriptions = allowMultipleWriteSubscriptions;
    }

    bool isAllowMultipleWriteSubscriptions() const {
        return allowMultipleWriteSubscriptions;
    }

    void setAllowMultipleReadSubscriptions(bool allowMultipleReadSubscriptions) {
        this->allowMultipleReadSubscriptions = allowMultipleReadSubscriptions;
    }

    bool isAllowMultipleReadSubscriptions() const {
        return allowMultipleReadSubscriptions;
    }

    void setMinWriteSecuritylevel(unsigned int minWriteSecuritylevel) {
        this->minWriteSecuritylevel = minWriteSecuritylevel;
    }

    unsigned int getMinWriteSecuritylevel() const {
        return minWriteSecuritylevel;
    }

    void setMinReadSecuritylevel(unsigned int minReadSecuritylevel) {
        this->minReadSecuritylevel = minReadSecuritylevel;
    }

    unsigned int getMinReadSecuritylevel() const {
        return minReadSecuritylevel;
    }

    void setAccess(AccessTypes access) {
        this->access = access;
    }

    AccessTypes getAccess() const {
        return access;
    }

    void setType(CapabilitiesTypes type) {
        this->type = type;
    }

    CapabilitiesTypes getType() const {
        return type;
    }

    void setId(long id) {
        this->id = id;
    }

    long getId() const {
        return id;
    }            
 
private:
    long                id;
    CapabilitiesTypes   type;
    AccessTypes         access;
    unsigned int        minReadSecuritylevel;
    unsigned int        minWriteSecuritylevel;
    bool                allowMultipleReadSubscriptions;
    bool                allowMultipleWriteSubscriptions;
    std::string         description;        
};   

#endif	/* CAPABILITYDEFINITION_H */

