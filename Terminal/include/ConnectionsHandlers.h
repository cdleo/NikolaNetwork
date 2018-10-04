/* 
 * File:   ConnectionsHandlers.h
 * Author: cleo
 *
 * Created on July 15, 2014, 11:09 AM
 */

#ifndef CONNECTIONSHANDLERS_H
#define	CONNECTIONSHANDLERS_H

#include <support/ConnectionsManager.h>

#include <SingletonBase.h>
#include <boost/thread.hpp>
#include <vector>
#include <string>

#define CONN_HANDLERS ConnectionsHandlers::getInstance()

typedef std::map<std::string, Ident>        MAP_IDENTS;

class ConnectionsHandlers : public SingletonBase<ConnectionsHandlers>, 
                            public ConnectionCallbacks, 
                            public RequestCallbacks, 
                            public ResponseCallbacks, 
                            public UpdateCallbacks {
public:
    friend class SingletonBase<ConnectionsHandlers>;
    
    bool initialize();
    
    void sendMessageToClient( std::string clientID, Packet& msg );
    void sendMessageToClient( Ident ident, Packet& msg );

private:  
    ConnectionsHandlers();
    virtual ~ConnectionsHandlers() throw();

    /* ************** Connection Callbacks ************** */
    void OnClient_Connect(SynapseConnector* peer);
    void OnClient_Disconnect(SynapseConnector* peer);  
    
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
    void OnRevokeSubscription_Request(RevokeSubscription* request, SynapseConnector* peer);

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
    void OnRevokeSubscription_Response(RevokeSubscriptionResp* request, SynapseConnector* peer);
    
    /* **************** Update Callbacks **************** */
    void OnNetwork_Update(NetworkUpdate* update, SynapseConnector* peer);
    void OnResources_Update(ResourcesUpdate* update, SynapseConnector* peer);
    void OnAnalogValue_Update(AnalogValueUpdate* update, SynapseConnector* peer);
    void OnDigitalValue_Update(DigitalValueUpdate* update, SynapseConnector* peer);        
    void OnRevokedUser_Update(RevokedUserUpdate* update, SynapseConnector* peer);       
    
    MAP_IDENTS                  identifiedHosts_;
    
    ConnectionsManager*         connections_;    
};

#endif	/* CONNECTIONSHANDLERS_H */

