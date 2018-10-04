/* 
 * File:   DataStorage.h
 * Author: cleo
 *
 * Created on July 25, 2014, 3:23 PM
 */

#ifndef DATASTORAGE_H
#define	DATASTORAGE_H

#include <support/GlobalDefinitions.h>
#include <synapse/UserValidationResp.h>
#include <synapse/UserSecurityLevelQueryResp.h>

#define STORAGE DataStorage::getInstance()

class DataStorage : public SingletonBase<DataStorage> {
public:
    friend class SingletonBase<DataStorage>;

    SetValueResult setValue(long capabilityID, bool value);
    GetDataResult getValue(long capabilityID, bool& value);

    SetValueResult setValue(long capabilityID, long value);
    GetDataResult getValue(long capabilityID, long& value);
    
    void setUserSecurityLevel(std::string userHash, long level);
    bool getUserSecurityLevel(std::string userHash, long& level);    
    
    void setUserHash(std::string value) { userHash_ = value; }
    std::string getUserHash() { return userHash_; }
    
private:
    DataStorage();
    virtual ~DataStorage() throw();
    
    MAP_ANALOG_VALUES        analogValuesMap_;
    MAP_DIGITAL_VALUES       digitalValuesMap_;
    
    MAP_USERS_LEVELS         usersSecurityLevels_;
    
    std::string              userHash_;
    
    pthread_mutex_t     mutex_;
    
};

#endif	/* DATASTORAGE_H */

