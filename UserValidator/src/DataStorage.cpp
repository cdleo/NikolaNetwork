/* 
 * File:   DataStorage.cpp
 * Author: cleo
 * 
 * Created on July 25, 2014, 3:23 PM
 */

#include <DataStorage.h>
#include <Parameters.h>
#include <DatabaseManager.h>

#include <synapse/UserValidation.h>
#include <synapse/UserSecurityLevelQueryResp.h>

#include <boost/functional/hash.hpp>

DataStorage::DataStorage() {
}

DataStorage::~DataStorage() throw() {
}

bool DataStorage::initialize() {
    std::string usr, pwd;
    PARAMETERS->getValue("user", usr);
    PARAMETERS->getValue("password", pwd);   
    
    return (this->getUserHash(usr, pwd, localUserHash_) == ValidationResult_Ok);
}

ValidationResult DataStorage::getUserHash( std::string user, std::string pwd, std::string& hash ){

    int securityLevel = -1;
    ValidationResult result = DB->loadUser(user,pwd,securityLevel);
    if (result == ValidationResult_Ok) {
        hash = makeUserHash(user,pwd,securityLevel);
        if (usersMap.find(hash) == usersMap.end()) {
            usersMap.insert( std::pair<std::string,int>(hash, securityLevel) );
        }
    } else {
        hash = "";
    }
    return result;
}

ValidationResult DataStorage::getUserSecurityLevel( std::string userHash, int& securityLevel ){

    if (usersMap.find(userHash) == usersMap.end()){
        securityLevel = -1;
        return ValidationResult_InvalidHash;
    }
    securityLevel = usersMap.at(userHash);
    return ValidationResult_Ok;
}

std::string DataStorage::makeUserHash(std::string user, std::string pwd, int securityLevel) {
    
    std::size_t h1 = boost::hash<std::string>()(user);
    std::size_t h2 = boost::hash<std::string>()(pwd);
    std::size_t h3 = boost::hash<int>()(securityLevel);
    
    std::size_t r1 = h1 ^ (h2 << 1);
    std::size_t r2 = r1 ^ (h3 << 1);
    
    std::ostringstream oss;
    oss << r2;
    return oss.str();
}