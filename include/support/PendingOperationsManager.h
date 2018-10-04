/* 
 * File:   PendingOperationsManager.h
 * Author: cleo
 *
 * Created on August 13, 2014, 11:33 PM
 */

#ifndef PENDINGOPERATIONSMANAGER_H
#define	PENDINGOPERATIONSMANAGER_H

#include <SingletonBase.h>
#include <synapse/Packet.h>

#include <map>
#include <vector>
#include <algorithm> 

#define PENDING PendingOperationsManager::getInstance()

typedef struct PendingOperation {
    Packet*         operation;
    std::string     requestedBy;
    
    bool operator< (const PendingOperation& other){
        return ((long long) operation) < ((long long) other.operation);
    }    
    
} PendingOperation;

struct CompareOps
{
    bool operator()(const PendingOperation& a, const PendingOperation& b) const
    {
        return ((long long) a.operation) < ((long long) b.operation);
    }
};

typedef std::vector<PendingOperation>                   PendingOperationsList;
typedef std::map<std::string, PendingOperationsList>    OperationsByUserMap;

class PendingOperationsManager : public SingletonBase<PendingOperationsManager> {
public:
    friend class SingletonBase<PendingOperationsManager>; 

     bool addPendingOperation(Packet* operation, std::string peerID);
     
     bool getPendingOperationsForUser(std::string userHash, PendingOperationsList& list);
     
     void erasePendingOperationsForUser(std::string userHash, PendingOperationsList& pendingList);
     
private:
    PendingOperationsManager();
    virtual ~PendingOperationsManager() throw();

    OperationsByUserMap         pendingOperations_;
    pthread_mutex_t             mutex_;
};

#endif	/* PENDINGOPERATIONSMANAGER_H */



