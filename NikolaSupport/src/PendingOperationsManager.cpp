/* 
 * File:   PendingOperationsManager.cpp
 * Author: cleo
 * 
 * Created on August 13, 2014, 11:33 PM
 */

#include <support/PendingOperationsManager.h>

#include <synapse/RevokeSubscription.h>
#include <synapse/SetAnalogValue.h>
#include <synapse/SetDigitalValue.h>
#include <synapse/GetAnalogValue.h>
#include <synapse/GetDigitalValue.h>
#include <synapse/Subscribe.h>

PendingOperationsManager::PendingOperationsManager() {
   pthread_mutex_init(&mutex_, NULL);
}

PendingOperationsManager::~PendingOperationsManager() throw() {
    for (OperationsByUserMap::iterator it = pendingOperations_.begin(); it != pendingOperations_.end(); it++){
        this->erasePendingOperationsForUser( it->first, it->second );
        pendingOperations_.erase(it);
    }
    pthread_mutex_destroy(&mutex_);
}

bool PendingOperationsManager::addPendingOperation(Packet* operation, std::string peerID){

    Packet* operationCopy = NULL;
    std::string userHash = "";
    switch (operation->getCommandID()){
        case CommandID_Subscribe:
        {
            operationCopy = new Subscribe( (Subscribe*) operation);
            userHash.assign( ( (Subscribe*) operationCopy)->getUserHash() );
            break;
        }
        
        case CommandID_Revoke:
        {
            operationCopy = new RevokeSubscription( (RevokeSubscription*) operation);
            userHash.assign( ( (RevokeSubscription*) operationCopy)->getUserHash() );
            break;
        }        
        
        case CommandID_SetValue:
            switch (operation->getSubCommandID()){
                case SubCommandID_AnalogValue:
                {
                    operationCopy = new SetAnalogValue( (SetAnalogValue*) operation);
                    userHash.assign( ( (SetAnalogValue*) operationCopy)->getUserHash() );
                    break;
                }
                
                case SubCommandID_DigitalValue:
                {
                    operationCopy = new SetDigitalValue( (SetDigitalValue*) operation);
                    userHash.assign( ( (SetDigitalValue*) operationCopy)->getUserHash() );
                    break;
                }
                
                default:
                    return false;
            }
            break;
            
        case CommandID_GetValue:
            switch (operation->getSubCommandID()){
                case SubCommandID_AnalogValue:
                {
                    operationCopy = new GetAnalogValue( (GetAnalogValue*) operation);
                    userHash.assign( ( (GetAnalogValue*) operationCopy)->getUserHash() );
                    break;
                }
                
                case SubCommandID_DigitalValue:
                {
                    operationCopy = new GetDigitalValue( (GetDigitalValue*) operation);
                    userHash.assign( ( (GetDigitalValue*) operationCopy)->getUserHash() );
                    break;
                }
                
                default:
                    return false;
            }
            break;
            
        default:
            return false;
    }
    
    if (operationCopy != NULL) {
        PendingOperation pendingOp;
        pendingOp.operation = operationCopy;
        pendingOp.requestedBy.assign( peerID );
        
        pthread_mutex_lock(&mutex_);
        PendingOperationsList list;
        OperationsByUserMap::iterator it = pendingOperations_.find( userHash );
        if (it != pendingOperations_.end()) {
            list = it->second;
            list.push_back( pendingOp );
            std::sort(list.begin(), list.end(), CompareOps() );
            it->second = list;
        } else {
            list.push_back( pendingOp );
            pendingOperations_.insert( std::pair<std::string, PendingOperationsList>(userHash, list) );
        }
        pthread_mutex_unlock(&mutex_);
        return true;
    }
    
    return false;
}

bool PendingOperationsManager::getPendingOperationsForUser(std::string userHash, PendingOperationsList& list){
    if (pendingOperations_.find(userHash) != pendingOperations_.end()) {
        pthread_mutex_lock(&mutex_);
        list = pendingOperations_.at( userHash ); 
        pthread_mutex_unlock(&mutex_);
        return true;
    }
    return false;
}

void PendingOperationsManager::erasePendingOperationsForUser(std::string userHash, PendingOperationsList& pendingList) {
    
    OperationsByUserMap::iterator itUser = pendingOperations_.find(userHash);
    if (itUser != pendingOperations_.end()) {
        pthread_mutex_lock(&mutex_);
        
        PendingOperationsList userList = itUser->second; 

        PendingOperationsList equalsOps;
        std::set_intersection(userList.begin(), userList.end(), pendingList.begin(), pendingList.end(), std::back_inserter(equalsOps));
        
        for (PendingOperationsList::iterator itProcessedOps = equalsOps.begin(); itProcessedOps != equalsOps.end(); itProcessedOps++){
            delete itProcessedOps->operation;
            itProcessedOps->operation = NULL;
        }        
        
        PendingOperationsList differenceOps;
        std::set_difference(userList.begin(), userList.end(), pendingList.begin(), pendingList.end(), std::back_inserter(differenceOps) );  
        std::sort(differenceOps.begin(), differenceOps.end(), CompareOps() );
        itUser->second = differenceOps;

        pthread_mutex_unlock(&mutex_);
    }
    pendingList.clear();
}
