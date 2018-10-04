/* 
 * File:   GatewayDataStorage.h
 * Author: cleo
 *
 * Created on September 21, 2014, 2:07 AM
 */

#ifndef GATEWAYDATASTORAGE_H
#define	GATEWAYDATASTORAGE_H

#include <support/GlobalDefinitions.h>

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/UserValidationResp.h>
#include <synapse/UserSecurityLevelQueryResp.h>

#define GATEWAY_STORAGE GatewayDataStorage::getInstance()

class GatewayDataStorage : public SingletonBase<GatewayDataStorage> {
public:
    friend class SingletonBase<GatewayDataStorage>;

    SetValueResult setValue(CapabilityDefinition capability, bool value);
    GetDataResult getValue(long capabilityID, bool& value);

    SetValueResult setValue(CapabilityDefinition capability, long value);
    GetDataResult getValue(long capabilityID, long& value);
    
    void setUserSecurityLevel(std::string userHash, long level);
    bool getUserSecurityLevel(std::string userHash, long& level);    
    
    void setUserHash(std::string value) { userHash_ = value; }
    std::string getUserHash() { return userHash_; }
    
private:
    GatewayDataStorage();
    virtual ~GatewayDataStorage() throw();
    
    MAP_ANALOG_VALUES        analogValuesMap_;
    MAP_DIGITAL_VALUES       digitalValuesMap_;
    
    MAP_USERS_LEVELS         usersSecurityLevels_;
    
    std::string              userHash_;
    
    pthread_mutex_t     mutex_;
    
};

#endif	/* GATEWAYDATASTORAGE_H */

