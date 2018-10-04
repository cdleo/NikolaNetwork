/* 
 * File:   UserSecurityLevelQuery.h
 * Author: cleo
 *
 * Created on July 7, 2014, 9:46 PM
 */

#ifndef USERSECURITYLEVELQUERY_H
#define	USERSECURITYLEVELQUERY_H

#include <synapse/GetServiceData.h>

class UserSecurityLevelQuery : public GetServiceData {	
public:    
    UserSecurityLevelQuery(ServiceDefinition service, std::string userHash);
    UserSecurityLevelQuery(Header& header) : GetServiceData(header) {};
    virtual ~UserSecurityLevelQuery(){};
        
    std::string getUserHash() { return userHash_; } 
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    std::string                 userHash_;
  
};

#endif	/* USERSECURITYLEVELQUERY_H */

