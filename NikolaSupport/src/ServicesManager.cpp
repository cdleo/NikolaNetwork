/* 
 * File:   ServicesManager.cpp
 * Author: cleo
 * 
 * Created on August 13, 2014, 11:33 PM
 */

#include <support/ServicesManager.h>
#include <Parameters.h>
#include <BaseUtilities.h>
#include <set>

ServicesManager::ServicesManager() {
    pthread_mutex_init(&mutex_,NULL);
    services_.clear();
}

ServicesManager::~ServicesManager() throw() {
    services_.clear();
    pthread_mutex_destroy(&mutex_);
}

void ServicesManager::load(){
    
    pthread_mutex_lock(&mutex_);
    std::vector<std::string> services;
    PARAMETERS->getValue("services", services);    
    for ( std::vector<std::string>::iterator it = services.begin(); it != services.end(); it++ ){
        std::vector<std::string> splitedService = UTILITIES->split( *it, '|' );
        if (splitedService.size() != 3) continue;
    
        ServiceDefinition def;
        def.setId( atol(splitedService.at(0).c_str()) );
        def.setType( (ServiceTypes) atol(splitedService.at(1).c_str()) );
        def.setDescription( splitedService.at(2) );
        services_.insert(def);          
    }
    pthread_mutex_unlock(&mutex_);
}

bool ServicesManager::existService(ServiceDefinition service){
    bool result = false;
    pthread_mutex_lock(&mutex_);
    result = (services_.find(service) != services_.end());
    pthread_mutex_unlock(&mutex_);
    return result;
}

void ServicesManager::addLocalService(ServiceDefinition service) {
    pthread_mutex_lock(&mutex_);
    services_.insert(service);   
    pthread_mutex_unlock(&mutex_);
}

bool ServicesManager::addForeignService(Ident ident, ServiceDefinition service){
    
    pthread_mutex_lock(&mutex_);
    for (ModuleServicesList::iterator it = foreignServices_.begin(); it != foreignServices_.end(); it++){
        if ( it->module.equals( ident ) ) {
            ServicesSet servSet = it->services;
            servSet.insert( service );
            it->services = servSet;
            pthread_mutex_unlock(&mutex_);
            return true;
        }
    }
    
    ModuleServices moServ;
    moServ.module = ident;
    moServ.services.insert( service );
    foreignServices_.push_back( moServ );
    pthread_mutex_unlock(&mutex_);
    return true;
}

bool ServicesManager::addForeignServices(Ident ident, ServicesList& services){
    
    pthread_mutex_lock(&mutex_);
    for (ModuleServicesList::iterator it = foreignServices_.begin(); it != foreignServices_.end(); it++){
        if ( it->module.equals( ident ) ) {
            ServicesSet servSet = it->services;
            for (ServicesList::iterator servIt = services.begin(); servIt != services.end(); servIt++) {
                servSet.insert( *servIt );
            }
            it->services = servSet;
            pthread_mutex_unlock(&mutex_);
            return true;
        }
    }
    
    ModuleServices modServ;
    modServ.module = ident;
    for (ServicesList::iterator servIt = services.begin(); servIt != services.end(); servIt++) {
        modServ.services.insert( *servIt );
    }
    foreignServices_.push_back( modServ );     
    pthread_mutex_unlock(&mutex_);
    return true;    
}

bool ServicesManager::getServiceById(long id, SourceData from, ServiceDefinition& service){
    
    pthread_mutex_lock(&mutex_);
    if ((from == SourceData_Local) || (from == SourceData_All)) {
        for (ServicesSet::iterator it = services_.begin(); it != services_.end(); it++){
            if ( it->getId() == id ) {
                service = *it;
                pthread_mutex_unlock(&mutex_);
                return true;
            }
        }
    } else if ((from == SourceData_Foreign) || (from == SourceData_All)) {
        for (ModuleServicesList::iterator it = foreignServices_.begin(); it != foreignServices_.end(); it++){
            ServicesSet set = it->services;
            for (ServicesSet::iterator itserv = set.begin(); itserv != set.end(); itserv++){
                if ( itserv->getId() == id ) {
                    service = *itserv;
                    pthread_mutex_unlock(&mutex_);
                    return true;
                }
            }            
        }        
    }
    pthread_mutex_unlock(&mutex_);
    return false;    
}

bool ServicesManager::getForeignServiceByType(ServiceTypes type, ServiceDefinition& service, Ident& module){
    pthread_mutex_lock(&mutex_);
    for (ModuleServicesList::iterator it = foreignServices_.begin(); it != foreignServices_.end(); it++){
        ServicesSet set = it->services;
        for (ServicesSet::iterator itserv = set.begin(); itserv != set.end(); itserv++){
            if ( itserv->getType() == type ) {
                service = *itserv;
                module = it->module;
                pthread_mutex_unlock(&mutex_);
                return true;
            }
        }            
    }
    pthread_mutex_unlock(&mutex_);
    return false;    
}

void ServicesManager::getServicesByType(ServiceTypes type, SourceData from, ServicesList& services) {
    
    pthread_mutex_lock(&mutex_);
    services.clear();
    if ((from == SourceData_Local) || (from == SourceData_All)) {
        for (ServicesSet::iterator it = services_.begin(); it != services_.end(); it++){
            if ( (type == it->getType()) || (type == ServiceType_Undefined) ) {
                services.push_back( *it );
            }
        }
    }
    
    if ((from == SourceData_Foreign) || (from == SourceData_All)) {
        for (ModuleServicesList::iterator it = foreignServices_.begin(); it != foreignServices_.end(); it++){
            ServicesSet set = it->services;
            for (ServicesSet::iterator itserv = set.begin(); itserv != set.end(); itserv++){
                if ( (type == itserv->getType()) || (type == ServiceType_Undefined) ) {
                    services.push_back( *itserv );
                }
            }
        }      
    }
    pthread_mutex_unlock(&mutex_);    
   
}
