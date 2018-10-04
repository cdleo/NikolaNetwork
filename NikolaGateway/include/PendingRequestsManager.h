/* 
 * File:   PendingRequestsManager.h
 * Author: cleo
 *
 * Created on September 22, 2014, 1:34 PM
 */

#ifndef PENDINGREQUESTSMANAGER_H
#define	PENDINGREQUESTSMANAGER_H

#include <GlobalDefinitions.h>
#include <SingletonBase.h>

#include <synapse/Messages.h>

#define PENDINGS PendingRequestsManager::getInstance()

class PendingRequestsManager : public SingletonBase<PendingRequestsManager> {
public:
    friend class SingletonBase<PendingRequestsManager>;

    bool addPendingOperation(CommandID commandID, SubCommandID subcommandID, long capabilityID, std::string userHash, std::string pendingPeer );
    
    PENDING_PEERS_SET retrievePendingOperation(CommandID commandID, SubCommandID subcommandID, long capabilityID, std::string userHash);
    
    bool addPendingUserValidation(CommandID commandID, SubCommandID subcommandID, long serviceID, std::string user, std::string pwd, std::string pendingPeer);
    
    PENDING_PEERS_SET retrievePendingUserValidation( CommandID commandID, SubCommandID subcommandID, long serviceID, std::string user, std::string pwd );
    
private:
    PendingRequestsManager();
    virtual ~PendingRequestsManager() throw();
    
    std::string buildID(CommandID commandID, SubCommandID subcommandID, long capabilityID, std::string userHash);
    
    std::string buildID(CommandID commandID, SubCommandID subcommandID, long capabilityID, std::string user, std::string pwd);
    
    PENDINGS_MAP        pendings_;
    
    pthread_mutex_t     mutex_;
};

#endif	/* PENDINGREQUESTSMANAGER_H */

