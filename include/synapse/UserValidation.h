/* 
 * File:   UserValidation.h
 * Author: cleo
 *
 * Created on July 7, 2014, 9:46 PM
 */

#ifndef USERVALIDATION_H
#define	USERVALIDATION_H

#include <synapse/GetServiceData.h>

class UserValidation : public GetServiceData {	
public:    
    UserValidation(ServiceDefinition service, std::string user, std::string pwd);
    UserValidation(Header& header) : GetServiceData(header) {};
    virtual ~UserValidation(){};
    
    std::string getUser() { return user_; } 
    std::string getPwd() { return pwd_; } 
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    std::string                 user_;
    std::string                 pwd_;    
};

#endif	/* USERVALIDATION_H */

