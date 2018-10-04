/* 
 * File:   SubscriptorsManager.cpp
 * Author: cleo
 * 
 * Created on July 16, 2014, 1:40 PM
 */

#include <support/SubscriptorsManager.h>
#include <support/ConnectionsManager.h>

#include <synapse/GlobalDefinitions.h>

SubscriptorsManager::SubscriptorsManager() 
: connections_(NULL) {
    subscriptors_.clear();
    pthread_mutex_init(&mutex_, NULL);
}

SubscriptorsManager::~SubscriptorsManager() throw() {
    subscriptors_.clear();
    pthread_mutex_destroy(&mutex_);
}

bool SubscriptorsManager::initialize(ConnectionsManager* connections){
    
    connections_ = connections;
    return (connections_ != NULL);
}

SubscribeResult SubscriptorsManager::addSubscription(long capabilityID, std::string userHash, std::string peerID) {
    
    try {
        pthread_mutex_lock(&mutex_);
        SubscriptionData data( peerID, userHash );            
        SUBSCRIPTORS_LIST subscriptors;
        SUBSCRIPTORS_MAP::iterator it = subscriptors_.find(capabilityID);
        if (it == subscriptors_.end()){
            subscriptors.push_back( data );
            subscriptors_.insert( std::pair<long,SUBSCRIPTORS_LIST>( capabilityID, subscriptors ) );
            pthread_mutex_unlock(&mutex_);
            return SubscribeResult_Ok;
            
        } else {
            subscriptors = it->second;
            for (SUBSCRIPTORS_LIST::iterator itSubscriptors = subscriptors.begin(); itSubscriptors != subscriptors.end(); itSubscriptors++) {
                if ( data.equals( *itSubscriptors ) ) {
                    pthread_mutex_unlock(&mutex_);
                    return SubscribeResult_AlreadySubscribed;
                }
            }
            subscriptors.push_back( data );
            it->second = subscriptors;    
            pthread_mutex_unlock(&mutex_);
            return SubscribeResult_Ok;
        }
        
    } catch(...){
        pthread_mutex_unlock(&mutex_);
        return SubscribeResult_GenericError;
    }
}

bool SubscriptorsManager::isSubscribed( long capabilityID, std::string peerID, std::string& subscribedUserHash ){
    pthread_mutex_lock(&mutex_);
    if (subscriptors_.find(capabilityID) == subscriptors_.end()){
        pthread_mutex_unlock(&mutex_);
        return false;
    }
    
    SUBSCRIPTORS_LIST subscriptors = subscriptors_.at(capabilityID);
    for (SUBSCRIPTORS_LIST::iterator itPeers = subscriptors.begin(); itPeers != subscriptors.end(); itPeers++){
        SubscriptionData data = *itPeers;
        if (peerID.compare(data.peerID_)==0) {
            subscribedUserHash.assign( data.userHash_ );
            pthread_mutex_unlock(&mutex_);
            return true;            
        }
    }
    subscribedUserHash.assign("");
    pthread_mutex_unlock(&mutex_);
    return false; 
}

UnsubscribeResult SubscriptorsManager::removeSubscription(long capabilityID, std::string userHash, std::string peerID ) {
   
    pthread_mutex_lock(&mutex_);
    if (subscriptors_.find(capabilityID) == subscriptors_.end()){
        pthread_mutex_unlock(&mutex_);
        return UnsubscribeResult_NotSubscribedToCapability;
    }
    
    SubscriptionData dataAux(peerID, userHash);
    SUBSCRIPTORS_LIST subscriptors = subscriptors_.at(capabilityID);
    for (SUBSCRIPTORS_LIST::iterator itPeers = subscriptors.begin(); itPeers != subscriptors.end(); itPeers++){
        SubscriptionData data = *itPeers;
        if ( dataAux.equals( data ) ) {
            subscriptors.erase(itPeers);
            subscriptors_.at(capabilityID) = subscriptors;            
            pthread_mutex_unlock(&mutex_);
            return UnsubscribeResult_Ok;
        }
    }
    pthread_mutex_unlock(&mutex_);
    return UnsubscribeResult_NotSubscribedToCapability; 
}

void SubscriptorsManager::removeSubscriptions( std::string peerID ) {

    pthread_mutex_lock(&mutex_);
    for( SUBSCRIPTORS_MAP::iterator it = subscriptors_.begin(); it != subscriptors_.end(); it++){
        SUBSCRIPTORS_LIST subscriptors = it->second;
        SUBSCRIPTORS_LIST::iterator itPeers = subscriptors.begin();
        while ( itPeers != subscriptors.end()){
            SubscriptionData data = *itPeers;
            if (data.peerID_.compare(peerID) == 0) {
                itPeers = subscriptors.erase(itPeers);
            } else {
                ++itPeers;
            }
        }
        subscriptors_.at(it->first) = subscriptors;
    } 
    pthread_mutex_unlock(&mutex_);
}

void SubscriptorsManager::onCapabilityUpdate( long capabilityID, Packet& message ) {
    
    if (connections_ == NULL)
        return;
    
    pthread_mutex_lock(&mutex_);
    if (subscriptors_.find(capabilityID) != subscriptors_.end()) {
        SUBSCRIPTORS_LIST subscriptors = subscriptors_.at( capabilityID );
        for (SUBSCRIPTORS_LIST::iterator itPeers = subscriptors.begin(); itPeers != subscriptors.end(); itPeers++){
            SubscriptionData data = *itPeers;
            connections_->sendMessageToClient( data.peerID_, message );
        }
    }
    pthread_mutex_unlock(&mutex_);
}