/* 
 * File:   ConnectionsManager.h
 * Author: cleo
 *
 * Created on July 15, 2014, 11:09 AM
 */

#ifndef CONNECTIONSMANAGER_H
#define	CONNECTIONSMANAGER_H

#include <synapse/SynapseConnections.h>

#include <SingletonBase.h>
#include <boost/thread.hpp>
#include <vector>
#include <string>

#define CONNECTIONS ConnectionsManager::getInstance()

typedef std::map<std::string, Ident>        MAP_IDENTS;

class ConnectionsManager : public SingletonBase<ConnectionsManager>, public RequestCallbacks, public ResponseCallbacks {
public:
    friend class SingletonBase<ConnectionsManager>;
    
    void sendMessageToClient(std::string clientID, Packet& msg, const int timeoutSeconds );
    void sendMessageToClient( Ident ident, Packet& msg, const int timeoutSeconds );

    void start();

private:  
    ConnectionsManager();
    virtual ~ConnectionsManager() throw();

    /* **************** Request Callbacks **************** */
    void OnIdent_Request(Request* request, SynapseConnector* peer);
    void OnServices_Request(Request* request, SynapseConnector* peer);
    void OnCapabilities_Request(Request* request, SynapseConnector* peer);
    void OnSubscription_Request(Subscribe* request, SynapseConnector* peer);
    void OnUnsubscription_Request(Unsubscribe* request, SynapseConnector* peer);
    void OnSetAnalogValue_Request(SetAnalogValue* request, SynapseConnector* peer);
    void OnSetDigitalValue_Request(SetDigitalValue* request, SynapseConnector* peer);
    void OnGetNetwork_Request(GetServiceData* request, SynapseConnector* peer);
    void OnGetUserHash_Request(UserValidation* request, SynapseConnector* peer);
    void OnGetUserSecurityLevel_Request(UserSecurityLevelQuery* request, SynapseConnector* peer);                 
    void OnGetAnalogValue_Request(GetAnalogValue* request, SynapseConnector* peer);
    void OnGetDigitalValue_Request(GetDigitalValue* request, SynapseConnector* peer);

    /* **************** Response Callbacks **************** */
    void OnIdent_Response(IdentResp* response, SynapseConnector* peer);
    void OnServices_Response(ServicesResp* response, SynapseConnector* peer);
    void OnCapabilities_Response(CapabilitiesResp* response, SynapseConnector* peer);
    void OnSubscription_Response(SubscribeResp* response, SynapseConnector* peer);
    void OnUnsubscription_Response(UnsubscribeResp* response, SynapseConnector* peer);
    void OnSetAnalogValue_Response(SetValueResp* response, SynapseConnector* peer);
    void OnSetDigitalValue_Response(SetValueResp* response, SynapseConnector* peer);
    void OnGetNetwork_Response(NetworkResp* response, SynapseConnector* peer);
    void OnGetUserHash_Response(UserValidationResp* response, SynapseConnector* peer);
    void OnGetUserSecurityLevel_Response(UserSecurityLevelQueryResp* response, SynapseConnector* peer);                 
    void OnGetAnalogValue_Response(GetAnalogValueResp* response, SynapseConnector* peer);
    void OnGetDigitalValue_Response(GetDigitalValueResp* response, SynapseConnector* peer);    
    void OnNetwork_Update(NetworkUpdate* update, SynapseConnector* peer);
    void OnAnalogValue_Update(AnalogValueUpdate* update, SynapseConnector* peer);
    void OnDigitalValue_Update(DigitalValueUpdate* update, SynapseConnector* peer);        
    
    SynapseConnections*         synapConn_;
    
    MAP_IDENTS                  identifiedHosts_;
    
    pthread_mutex_t             mutex_;
};

#endif	/* CONNECTIONSMANAGER_H */

