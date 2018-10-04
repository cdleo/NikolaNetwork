/* 
 * File:   ServicesManager.h
 * Author: cleo
 *
 * Created on August 13, 2014, 11:33 PM
 */

#ifndef SERVICESMANAGER_H
#define	SERVICESMANAGER_H

#include <SingletonBase.h>

#include <synapse/ServicesResp.h>
#include <synapse/items/Ident.h>

#include <set>

#define SERVICES ServicesManager::getInstance()

typedef std::set<ServiceDefinition>       ServicesSet;

typedef struct ModuleServices {
    Ident           module;
    ServicesSet     services;
} ModuleServices;

typedef std::vector<ModuleServices>  ModuleServicesList;

class ServicesManager : public SingletonBase<ServicesManager> {
public:
    friend class SingletonBase<ServicesManager>;

    void load();
    
    ServicesList getLocalServices() {
        ServicesList list;
        std::copy(services_.begin(), services_.end(), std::back_inserter(list));
        
        return list;        
    }

    ModuleServicesList& getWideServices() {
        return foreignServices_;
    }      

    void addLocalService(ServiceDefinition service);
    bool addForeignService(Ident ident, ServiceDefinition service);
    bool addForeignServices(Ident ident, ServicesList& services);     

    bool getServiceById(long id, SourceData from, ServiceDefinition& service);
    bool getForeignServiceByType(ServiceTypes type, ServiceDefinition& service, Ident& module);
    void getServicesByType(ServiceTypes type, SourceData from, ServicesList& services);
    
    bool existService(ServiceDefinition service);
     
private:
    ServicesManager();
    virtual ~ServicesManager() throw();
    
    ServicesSet                 services_;
    ModuleServicesList          foreignServices_;
    
    pthread_mutex_t             mutex_;
};

#endif	/* SERVICESMANAGER_H */



