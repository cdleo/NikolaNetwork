/* 
 * File:   BusinessLogicManager.h
 * Author: cleo
 *
 * Created on August 14, 2014, 11:35 AM
 */

#ifndef BUSINESSLOGICMANAGER_H
#define	BUSINESSLOGICMANAGER_H

#include <GlobalDefinitions.h>
#include <SingletonBase.h>

#include <synapse/Messages.h>

#define LOGIC BusinessLogicManager::getInstance()

class BusinessLogicManager : public SingletonBase<BusinessLogicManager> {
public:
    friend class SingletonBase<BusinessLogicManager>;

    void SetAnalogValue_Request(SetAnalogValue* request, std::string peerID);
    void SetDigitalValue_Request(SetDigitalValue* request, std::string peerID);
    void GetAnalogValue_Request(GetAnalogValue* request, std::string peerID);
    void GetDigitalValue_Request(GetDigitalValue* request, std::string peerID);    
    
    void Subscription_Request(Subscribe* request, std::string peerID);
    
    void Revoke_Request(RevokeSubscription* request, std::string peerID);
    
    void OnUserSecurityLevelReceived(ValidationResult result, std::string userHash, int securityLevel);
    
private:
    BusinessLogicManager();
    virtual ~BusinessLogicManager() throw();    
    
};

#endif	/* BUSINESSLOGICMANAGER_H */

