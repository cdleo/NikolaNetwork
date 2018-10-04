/* 
 * File:   CapabilitiesManager.h
 * Author: cleo
 *
 * Created on July 16, 2014, 1:40 PM
 */

#ifndef CAPABILITIESMANAGER_H
#define	CAPABILITIESMANAGER_H

#include <SingletonBase.h>

#include <synapse/CapabilitiesResp.h>
#include <synapse/items/Ident.h>

#include <set>

#define CAPABILITIES CapabilitiesManager::getInstance()

typedef std::set<CapabilityDefinition>       CapabilitiesSet;

typedef struct ModuleCapabilities {
    Ident               module;
    CapabilitiesSet     capabilities;
} ModuleCapabilities;

typedef std::vector<ModuleCapabilities>  ModuleCapabilitiesList;

class CapabilitiesManager : public SingletonBase<CapabilitiesManager> {
public:
    friend class SingletonBase<CapabilitiesManager>;
    
    void load();
    
    CapabilitiesList getLocalCapabilities() {
        
        CapabilitiesList list;
        std::copy(capabilities_.begin(), capabilities_.end(), std::back_inserter(list));
        
        return list;
    }

    ModuleCapabilitiesList& getWideCapabilities() {
        return foreignCapabilities_;
    } 

    void addLocalCapability(CapabilityDefinition capability);
    bool addForeignCapability(Ident ident, CapabilityDefinition capability);
    bool addForeignCapabilities(Ident ident, CapabilitiesList& capabilities);

    bool getCapabilityById(long capabilityID, SourceData from, CapabilityDefinition& capability);
    bool getForeignCapabilityById(long capabilityID, CapabilityDefinition& capability, Ident& module);
    void getCapabilitiesByType(CapabilitiesTypes type, SourceData from, CapabilitiesList& capabilities);

    bool existCapability(CapabilityDefinition def);
     
private:
    CapabilitiesManager();
    virtual ~CapabilitiesManager() throw();
    
    CapabilitiesSet             capabilities_;
    ModuleCapabilitiesList      foreignCapabilities_;
    
    pthread_mutex_t             mutex_;
};

#endif	/* CAPABILITIESMANAGER_H */

