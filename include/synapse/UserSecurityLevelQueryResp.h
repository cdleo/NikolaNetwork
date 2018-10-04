/* 
 * File:   UserSecurityLevelQueryResp.h
 * Author: cleo
 *
 * Created on July 16, 2014, 11:55 AM
 */

#ifndef USERSECURITYLEVELQUERYRESP_H
#define	USERSECURITYLEVELQUERYRESP_H

#include <synapse/GetServiceDataResp.h>

using namespace Synapse;

class UserSecurityLevelQueryResp : public GetServiceDataResp {	
public:    
    UserSecurityLevelQueryResp(ServiceDefinition service, ValidationResult result, std::string hash, int securityLevel);
    UserSecurityLevelQueryResp(Header& header) : GetServiceDataResp(header) {};
    virtual ~UserSecurityLevelQueryResp(){};
    
    ValidationResult getResult() { return result_; } 
    
    std::string getHash() { return userHash_; }     
    
    int getSecurityLevel() { return securityLevel_; }
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    ValidationResult            result_; 
    std::string                 userHash_;
    int                         securityLevel_;
};

#endif	/* USERSECURITYLEVELQUERYRESP_H */

