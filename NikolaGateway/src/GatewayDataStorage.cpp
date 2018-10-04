/* 
 * File:   GatewayDataStorage.cpp
 * Author: cleo
 * 
 * Created on September 21, 2014, 2:07 AM
 */

#include <GatewayDataStorage.h>
#include <GatewaySubscriptorsManager.h>

#include <synapse/AnalogValueUpdate.h>
#include <synapse/DigitalValueUpdate.h>

GatewayDataStorage::GatewayDataStorage() {
    pthread_mutex_init(&mutex_, NULL);
}

GatewayDataStorage::~GatewayDataStorage() throw() {
    pthread_mutex_destroy(&mutex_);
    analogValuesMap_.clear();
    digitalValuesMap_.clear();
}

SetValueResult GatewayDataStorage::setValue(CapabilityDefinition capability, bool value) {
    pthread_mutex_lock(&mutex_);
    bool valueChanged = true;
    if (digitalValuesMap_.find(capability.getId()) == digitalValuesMap_.end()){
        digitalValuesMap_.insert( std::pair<long,bool>(capability.getId(), value) );
    } else {
        valueChanged = (digitalValuesMap_[capability.getId()] != value);
        digitalValuesMap_[capability.getId()] = value;
    }
    pthread_mutex_unlock(&mutex_);
    
    if (valueChanged){
        DigitalValueUpdate packet(capability, value);
        GATEWAY_SUBSCRIPTORS->onCapabilityUpdate( capability.getId(), packet );
    }
    return SetValueResult_Ok;
}


GetDataResult GatewayDataStorage::getValue(long capabilityID, bool& value) {
    
    GetDataResult result = GetDataResult_NoDataAvailable;
    pthread_mutex_lock(&mutex_);
    if (digitalValuesMap_.find(capabilityID) != digitalValuesMap_.end()) {
        value = digitalValuesMap_.at(capabilityID);
        result = GetDataResult_Ok;
    }
    pthread_mutex_unlock(&mutex_);
    return result;
}


SetValueResult GatewayDataStorage::setValue(CapabilityDefinition capability, long value) {
    bool valueChanged = true;
    pthread_mutex_lock(&mutex_);
    if (analogValuesMap_.find(capability.getId()) == analogValuesMap_.end()){
        analogValuesMap_.insert( std::pair<long,long>(capability.getId(), value) );
    } else {
        valueChanged = (analogValuesMap_[capability.getId()] != value);
        analogValuesMap_[capability.getId()] = value;
    }    
    pthread_mutex_unlock(&mutex_);
    
    if (valueChanged){
        AnalogValueUpdate packet(capability, value);
        GATEWAY_SUBSCRIPTORS->onCapabilityUpdate( capability.getId(), packet);
    }
    return SetValueResult_Ok;
}

GetDataResult GatewayDataStorage::getValue(long capabilityID, long& value) {
    
    GetDataResult result = GetDataResult_NoDataAvailable;
    pthread_mutex_lock(&mutex_);
    if (analogValuesMap_.find(capabilityID) == analogValuesMap_.end()) {
        value = analogValuesMap_.at(capabilityID);
        result = GetDataResult_Ok;
    }
    pthread_mutex_unlock(&mutex_);
    return result;
}

void GatewayDataStorage::setUserSecurityLevel(std::string userHash, long level) { 
    if (usersSecurityLevels_.find(userHash) != usersSecurityLevels_.end()) {
        usersSecurityLevels_.at(userHash) = level;
    } else {        
        usersSecurityLevels_.insert( std::pair<std::string,long>(userHash,level) ); 
    }
}

bool GatewayDataStorage::getUserSecurityLevel(std::string userHash, long& level) { 
    if (usersSecurityLevels_.find(userHash) != usersSecurityLevels_.end()) {
        level = usersSecurityLevels_.at(userHash);
        return true;
    }  
    return false; 
}
