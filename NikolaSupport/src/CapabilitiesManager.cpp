/* 
 * File:   CapabilitiesManager.cpp
 * Author: cleo
 * 
 * Created on July 16, 2014, 1:40 PM
 */

#include <support/CapabilitiesManager.h>
#include <Parameters.h>
#include <BaseUtilities.h>

CapabilitiesManager::CapabilitiesManager() {
   pthread_mutex_init(&mutex_,NULL);
   capabilities_.clear();
}

CapabilitiesManager::~CapabilitiesManager() throw() {
    capabilities_.clear();
    pthread_mutex_destroy(&mutex_);
}

void CapabilitiesManager::load() {
    
    pthread_mutex_lock(&mutex_);
    std::vector<std::string> caps;
    PARAMETERS->getValue("capabilities", caps);    
    for ( std::vector<std::string>::iterator it = caps.begin(); it != caps.end(); it++ ){
        std::vector<std::string> splitedCap = UTILITIES->split( *it, '|' );
        if (splitedCap.size() != 8) continue;
    
        CapabilityDefinition def;
        def.setId( atol(splitedCap.at(0).c_str()) );
        def.setType( (CapabilitiesTypes) atol(splitedCap.at(1).c_str()) );
        def.setAccess( (AccessTypes) atol(splitedCap.at(2).c_str()) );
        def.setMinReadSecuritylevel( atoi(splitedCap.at(3).c_str()) );
        def.setMinWriteSecuritylevel( atoi(splitedCap.at(4).c_str()) );
        def.setAllowMultipleReadSubscriptions( (splitedCap.at(5).compare("true") == 0) );
        def.setAllowMultipleWriteSubscriptions( (splitedCap.at(6).compare("true") == 0) );
        def.setDescription( splitedCap.at(7) );
        capabilities_.insert(def);          
    }
    pthread_mutex_unlock(&mutex_);
}

bool CapabilitiesManager::existCapability(CapabilityDefinition def){
    bool result = false;
    pthread_mutex_lock(&mutex_);
    result = (capabilities_.find(def) != capabilities_.end());
    pthread_mutex_unlock(&mutex_);
    return result;
}

void CapabilitiesManager::addLocalCapability(CapabilityDefinition capability){
    pthread_mutex_lock(&mutex_);
    capabilities_.insert(capability);
    pthread_mutex_unlock(&mutex_);
}

bool CapabilitiesManager::addForeignCapability(Ident ident, CapabilityDefinition capability){
    
    pthread_mutex_lock(&mutex_);
    for (ModuleCapabilitiesList::iterator it = foreignCapabilities_.begin(); it != foreignCapabilities_.end(); it++){
        if ( it->module.equals( ident ) ) {
            CapabilitiesSet capSet = it->capabilities;
            capSet.insert( capability );
            it->capabilities = capSet;
            pthread_mutex_unlock(&mutex_);
            return true;
        }
    }
    
    ModuleCapabilities modCap;
    modCap.module = ident;
    modCap.capabilities.insert( capability );
    foreignCapabilities_.push_back( modCap );
    pthread_mutex_unlock(&mutex_);
    return true;
}

bool CapabilitiesManager::addForeignCapabilities(Ident ident, CapabilitiesList& capabilities){
    
    pthread_mutex_lock(&mutex_);
    for (ModuleCapabilitiesList::iterator it = foreignCapabilities_.begin(); it != foreignCapabilities_.end(); it++){
        if ( it->module.equals( ident ) ) {
            CapabilitiesSet capSet = it->capabilities;
            for (CapabilitiesList::iterator capIt = capabilities.begin(); capIt != capabilities.end(); capIt++) {
                capSet.insert( *capIt );
            }
            it->capabilities = capSet;
            pthread_mutex_unlock(&mutex_);
            return true;
        }
    }
    
    ModuleCapabilities modCap;
    modCap.module = ident;
    for (CapabilitiesList::iterator capIt = capabilities.begin(); capIt != capabilities.end(); capIt++) {
        modCap.capabilities.insert( *capIt );
    }    
    foreignCapabilities_.push_back( modCap );
    pthread_mutex_unlock(&mutex_);
    return true;    
}

bool CapabilitiesManager::getCapabilityById(long capabilityID, SourceData from, CapabilityDefinition& capability){
    
    pthread_mutex_lock(&mutex_);
    if ((from == SourceData_Local) || (from == SourceData_All)) {
        for (CapabilitiesSet::iterator it = capabilities_.begin(); it != capabilities_.end(); it++){
            if ( it->getId() == capabilityID ) {
                capability = *it;
                pthread_mutex_unlock(&mutex_);
                return true;
            }
        }
    } else if ((from == SourceData_Foreign) || (from == SourceData_All)) {
        for (ModuleCapabilitiesList::iterator it = foreignCapabilities_.begin(); it != foreignCapabilities_.end(); it++){
            CapabilitiesSet set = it->capabilities;
            for (CapabilitiesSet::iterator itcap = set.begin(); itcap != set.end(); itcap++){
                if ( itcap->getId() == capabilityID ) {
                    capability = *itcap;
                    pthread_mutex_unlock(&mutex_);
                    return true;
                }
            }            
        }        
    }
    pthread_mutex_unlock(&mutex_);
    return false;
}

bool CapabilitiesManager::getForeignCapabilityById(long capabilityID, CapabilityDefinition& capability, Ident& module){
    
    pthread_mutex_lock(&mutex_);
    for (ModuleCapabilitiesList::iterator it = foreignCapabilities_.begin(); it != foreignCapabilities_.end(); it++){
        CapabilitiesSet set = it->capabilities;
        for (CapabilitiesSet::iterator itcap = set.begin(); itcap != set.end(); itcap++){
            if ( itcap->getId() == capabilityID ) {
                capability = *itcap;
                module = it->module;
                pthread_mutex_unlock(&mutex_);
                return true;
            }
        }            
    }
    pthread_mutex_unlock(&mutex_);
    return false;
}

void CapabilitiesManager::getCapabilitiesByType(CapabilitiesTypes type, SourceData from, CapabilitiesList& capabilities){
    
    pthread_mutex_lock(&mutex_);
    capabilities.clear();
    if ((from == SourceData_Local) || (from == SourceData_All)) {
        for (CapabilitiesSet::iterator it = capabilities_.begin(); it != capabilities_.end(); it++){
            if ( (type == it->getType()) || (type == Capability_Undefined) ) {
                capabilities.push_back( *it );
            }
        }
    }
    
    if ((from == SourceData_Foreign) || (from == SourceData_All)) {
        for (ModuleCapabilitiesList::iterator it = foreignCapabilities_.begin(); it != foreignCapabilities_.end(); it++){
            CapabilitiesSet set = it->capabilities;
            for (CapabilitiesSet::iterator itcap = set.begin(); itcap != set.end(); itcap++){
                if ( (type == itcap->getType()) || (type == Capability_Undefined) ) {
                    capabilities.push_back( *itcap );
                }
            }
        }      
    }
    pthread_mutex_unlock(&mutex_);
}