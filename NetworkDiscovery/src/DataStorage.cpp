/* 
 * File:   DataStorage.cpp
 * Author: cleo
 * 
 * Created on July 25, 2014, 3:23 PM
 */

#include <DataStorage.h>
#include <support/ServicesManager.h>

DataStorage::DataStorage() {
    pthread_mutex_init(&mutex_, NULL);
}

DataStorage::~DataStorage() throw() {
    pthread_mutex_destroy(&mutex_);
}

bool DataStorage::initialize(){
    networkMap.clear();
    userHash_.assign("");
    return true;
} 

void DataStorage::setNetworkMap(MAP_NEIGHBOR newMap) {
    pthread_mutex_lock(&mutex_);
    networkMap = newMap; 
    pthread_mutex_unlock(&mutex_);
} 

GetServiceDataResp* DataStorage::getData( ServiceDefinition def ){

    if (!SERVICES->existService(def))
        return NULL;
    
    switch (def.getId()){
        case ID_FULLNETWORK_SERVICE:
            return GetFullNetworkMap( def );
                    
        default:
            return NULL;
    }
    
}

GetServiceDataResp* DataStorage::GetFullNetworkMap( ServiceDefinition service ) {

    pthread_mutex_lock(&mutex_);
    
    GetDataResult result = GetDataResult_Ok;
    if (networkMap.size() == 0) 
        result = GetDataResult_NoDataAvailable;
    
    NetworkResp* msg = new NetworkResp( service, result );
    for (MAP_NEIGHBOR::iterator it = networkMap.begin(); it != networkMap.end(); it++) {
        if ((it->second.lastStatus == HostStatus_BecomeOnline) || (it->second.lastStatus == HostStatus_Online)) {
            NetworkItem item(it->first.c_str());
            msg->addItem( item );
        }
    }
    
    pthread_mutex_unlock(&mutex_);
    
    return msg;
}