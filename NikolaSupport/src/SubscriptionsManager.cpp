/* 
 * File:   SubscriptionsManager.cpp
 * Author: cleo
 * 
 * Created on July 16, 2014, 1:40 PM
 */

#include <support/SubscriptionsManager.h>
#include <support/ConnectionsManager.h>

SubscriptionsManager::SubscriptionsManager() 
: connections_(NULL) {
    subscriptions_.clear();
    pthread_mutex_init(&mutex_, NULL);
}

SubscriptionsManager::~SubscriptionsManager() throw() {
    subscriptions_.clear();
    pthread_mutex_destroy(&mutex_);
}

bool SubscriptionsManager::initialize(ConnectionsManager* connections){
    
    connections_ = connections;
    return (connections_ != NULL);
}

bool SubscriptionsManager::addSubscription(CapabilityDefinition capability, std::string peerID) {
    
    pthread_mutex_lock(&mutex_);
    if (subscriptions_.find(capability.getId()) != subscriptions_.end()){
        pthread_mutex_unlock(&mutex_);
        return false;
    }
    subscriptions_.insert( std::pair<long,std::string>( capability.getId(), peerID ) );
    pthread_mutex_unlock(&mutex_);
    return true;
}

bool SubscriptionsManager::existsSubscription( long capabilityID ) {
    pthread_mutex_lock(&mutex_);
    bool result = (subscriptions_.find(capabilityID) != subscriptions_.end());
    pthread_mutex_unlock(&mutex_);
    return result;
}

UnsubscribeResult SubscriptionsManager::removeSubscription(CapabilityDefinition capability ) {

    pthread_mutex_lock(&mutex_);
    if (subscriptions_.find(capability.getId()) == subscriptions_.end()){
        pthread_mutex_unlock(&mutex_);
        return UnsubscribeResult_NotSubscribedToCapability;
    }
    subscriptions_.erase(capability.getId());
    pthread_mutex_unlock(&mutex_);
    return UnsubscribeResult_Ok;    
}

