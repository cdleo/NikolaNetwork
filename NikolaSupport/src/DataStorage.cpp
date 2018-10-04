/* 
 * File:   DataStorage.cpp
 * Author: cleo
 * 
 * Created on July 25, 2014, 3:23 PM
 */

#include <support/DataStorage.h>
#include <support/ActionsManager.h>
#include <support/CapabilitiesManager.h>
#include <support/SubscriptorsManager.h>

#include <synapse/AnalogValueUpdate.h>
#include <synapse/DigitalValueUpdate.h>

DataStorage::DataStorage() {
    pthread_mutex_init(&mutex_, NULL);
}

DataStorage::~DataStorage() throw() {
    pthread_mutex_destroy(&mutex_);
    analogValuesMap_.clear();
    digitalValuesMap_.clear();
}

SetValueResult DataStorage::setValue(long capabilityID, bool value) {
    pthread_mutex_lock(&mutex_);
    bool valueChanged = true;
    if (digitalValuesMap_.find(capabilityID) == digitalValuesMap_.end()){
        digitalValuesMap_.insert( std::pair<long,bool>(capabilityID, value) );
    } else {
        valueChanged = (digitalValuesMap_[capabilityID] != value);
        digitalValuesMap_[capabilityID] = value;
    }
    pthread_mutex_unlock(&mutex_);
    
    if (valueChanged){
        CapabilityDefinition def;
        if (CAPABILITIES->getCapabilityById(capabilityID, SourceData_Local, def)){
            DigitalValueUpdate packet(def, value);
            SUBSCRIPTORS->onCapabilityUpdate( capabilityID, packet );
            ACTIONS->onChange(capabilityID, value); 
        }
    }
    return SetValueResult_Ok;
}


GetDataResult DataStorage::getValue(long capabilityID, bool& value) {
    
    GetDataResult result = GetDataResult_NoDataAvailable;
    pthread_mutex_lock(&mutex_);
    if (digitalValuesMap_.find(capabilityID) != digitalValuesMap_.end()) {
        value = digitalValuesMap_.at(capabilityID);
        result = GetDataResult_Ok;
    }
    pthread_mutex_unlock(&mutex_);
    return result;
}


SetValueResult DataStorage::setValue(long capabilityID, long value) {
    bool valueChanged = true;
    pthread_mutex_lock(&mutex_);
    if (analogValuesMap_.find(capabilityID) == analogValuesMap_.end()){
        analogValuesMap_.insert( std::pair<long,long>(capabilityID, value) );
    } else {
        valueChanged = (analogValuesMap_[capabilityID] != value);
        analogValuesMap_[capabilityID] = value;
    }    
    pthread_mutex_unlock(&mutex_);
    
    if (valueChanged){
        CapabilityDefinition def;
        if (CAPABILITIES->getCapabilityById(capabilityID, SourceData_Local, def)){
            AnalogValueUpdate packet(def, value);
            SUBSCRIPTORS->onCapabilityUpdate( capabilityID, packet);
            ACTIONS->onChange(capabilityID, value); 
        }
    }
    return SetValueResult_Ok;
}

GetDataResult DataStorage::getValue(long capabilityID, long& value) {
    
    GetDataResult result = GetDataResult_NoDataAvailable;
    pthread_mutex_lock(&mutex_);
    if (analogValuesMap_.find(capabilityID) != analogValuesMap_.end()) {
        value = analogValuesMap_.at(capabilityID);
        result = GetDataResult_Ok;
    }
    pthread_mutex_unlock(&mutex_);
    return result;
}

void DataStorage::setUserSecurityLevel(std::string userHash, long level) { 
    if (usersSecurityLevels_.find(userHash) != usersSecurityLevels_.end()) {
        usersSecurityLevels_.at(userHash) = level;
    } else {        
        usersSecurityLevels_.insert( std::pair<std::string,long>(userHash,level) ); 
    }
}

bool DataStorage::getUserSecurityLevel(std::string userHash, long& level) { 
    if (usersSecurityLevels_.find(userHash) != usersSecurityLevels_.end()) {
        level = usersSecurityLevels_.at(userHash);
        return true;
    }  
    return false; 
}