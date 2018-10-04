/* 
 * File:   GlobalDefinitions.h
 * Author: cleo
 *
 * Created on August 5, 2014, 9:20 AM
 */

#ifndef GLOBALDEFINITIONS_H
#define	GLOBALDEFINITIONS_H

#include <map>
#include <set>
#include <string>

#include <synapse/GlobalDefinitions.h>

//#define _DAEMON_

using namespace Synapse;

class GatewaySubscriptionKey {
public:
    GatewaySubscriptionKey(long capabilityID, std::string userHash)
      : capabilityID(capabilityID), userHash(userHash) {};  

    virtual ~GatewaySubscriptionKey(){};
      
    bool operator<(const GatewaySubscriptionKey &right) const {
        return ( capabilityID < right.capabilityID );
    } 
    
    bool operator==(const GatewaySubscriptionKey &right) const {
       return ( ( capabilityID == right.capabilityID ) && (userHash.compare(right.userHash) == 0) );
    }  
    
    long getCapabilityID(){
        return capabilityID;
    }
    
    std::string getUserHash(){
        return userHash;
    }
    
protected:    
    long  capabilityID;
    std::string userHash;    
};

class GatewaySubscription {
public:    
    GatewaySubscription()
    : subscriptionStatus(SubscribeResult_Undefined) {
        pthread_mutex_init(&mutex,NULL);
        gatewayPeers.clear();
    }  
    
    virtual ~GatewaySubscription(){
        pthread_mutex_destroy(&mutex);
    }

    long getSubscriptionStatus() {
        return subscriptionStatus;
    }
    
    void setSubscriptionStatus(long status) {
        subscriptionStatus = status;
    }
    
    bool addGatewayPeer(std::string peer) {
        pthread_mutex_lock(&mutex);
        gatewayPeers.insert( peer );
        pthread_mutex_unlock(&mutex);
        return true;
    }
    
    bool removeGatewayPeer(std::string peer) {
        pthread_mutex_lock(&mutex);
        std::set<std::string>::iterator it = gatewayPeers.find(peer);
        if (it != gatewayPeers.end()) {
            gatewayPeers.erase(it);
            pthread_mutex_unlock(&mutex);
            return true;
        }
        pthread_mutex_unlock(&mutex);
        return false;
    }    
    
    std::set<std::string> getPeers() {
        pthread_mutex_lock(&mutex);
        std::set<std::string> peersCopy = gatewayPeers;
        pthread_mutex_unlock(&mutex);
        return peersCopy;
    }
    
    bool existsPeer(std::string peer) {
        bool result = false;
        pthread_mutex_lock(&mutex);
        result = (gatewayPeers.find(peer) != gatewayPeers.end());
        pthread_mutex_unlock(&mutex);
        return result;
    }    
    
private:
    long                        subscriptionStatus;
    std::set<std::string>       gatewayPeers;    
    
    pthread_mutex_t             mutex;
};

typedef std::map< GatewaySubscriptionKey, GatewaySubscription >   GATEWAY_SUBSCRIPTORS_MAP;

typedef std::set< std::string >                                   PENDING_PEERS_SET;
typedef std::map< std::string /*pendingOperationID*/, PENDING_PEERS_SET /*pendingPeers*/>   PENDINGS_MAP;

#endif	/* GLOBALDEFINITIONS_H */

