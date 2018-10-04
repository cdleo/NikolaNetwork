/* 
 * File:   DataStorage.h
 * Author: cleo
 *
 * Created on July 25, 2014, 3:23 PM
 */

#ifndef DATASTORAGE_H
#define	DATASTORAGE_H

#include <GlobalDefinitions.h>
#include <synapse/UserValidationResp.h>
#include <synapse/UserSecurityLevelQueryResp.h>

#define CUSTOM_STORAGE DataStorage::getInstance()

class DataStorage : public SingletonBase<DataStorage> {
public:
    friend class SingletonBase<DataStorage>;

    bool initialize();
    
    ValidationResult getUserHash( std::string user, std::string pwd, std::string& hash );
    
    ValidationResult getUserSecurityLevel( std::string userHash, int& securityLevel );
    
    void setLocalUserHash(std::string value) { localUserHash_ = value; }
    std::string getLocalUserHash() { return localUserHash_; }     
    
private:
    DataStorage();
    virtual ~DataStorage() throw();
    
    std::string makeUserHash(std::string user, std::string pwd, int securityLevel);
    
    MAP_USERS           usersMap;
    std::string         localUserHash_;
};

#endif	/* DATASTORAGE_H */

