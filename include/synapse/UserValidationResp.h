/* 
 * File:   UserValidationResp.h
 * Author: cleo
 *
 * Created on July 16, 2014, 11:55 AM
 */

#ifndef USERVALIDATIONRESP_H
#define	USERVALIDATIONRESP_H

#include <synapse/GetServiceDataResp.h>

using namespace Synapse;

class UserValidationResp : public GetServiceDataResp {	
public:    
    UserValidationResp(ServiceDefinition service, ValidationResult result, std::string user, std::string pwd, std::string hash);
    UserValidationResp(Header& header) : GetServiceDataResp(header) {};
    virtual ~UserValidationResp(){};
    
    ValidationResult getResult() { return result_; } 
    
    std::string getUser() { return user_; }     
    
    std::string getPwd() { return pwd_; }     
    
    std::string getHash() { return userHash_; }     
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    ValidationResult            result_; 
    std::string                 user_;
    std::string                 pwd_;
    std::string                 userHash_;
};

#endif	/* USERVALIDATIONRESP_H */

