/* 
 * File:   GatewaySubscriptorsManager.h
 * Author: cleo
 *
 * Clase utilizada para almacenar a los peers que estan subscriptos a las capabilities 
 * del sistema, a través del gateway
 * 
 * Created on September 19, 2014, 12:19 PM
 */

#ifndef GATEWAYSUBSCRIPTORSMANAGERS_H
#define	GATEWAYSUBSCRIPTORSMANAGERS_H

#include <SingletonBase.h>

#include <GlobalDefinitions.h>

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/UnsubscribeResp.h>

#define GATEWAY_SUBSCRIPTORS GatewaySubscriptorsManager::getInstance()

class GatewaySubscriptorsManager : public SingletonBase<GatewaySubscriptorsManager> {
public:
    friend class SingletonBase<GatewaySubscriptorsManager>;
    
    bool initialize();
    
    long addSubscription( long capabilityID, std::string userHash, std::string gatewayPeerID );
    
    bool setSubscriptionStatus( long capabilityID, std::string userHash, long status);
    
    bool getGatewayPeersSubscribed( long capabilityID, std::string userHash, std::set<std::string>& gatewayPeers);
    
    bool removeSubscription( long capabilityID, std::string userHash);
    
    bool revokeSubscriptions (CapabilityDefinition capability, std::string revokedUserHash );
    
    void onCapabilityUpdate( long capabilityID, Packet& message );
    
private:
    GatewaySubscriptorsManager();
    virtual ~GatewaySubscriptorsManager() throw();
        
    GATEWAY_SUBSCRIPTORS_MAP    subscriptors_;
    
    pthread_mutex_t             mutex_;  
    
};

#endif	/* GATEWAYSUBSCRIPTORSMANAGERS_H */

