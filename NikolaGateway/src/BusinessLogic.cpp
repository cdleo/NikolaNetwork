/* 
 * File:   BusinessLogic.cpp
 * Author: cleo
 * 
 * Created on August 20, 2014, 10:01 AM
 */

#include <BusinessLogic.h>

#include <ConnectionsHandlers.h>
#include <GatewayConnHandlers.h>
#include <PendingRequestsManager.h>
#include <GatewaySubscriptorsManager.h>

#include <support/DataStorage.h>
#include <support/ServicesManager.h>
#include <support/SubscriptorsManager.h>
#include <support/CapabilitiesManager.h>

#include <Parameters.h>

#include <synapse/Messages.h>

BusinessLogic::BusinessLogic() {
}

BusinessLogic::~BusinessLogic() throw() {
}

void BusinessLogic::OnSubscription_Request(Subscribe* request, std::string gatewayPeerID){

    SubscribeResult result = SubscribeResult_UnknownCapability;
    
    CapabilityDefinition def;
    Ident module;
    if (CAPABILITIES->getForeignCapabilityById( request->getCapability().getId(), def, module ) ) {
        result = (SubscribeResult) GATEWAY_SUBSCRIPTORS->addSubscription( request->getCapability().getId(), request->getUserHash(), gatewayPeerID );
        if (result == SubscribeResult_SubscriptionPending) {
            CONN_HANDLERS->sendMessageToClient( module, (Packet&) *request);
        }  
    } else if (CAPABILITIES->getCapabilityById( request->getCapability().getId(), SourceData_Local, def ) ) {
        result = SUBSCRIPTORS->addSubscription( request->getCapability().getId(), request->getUserHash(), gatewayPeerID );
    }

    SubscribeResp response(request->getCapability(), request->getUserHash(), result);
    GATEWAY_CONN_HANDLERS->sendMessageToClient(gatewayPeerID, (Packet&) response);     
    
}

void BusinessLogic::OnSubscription_Response(SubscribeResp* response, std::string peerID){

    GATEWAY_SUBSCRIPTORS->setSubscriptionStatus(response->getCapability().getId(), response->getUserHash(), response->getResult());
    std::set<std::string> gatewayPeers;
    if ( GATEWAY_SUBSCRIPTORS->getGatewayPeersSubscribed(response->getCapability().getId(), response->getUserHash(), gatewayPeers) ) {
        for ( std::set<std::string>::iterator it = gatewayPeers.begin(); it != gatewayPeers.end(); it++ ) {
            GATEWAY_CONN_HANDLERS->sendMessageToClient( (std::string) *it, (Packet&) *response);     
        }
    }
    
}

void BusinessLogic::OnUnsubscription_Request(Unsubscribe* request, std::string gatewayPeerID){
    UnsubscribeResult result = UnsubscribeResult_UnknownCapability;
    
    CapabilityDefinition def;
    Ident module;
    if (CAPABILITIES->getForeignCapabilityById( request->getCapability().getId(), def, module ) ) {
        result = UnsubscribeResult_UnsubscriptionPending;
        if (GATEWAY_SUBSCRIPTORS->setSubscriptionStatus(request->getCapability().getId(), request->getUserHash(), result)) {
            CONN_HANDLERS->sendMessageToClient( module, (Packet&) *request);
        } else {
            result = UnsubscribeResult_NotSubscribedToCapability;
        }
    } else if (CAPABILITIES->getCapabilityById( request->getCapability().getId(), SourceData_Local, def ) ) {
        result = SUBSCRIPTORS->removeSubscription( request->getCapability().getId(), request->getUserHash(), gatewayPeerID );
    }

    UnsubscribeResp response(request->getCapability(), request->getUserHash(), result);
    GATEWAY_CONN_HANDLERS->sendMessageToClient(gatewayPeerID, (Packet&) response);   
}

void BusinessLogic::OnUnsubscription_Response(UnsubscribeResp* response, std::string peerID) {

    std::set<std::string> gatewayPeers;
    if ( GATEWAY_SUBSCRIPTORS->getGatewayPeersSubscribed(response->getCapability().getId(), response->getUserHash(), gatewayPeers) ) {
        for ( std::set<std::string>::iterator it = gatewayPeers.begin(); it != gatewayPeers.end(); it++ ) {
            GATEWAY_CONN_HANDLERS->sendMessageToClient( (std::string) *it, (Packet&) *response);     
        }
    }
    
    if (response->getResult() == UnsubscribeResult_Ok) {
        GATEWAY_SUBSCRIPTORS->removeSubscription(response->getCapability().getId(), response->getUserHash());
    }
        
}

void BusinessLogic::OnSetAnalogValue_Request(SetAnalogValue* request, std::string gatewayPeerID){
    
    if (!this->OnRequestOperation(request, request->getCommandID(), request->getSubCommandID(), 
                                 request->getCapability().getId(), request->getUserHash(), gatewayPeerID)) {    
        SetValueResp resp(request->getSubCommandID(), request->getCapability(), request->getUserHash(), SetValueResult_UnableToSetValue);
        GATEWAY_CONN_HANDLERS->sendMessageToClient(gatewayPeerID, (Packet&) resp);
    }
}

