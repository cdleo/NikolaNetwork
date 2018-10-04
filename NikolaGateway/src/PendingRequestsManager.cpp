/* 
 * File:   PendingRequestsManager.cpp
 * Author: cleo
 * 
 * Created on September 22, 2014, 1:34 PM
 */

#include <PendingRequestsManager.h>

PendingRequestsManager::PendingRequestsManager() {
    pthread_mutex_init(&mutex_,NULL);
}

PendingRequestsManager::~PendingRequestsManager() throw(){
    pthread_mutex_destroy(&mutex_);
}

bool PendingRequestsManager::addPendingOperation(CommandID commandID, SubCommandID subcommandID, 
        long capabilityID, std::string userHash, std::string pendingPeer ){

    pthread_mutex_lock(&mutex_);
    
    PENDING_PEERS_SET peers;
    bool result = false;
    
    std::string id = buildID(commandID, subcommandID, capabilityID, userHash);
    PENDINGS_MAP::iterator it = pendings_.find(id);
    if (it == pendings_.end()){
        peers.insert( pendingPeer );
        pendings_.insert( std::pair<std::string,PENDING_PEERS_SET>(id,peers) );
        result = true;
    } else {
        peers = it->second;
        if (peers.find(pendingPeer) == peers.end()){
            peers.insert( pendingPeer );
            pendings_.at(id) = peers;
            result =  true;
        }
    }
    
    pthread_mutex_unlock(&mutex_);
    return result;
}
    
PENDING_PEERS_SET PendingRequestsManager::retrievePendingOperation(CommandID commandID, 
        SubCommandID subcommandID, long capabilityID, std::string userHash) {

    pthread_mutex_lock(&mutex_);
    std::string id = buildID(commandID, subcommandID, capabilityID, userHash);
    PENDING_PEERS_SET peers;
    
    peers.clear();
    PENDINGS_MAP::iterator it = pendings_.find(id);
    if (it != pendings_.end()){
        peers = it->second;
        pendings_.erase(it);
    }
    
    pthread_mutex_unlock(&mutex_);
    return peers;    
}

bool PendingRequestsManager::addPendingUserValidation(CommandID commandID, SubCommandID subcommandID, 
        long serviceID, std::string user, std::string pwd, std::string pendingPeer){
    
    pthread_mutex_lock(&mutex_);
    
    PENDING_PEERS_SET peers;
    bool result = false;
    
    std::string id = buildID(commandID, subcommandID, serviceID, user, pwd);    
    PENDINGS_MAP::iterator it = pendings_.find(id);
    if (it == pendings_.end()){
        peers.insert( pendingPeer );
        pendings_.insert( std::pair<std::string,PENDING_PEERS_SET>(id,peers) );
        result = true;
    } else {
        peers = it->second;
        if (peers.find(pendingPeer) == peers.end()){
            peers.insert( pendingPeer );
            pendings_.at(id) = peers;
            result =true;
        }
    }    
    
    pthread_mutex_unlock(&mutex_);
    return result;
}
    
PENDING_PEERS_SET PendingRequestsManager::retrievePendingUserValidation( CommandID commandID, 
        SubCommandID subcommandID, long serviceID, std::string user, std::string pwd ){

    pthread_mutex_lock(&mutex_);
    std::string id = buildID(commandID, subcommandID, serviceID, user, pwd);
    PENDING_PEERS_SET peers;    

    peers.clear();
    PENDINGS_MAP::iterator it = pendings_.find(id);
    if (it != pendings_.end()){
        peers = it->second;
        pendings_.erase(it);
    }
    
    pthread_mutex_unlock(&mutex_);
    return peers;      
}

std::string PendingRequestsManager::buildID(CommandID commandID, SubCommandID subcommandID, 
                                            long capabilityID, std::string userHash) {
    std::ostringstream oss;
    oss << commandID << subcommandID << capabilityID << userHash;
    return oss.str();
}

std::string PendingRequestsManager::buildID(CommandID commandID, SubCommandID subcommandID, 
                                            long capabilityID, std::string user, std::string pwd) {
    std::ostringstream oss;
    oss << commandID << subcommandID << capabilityID << user << pwd;
    return oss.str();
}