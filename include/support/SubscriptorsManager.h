/* 
 * File:   SubscriptorsManager.h
 * Author: cleo
 *
 * Clase utilizada para almacenar a los peers que estan subscriptos a MIS capabilities 
 * 
 * Created on July 16, 2014, 1:40 PM
 */

#ifndef SUBSCRIPTORSMANAGER_H
#define	SUBSCRIPTORSMANAGER_H

#include <SingletonBase.h>

#include <synapse/GlobalDefinitions.h>
#include <synapse/SynapseConnector.h>

#include <support/ConnectionsManager.h>
#include <support/GlobalDefinitions.h>

using namespace Synapse;

#define SUBSCRIPTORS SubscriptorsManager::getInstance()

class SubscriptorsManager : public SingletonBase<SubscriptorsManager> {
public:
    friend class SingletonBase<SubscriptorsManager>;
    
    bool initialize(ConnectionsManager* connections);
    
    SubscribeResult addSubscription( long capabilityID, std::string userHash, std::string peerID );
    
    bool isSubscribed( long capabilityID, std::string peerID, std::string& subscribedUserHash );
    
    UnsubscribeResult removeSubscription(long capabilityID, std::string userHash, std::string peerID );
    
    void removeSubscriptions( std::string peerID );
    
    void onCapabilityUpdate( long capabilityID, Packet& message );
    
private:
    SubscriptorsManager();
    virtual ~SubscriptorsManager() throw();
    
    ConnectionsManager* connections_;
    
    SUBSCRIPTORS_MAP    subscriptors_;
    
    pthread_mutex_t     mutex_;  
    
};

#endif	/* SUBSCRIPTORSMANAGER_H */