void BusinessLogic::OnSetDigitalValue_Request(SetDigitalValue* request, std::string gatewayPeerID){
    
    if (!this->OnRequestOperation(request, request->getCommandID(), request->getSubCommandID(), 
                                 request->getCapability().getId(), request->getUserHash(), gatewayPeerID)) {    
        SetValueResp resp(request->getSubCommandID(), request->getCapability(), request->getUserHash(), SetValueResult_UnableToSetValue);
        GATEWAY_CONN_HANDLERS->sendMessageToClient(gatewayPeerID, (Packet&) resp);
    }
}

void BusinessLogic::OnSetValue_Response(SetValueResp* response, std::string peerID){
    
    PENDING_PEERS_SET peers = PENDINGS->retrievePendingOperation( response->getCommandID(), 
                                response->getSubCommandID(), response->getCapability().getId(),
                                response->getUserHash() );
    
    for (PENDING_PEERS_SET::iterator it = peers.begin(); it != peers.end(); it++) {
        GATEWAY_CONN_HANDLERS->sendMessageToClient( *it, (Packet&) *response);
    }      
}

void BusinessLogic::OnGetAnalogValue_Request(GetAnalogValue* request, std::string gatewayPeerID){
    
    if (!this->OnRequestOperation(request, request->getCommandID(), request->getSubCommandID(), 
                                 request->getCapability().getId(), request->getUserHash(), gatewayPeerID)) {    
        GetAnalogValueResp resp(request->getCapability(), request->getUserHash(), GetDataResult_UnableToGetValue, 0);
        GATEWAY_CONN_HANDLERS->sendMessageToClient(gatewayPeerID, (Packet&) resp);
    }
}

void BusinessLogic::OnGetDigitalValue_Request(GetDigitalValue* request, std::string gatewayPeerID){
    
    if (!this->OnRequestOperation(request, request->getCommandID(), request->getSubCommandID(), 
                                 request->getCapability().getId(), request->getUserHash(), gatewayPeerID)) {    
        GetDigitalValueResp resp(request->getCapability(), request->getUserHash(), GetDataResult_UnableToGetValue, false);
        GATEWAY_CONN_HANDLERS->sendMessageToClient(gatewayPeerID, (Packet&) resp);
    }
} 

void BusinessLogic::OnGetValue_Response(GetValueResp* response, std::string peerID){
    
    PENDING_PEERS_SET peers = PENDINGS->retrievePendingOperation( response->getCommandID(), 
                                response->getSubCommandID(), response->getCapability().getId(),
                                response->getUserHash() );
    
    for (PENDING_PEERS_SET::iterator it = peers.begin(); it != peers.end(); it++) {
        GATEWAY_CONN_HANDLERS->sendMessageToClient( *it, (Packet&) *response);
    }      
}

bool BusinessLogic::OnRequestOperation(Packet* request, CommandID commandID, SubCommandID subcommandID, long capabilityID, std::string userHash, std::string pendingPeer){
    
    CapabilityDefinition def;
    Ident module;
    if (CAPABILITIES->getForeignCapabilityById( capabilityID, def, module ) ) {
        if (PENDINGS->addPendingOperation( commandID, subcommandID, capabilityID, userHash, pendingPeer) ) {
            CONN_HANDLERS->sendMessageToClient( module, (Packet&) *request);
            return true;
        }        
    }
    return false;    
}

void BusinessLogic::OnGetUserHash_Request(UserValidation* request, std::string gatewayPeerID ){

    Ident ident;
    ServiceDefinition def;
    if (SERVICES->getForeignServiceByType(ServiceType_UserValidation, def, ident)) {
        if (PENDINGS->addPendingUserValidation( request->getCommandID(), request->getSubCommandID(), 
                                request->getService().getId(), request->getUser(), request->getPwd(), gatewayPeerID) ) {
            
            CONN_HANDLERS->sendMessageToClient( ident, (Packet&) *request);
        }

    } else {
        UserValidationResp resp(request->getService(), ValidationResult_NoUserValidationModuleAvailable, request->getUser(), request->getPwd(), "");
        GATEWAY_CONN_HANDLERS->sendMessageToClient(gatewayPeerID, (Packet&) resp);
    }    
}

void BusinessLogic::OnGetUserHash_Response(UserValidationResp* response, std::string peerID ){
    PENDING_PEERS_SET peers = PENDINGS->retrievePendingUserValidation( response->getCommandID(), 
                                response->getSubCommandID(), response->getService().getId(),
                                response->getUser(), response->getPwd() );
    
    for (PENDING_PEERS_SET::iterator it = peers.begin(); it != peers.end(); it++) {
        GATEWAY_CONN_HANDLERS->sendMessageToClient( *it, (Packet&) *response);
    } 
}
