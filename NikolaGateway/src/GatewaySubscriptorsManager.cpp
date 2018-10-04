/* 
 * File:   GatewaySubscriptorsManager.cpp
 * Author: cleo
 * 
 * Created on September 19, 2014, 12:19 PM
 */

#include <GatewaySubscriptorsManager.h>
#include <GatewayConnHandlers.h>
#include <ConnectionsHandlers.h>

GatewaySubscriptorsManager::GatewaySubscriptorsManager() {
    pthread_mutex_init(&mutex_, NULL);
}

GatewaySubscriptorsManager::~GatewaySubscriptorsManager() throw() {
    subscriptors_.clear();
    pthread_mutex_destroy(&mutex_);
}

bool GatewaySubscriptorsManager::initialize(){
    
    subscriptors_.clear();
    return (subscriptors_.size() == 0);
}

long GatewaySubscriptorsManager::addSubscription(long capabilityID, std::string userHash, std::string gatewayPeerID) {
    
    try {
        pthread_mutex_lock(&mutex_);
        SubscribeResult result = SubscribeResult_Undefined;
        GatewaySubscriptionKey key( capabilityID, userHash );            
        GatewaySubscription data;
        
        GATEWAY_SUBSCRIPTORS_MAP::iterator it = subscriptors_.find(key);        
        if (it == subscriptors_.end()){
            if (data.addGatewayPeer(gatewayPeerID)) {
                data.setSubscriptionStatus( SubscribeResult_SubscriptionPending );
                subscriptors_.insert( std::pair<GatewaySubscriptionKey,GatewaySubscription>( key, data ) );
                result = (SubscribeResult) data.getSubscriptionStatus();
            } else {
                result = SubscribeResult_GenericError;
            }
            
        } else {
            data = it->second;
            if (data.existsPeer(gatewayPeerID)) {
                result = SubscribeResult_AlreadySubscribed;
            } else {
                if (data.addGatewayPeer(gatewayPeerID)) {
                    result = (SubscribeResult) data.getSubscriptionStatus();
                    it->second = data;
                } else {
                    result = SubscribeResult_GenericError;
                }
            }
        }
        pthread_mutex_unlock(&mutex_);
        return result;        
        
    } catch(...){
        pthread_mutex_unlock(&mutex_);
        return SubscribeResult_GenericError;
    }
}

bool GatewaySubscriptorsManager::setSubscriptionStatus( long capabilityID, std::string userHash, long status){
    try {
        pthread_mutex_lock(&mutex_);
        bool result = false;
        GatewaySubscriptionKey key( capabilityID, userHash );            
        GatewaySubscription data;
        
        GATEWAY_SUBSCRIPTORS_MAP::iterator it = subscriptors_.find(key);        
        if (it != subscriptors_.end()){
            data = it->second;
            data.setSubscriptionStatus(status);
            subscriptors_.at(key) = data;
            result = true;
        }
        pthread_mutex_unlock(&mutex_);
        return result;        
        
    } catch(...){
        pthread_mutex_unlock(&mutex_);
        return false;;
    }
}

bool GatewaySubscriptorsManager::getGatewayPeersSubscribed( long capabilityID, std::string userHash, std::set<std::string>& gatewayPeers){
    try {
        pthread_mutex_lock(&mutex_);
        bool result = false;
        GatewaySubscriptionKey key( capabilityID, userHash );            
        
        GATEWAY_SUBSCRIPTORS_MAP::iterator it = subscriptors_.find(key);        
        if (it != subscriptors_.end()){
            gatewayPeers = it->second.getPeers();
            result = true;
        }
        pthread_mutex_unlock(&mutex_);
        return result;        
        
    } catch(...){
        pthread_mutex_unlock(&mutex_);
        return false;
    }    
}

bool GatewaySubscriptorsManager::removeSubscription( long capabilityID, std::string userHash){
    try {
        pthread_mutex_lock(&mutex_);
        bool result = false;
        GatewaySubscriptionKey key( capabilityID, userHash );            
        
        GATEWAY_SUBSCRIPTORS_MAP::iterator it = subscriptors_.find(key);        
        if (it != subscriptors_.end()){
            subscriptors_.erase(it);
            result = true;
        }
        pthread_mutex_unlock(&mutex_);
        return result;        
        
    } catch(...){
        pthread_mutex_unlock(&mutex_);
        return false;
    }    
}

bool GatewaySubscriptorsManager::revokeSubscriptions(CapabilityDefinition capability, std::string revokedUserHash ){
    try {
        pthread_mutex_lock(&mutex_);
        bool result = false;
        GatewaySubscriptionKey key( capability.getId(), revokedUserHash );            
        
        GATEWAY_SUBSCRIPTORS_MAP::iterator it = subscriptors_.find(key);        
        if (it != subscriptors_.end()){
                        
            GatewaySubscription subscription = it->second;
            std::set<std::string> peers = subscription.getPeers();
            for(std::set<std::string>::iterator itPeers = peers.begin(); itPeers != peers.end(); itPeers++){
                UnsubscribeResp message(capability, revokedUserHash, UnsubscribeResult_SubscriptionRevoked);
                GATEWAY_CONN_HANDLERS->sendMessageToClient( *itPeers, message);
            }
            
            subscriptors_.erase(it);
            result = true;
        }
        pthread_mutex_unlock(&mutex_);
        return result;        
        
    } catch(...){
        pthread_mutex_unlock(&mutex_);
        return false;
    }    
}

void GatewaySubscriptorsManager::onCapabilityUpdate( long capabilityID, Packet& message ) {
    
    try {
        pthread_mutex_lock(&mutex_);
        
        for (GATEWAY_SUBSCRIPTORS_MAP::iterator it = subscriptors_.begin(); it != subscriptors_.end(); it++){
            GatewaySubscriptionKey key = it->first;
            if (key.getCapabilityID() == capabilityID){
                std::set<std::string> gatewayPeers = it->second.getPeers();
                for (std::set<std::string>::iterator itPeers = gatewayPeers.begin(); itPeers != gatewayPeers.end(); itPeers++){
                    GATEWAY_CONN_HANDLERS->sendMessageToClient( *itPeers, message);
                }
            }
        }
        
        pthread_mutex_unlock(&mutex_);      
        
    } catch(...){
        pthread_mutex_unlock(&mutex_);
    }       
}
