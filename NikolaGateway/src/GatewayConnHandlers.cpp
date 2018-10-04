/* 
 * File:   GatewayConnHandlers.cpp
 * Author: cleo
 * 
 * Created on July 15, 2014, 11:09 AM
 */

#include <GatewayConnHandlers.h>
#include <PendingRequestsManager.h>

#include <support/CapabilitiesManager.h>
#include <support/SubscriptorsManager.h>
#include <support/ServicesManager.h>
#include <support/DataStorage.h>

#include <BusinessLogic.h>
#include <Parameters.h>

#include <synapse/Messages.h>

using namespace Synapse;

GatewayConnHandlers::GatewayConnHandlers() { 
    connections_ = new ConnectionsManager();
    connections_->setConnectionCallbacks(this);
    connections_->setRequestCallbacks(this);
    connections_->setResponseCallbacks(this);
    connections_->setUpdateCallbacks(this);
    
    SUBSCRIPTORS->initialize(connections_);
}

GatewayConnHandlers::~GatewayConnHandlers() throw() {
    delete connections_;
}

bool GatewayConnHandlers::initialize(){
    
    std::string key,cert;
    long timeoutms;
    PARAMETERS->getValue("gatewayCertificate", cert); 
    PARAMETERS->getValue("gatewayPrivateKey", key);       
    PARAMETERS->getValue("pollTimeout", timeoutms);       
    bool result = connections_->initialize(cert, key, timeoutms);
    
    if (result) {
        long gatewayPort;
        PARAMETERS->getValue("gatewayPort", gatewayPort);
        result = connections_->listen( gatewayPort, 0 );
    }

    return result;
}

void GatewayConnHandlers::sendMessageToClient( std::string clientID, Packet& msg ) {

    connections_->sendMessageToClient(clientID, msg);
    
}

void GatewayConnHandlers::sendMessageToClient( Ident ident, Packet& msg ) {

    connections_->sendMessageToClient( ident, msg);
    
}

/* ************** Connection Callbacks ************** */
void GatewayConnHandlers::OnClient_Connect(SynapseConnector* peer) {
    LOG->LogMessage(LOG_INFO, "GW: NEW CONNECTION FROM %s (fd %d)", peer->getID().c_str(), peer->getFD());
}

void GatewayConnHandlers::OnClient_Disconnect(SynapseConnector* peer){
    LOG->LogMessage(LOG_INFO, "GW: DISCONNECTION FROM %s (fd %d)", peer->getID().c_str(), peer->getFD());
}    

/* **************** Request Callbacks **************** */
void GatewayConnHandlers::OnIdent_Request(Request* request, SynapseConnector* peer){
    
    std::string moduleDescription;
    PARAMETERS->getValue("moduleDescription", moduleDescription);
    Ident ident(peer->getLocalIP().c_str(), connections_->getAcceptorPort(), ModuleType_Gateway, moduleDescription.c_str());
    IdentResp resp(ident);
    peer->write( (Packet&) resp);    
}

void GatewayConnHandlers::OnServices_Request(Request* request, SynapseConnector* peer){
    ServicesResp servResp;
    servResp.setServices( SERVICES->getLocalServices() );
    peer->write( (Packet&) servResp);    
}

void GatewayConnHandlers::OnCapabilities_Request(Request* request, SynapseConnector* peer){
    
    CapabilitiesList list;
    CAPABILITIES->getCapabilitiesByType(Capability_Undefined, SourceData_All, list);
    
    CapabilitiesResp capResp;
    capResp.setCapabilities( list );
    peer->write( (Packet&) capResp);
}

void GatewayConnHandlers::OnSubscription_Request(Subscribe* request, SynapseConnector* peer){
    LOGIC->OnSubscription_Request( request, peer->getID() );
}

void GatewayConnHandlers::OnUnsubscription_Request(Unsubscribe* request, SynapseConnector* peer){
    LOGIC->OnUnsubscription_Request( request, peer->getID() );
}

void GatewayConnHandlers::OnSetAnalogValue_Request(SetAnalogValue* request, SynapseConnector* peer){
    LOGIC->OnSetAnalogValue_Request( request, peer->getID() );
}

void GatewayConnHandlers::OnSetDigitalValue_Request(SetDigitalValue* request, SynapseConnector* peer){
    LOGIC->OnSetDigitalValue_Request( request, peer->getID() );
}

void GatewayConnHandlers::OnGetAnalogValue_Request(GetAnalogValue* request, SynapseConnector* peer){
    LOGIC->OnGetAnalogValue_Request( request, peer->getID() );
}

void GatewayConnHandlers::OnGetDigitalValue_Request(GetDigitalValue* request, SynapseConnector* peer){
    LOGIC->OnGetDigitalValue_Request( request, peer->getID() );
}

