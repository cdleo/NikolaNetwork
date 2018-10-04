/* 
 * File:   DataStorage.h
 * Author: cleo
 *
 * Created on July 25, 2014, 3:23 PM
 */

#ifndef DATASTORAGE_H
#define	DATASTORAGE_H

#include <GlobalDefinitions.h>
#include <synapse/NetworkResp.h>

#define CUSTOM_STORAGE DataStorage::getInstance()

class DataStorage : public SingletonBase<DataStorage> {
public:
    friend class SingletonBase<DataStorage>;

    bool initialize();
    
    GetServiceDataResp* getData( ServiceDefinition def );
    
    MAP_NEIGHBOR getNetworkMap() { return networkMap; }
    void setNetworkMap(MAP_NEIGHBOR newMap); 
    
    void setUserHash(std::string value) { userHash_ = value; }
    std::string getUserHash() { return userHash_; }    
    
private:
    DataStorage();
    virtual ~DataStorage() throw();
    
    GetServiceDataResp* GetFullNetworkMap( ServiceDefinition capability );
    
    MAP_NEIGHBOR        networkMap;
    std::string         userHash_;
    pthread_mutex_t     mutex_;
};

#endif	/* DATASTORAGE_H */

