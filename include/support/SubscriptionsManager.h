/* 
 * File:   SubscriptionsManager.h
 * Author: cleo
 *
 * Clase utilizada para almacenar MIS subscripciones a capabilities de OTROS peers
 * 
 * Created on July 16, 2014, 1:40 PM
 */

#ifndef SUBSCRIPTIONSMANAGER_H
#define	SUBSCRIPTIONSMANAGER_H

#include <SingletonBase.h>

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/GlobalDefinitions.h>
#include <synapse/SynapseConnector.h>

#include <support/ConnectionsManager.h>
#include <support/GlobalDefinitions.h>

using namespace Synapse;

#define SUBSCRIPTIONS SubscriptionsManager::getInstance()

class SubscriptionsManager : public SingletonBase<SubscriptionsManager> {
public:
    friend class SingletonBase<SubscriptionsManager>;
    
    bool initialize(ConnectionsManager* connections);
    
    bool addSubscription( CapabilityDefinition capability, std::string peerID );
    
    bool existsSubscription( long capabilityID );
    
    UnsubscribeResult removeSubscription(CapabilityDefinition capability );

private:
    SubscriptionsManager();
    virtual ~SubscriptionsManager() throw();
    
    ConnectionsManager* connections_;    
    
    SUBSCRIPTIONS_MAP   subscriptions_;
    
    pthread_mutex_t     mutex_;  
};

#endif	/* SUBSCRIPTIONSMANAGER_H */