void GatewayConnHandlers::OnGetNetwork_Request(GetServiceData* request, SynapseConnector* peer){
    /* Nada, el servicio de Network es para quienes estan dentro de la red */    
}

void GatewayConnHandlers::OnGetUserHash_Request(UserValidation* request, SynapseConnector* peer){
    LOGIC->OnGetUserHash_Request(request, peer->getID());
}

void GatewayConnHandlers::OnGetUserSecurityLevel_Request(UserSecurityLevelQuery* request, SynapseConnector* peer){
    /* Nada, no hay solicitud de securitylevel desde fuera de la red */     
}   

void GatewayConnHandlers::OnRevokeSubscription_Request(RevokeSubscription* request, SynapseConnector* peer){
}

/* **************** Response Callbacks **************** */
void GatewayConnHandlers::OnIdent_Response(IdentResp* response, SynapseConnector* peer){
    
    if (identifiedHosts_.find( peer->getID() ) != identifiedHosts_.end()) {
        identifiedHosts_.at( peer->getID() ) = response->getIdent();
    } else {
        identifiedHosts_.insert( std::pair<std::string, Ident>( peer->getID(), response->getIdent() ) );
    }

    std::cout << "Connection from " << response->getIdent().getModuleDescription() << " received! IP:" << response->getIdent().getHost() << " Port:" 
              << response->getIdent().getPort() << " PeerID:" << peer->getID() << std::endl;
}

void GatewayConnHandlers::OnServices_Response(ServicesResp* response, SynapseConnector* peer){
    /*Nada, no se le solicitan servicios a ningun cliente externo */
}

void GatewayConnHandlers::OnCapabilities_Response(CapabilitiesResp* response, SynapseConnector* peer){
    
    /*Nada, no se le solicitan capabilities a ningun cliente externo */
}

void GatewayConnHandlers::OnSubscription_Response(SubscribeResp* response, SynapseConnector* peer){
    /*Nada, no se subscribe a servicios de ningun cliente externo */
}

void GatewayConnHandlers::OnUnsubscription_Response(UnsubscribeResp* response, SynapseConnector* peer){
    /*Nada, no se unsubscribe a servicios de ningun cliente externo */
}

void GatewayConnHandlers::OnSetAnalogValue_Response(SetValueResp* response, SynapseConnector* peer){
    /*Nada, no se le solicita nada a ningun cliente externo */
}

void GatewayConnHandlers::OnSetDigitalValue_Response(SetValueResp* response, SynapseConnector* peer){
    /*Nada, no se le solicita nada a ningun cliente externo */
}

void GatewayConnHandlers::OnGetNetwork_Response(NetworkResp* response, SynapseConnector* peer){
    /*Nada, no se le solicita nada a ningun cliente externo */
}

void GatewayConnHandlers::OnGetUserHash_Response(UserValidationResp* response, SynapseConnector* peer){
    /*Nada, no se le solicita nada a ningun cliente externo */   
}

void GatewayConnHandlers::OnGetUserSecurityLevel_Response(UserSecurityLevelQueryResp* response, SynapseConnector* peer){
    //Nada, como no tenemos servicios ni capabilities, no verificamos la identidad de nadie
}                 

void GatewayConnHandlers::OnGetAnalogValue_Response(GetAnalogValueResp* response, SynapseConnector* peer){
    /*Nada, no se le solicita nada a ningun cliente externo */
}

void GatewayConnHandlers::OnGetDigitalValue_Response(GetDigitalValueResp* response, SynapseConnector* peer){
    /*Nada, no se le solicita nada a ningun cliente externo */
}    

void GatewayConnHandlers::OnRevokeSubscription_Response(RevokeSubscriptionResp* request, SynapseConnector* peer){
    /*Nada, no se le solicita nada a ningun cliente externo */
}

void GatewayConnHandlers::OnNetwork_Update(NetworkUpdate* update, SynapseConnector* peer){
    /*Nada, no se le solicita nada a ningun cliente externo */ 
}

void GatewayConnHandlers::OnResources_Update(ResourcesUpdate* update, SynapseConnector* peer){
    /*Nada, no se le solicita nada a ningun cliente externo */ 
}

void GatewayConnHandlers::OnAnalogValue_Update(AnalogValueUpdate* update, SynapseConnector* peer){
    /*Nada, no se le solicita nada a ningun cliente externo */
}

void GatewayConnHandlers::OnDigitalValue_Update(DigitalValueUpdate* update, SynapseConnector* peer){
    /*Nada, no se le solicita nada a ningun cliente externo */
} 

void GatewayConnHandlers::OnRevokedUser_Update(RevokedUserUpdate* update, SynapseConnector* peer){
    /*Nada, no se le solicita nada a ningun cliente externo */
}