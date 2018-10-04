/* 
 * File:   ServiceDefinition.h
 * Author: cleo
 *
 * Created on August 25, 2014, 3:47 PM
 */

#ifndef SERVICEDEFINITION_H
#define	SERVICEDEFINITION_H

#include <synapse/items/SynapseItem.h>

using namespace Synapse;

class ServiceDefinition : public SynapseItem {
public:
    ServiceDefinition();
    ServiceDefinition(const ServiceDefinition& other);
    virtual ~ServiceDefinition() {};

    virtual std::string encode();
    virtual bool decode(std::string buffer);

    bool operator<(const ServiceDefinition &right) const {
        return ( id < right.id );
    } 
    
    bool operator==(const ServiceDefinition &other) const {
        return ( (id == other.id) && (type == other.type) && (description.compare(other.description) == 0) );
    }     
    
    bool equals(const ServiceDefinition& other);
    
    void setDescription(std::string description) {
        this->description = description;
    }

    std::string getDescription() const {
        return description;
    }

    void setType(ServiceTypes type) {
        this->type = type;
    }

    ServiceTypes getType() const {
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
    ServiceTypes        type;
    std::string         description;    
};

#endif	/* SERVICEDEFINITION_H */

