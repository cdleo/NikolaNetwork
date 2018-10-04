/* 
 * File:   ConnectionsHandlers.cpp
 * Author: cleo
 * 
 * Created on July 15, 2014, 11:09 AM
 */

#include <ConnectionsHandlers.h>
#include <support/CapabilitiesManager.h>
#include <support/SubscriptorsManager.h>
#include <support/ServicesManager.h>
#include <DataStorage.h>
#include <Parameters.h>

#include <synapse/Messages.h>

using namespace Synapse;

ConnectionsHandlers::ConnectionsHandlers() { 
    connections_ = new ConnectionsManager();
    connections_->setConnectionCallbacks(this);
    connections_->setRequestCallbacks(this);
    connections_->setResponseCallbacks(this);
    connections_->setUpdateCallbacks(this);
}

ConnectionsHandlers::~ConnectionsHandlers() throw() {
    delete connections_;
}

bool ConnectionsHandlers::initialize(){
    
    std::string key,cert;
    long timeoutms;
    PARAMETERS->getValue("certificate", cert); 
    PARAMETERS->getValue("privateKey", key);       
    PARAMETERS->getValue("pollTimeout", timeoutms);       
    bool result = connections_->initialize(cert, key, timeoutms);
    
    if (result) {
        long basePort, maxAcceptors;
        PARAMETERS->getValue("acceptorBasePort", basePort);
        PARAMETERS->getValue("maxAcceptorsPerHost", maxAcceptors);    
        result = connections_->listen( basePort, maxAcceptors );
    }
    
    if (result) {
        result = SUBSCRIPTORS->initialize(connections_);
    }
        
    return result;
}

bool ConnectionsHandlers::connectToClient(std::string host, const int port) {

    return connections_->connectToClient(host, port, 1);
    
}

void ConnectionsHandlers::sendMessageToClient( std::string clientID, Packet& msg ) {

    connections_->sendMessageToClient(clientID, msg);
    
}

/* ************** Connection Callbacks ************** */
void ConnectionsHandlers::OnClient_Connect(SynapseConnector* peer) {
    LOG->LogMessage(LOG_INFO, "NEW CONNECTION FROM %s (fd %d)", peer->getID().c_str(), peer->getFD());
}

void ConnectionsHandlers::OnClient_Disconnect(SynapseConnector* peer){
    LOG->LogMessage(LOG_INFO, "DISCONNECTION FROM %s (fd %d)", peer->getID().c_str(), peer->getFD());
    SUBSCRIPTORS->removeSubscriptions( peer->getID() );
}    

/* **************** Request Callbacks **************** */
void ConnectionsHandlers::OnIdent_Request(Request* request, SynapseConnector* peer){
    
    std::string moduleDescription;
    PARAMETERS->getValue("moduleDescription", moduleDescription);
    Ident ident(peer->getLocalIP().c_str(), connections_->getAcceptorPort(), ModuleType_NetworkDiscover, moduleDescription.c_str());
    IdentResp resp(ident);
    peer->write( (Packet&) resp);
}

void ConnectionsHandlers::OnServices_Request(Request* request, SynapseConnector* peer){
    ServicesResp servResp;
    servResp.setServices( SERVICES->getLocalServices() );
    peer->write( (Packet&) servResp);
}

void ConnectionsHandlers::OnCapabilities_Request(Request* request, SynapseConnector* peer){
    CapabilitiesResp capResp;
    capResp.setCapabilities( CAPABILITIES->getLocalCapabilities() );
    peer->write( (Packet&) capResp);
}

void ConnectionsHandlers::OnSubscription_Request(Subscribe* request, SynapseConnector* peer){
    SubscribeResult result = SubscribeResult_Undefined;
    if (CAPABILITIES->existCapability(request->getCapability())) {
        result = SUBSCRIPTORS->addSubscription( request->getCapability().getId(), request->getUserHash(), peer->getID() );
    } else {
        result = SubscribeResult_UnknownCapability;
    }
    SubscribeResp resp(request->getCapability(), request->getUserHash(), result);
    peer->write( (Packet&) resp);
}

void ConnectionsHandlers::OnRevokeSubscription_Request(RevokeSubscription* request, SynapseConnector* peer){

}

void ConnectionsHandlers::OnUnsubscription_Request(Unsubscribe* request, SynapseConnector* peer){
    UnsubscribeResult result = UnsubscribeResult_Undefined;
    if (CAPABILITIES->existCapability(request->getCapability())) {
        result = SUBSCRIPTORS->removeSubscription( request->getCapability().getId(), request->getUserHash(), peer->getID() );
    } else {
        result = UnsubscribeResult_UnknownCapability;
    }
    UnsubscribeResp resp(request->getCapability(), request->getUserHash(), result);
    peer->write( (Packet&) resp);
}

void ConnectionsHandlers::OnSetAnalogValue_Request(SetAnalogValue* request, SynapseConnector* peer){
    //Nada, este modulo no tiene capabilities que setear
}

void ConnectionsHandlers::OnSetDigitalValue_Request(SetDigitalValue* request, SynapseConnector* peer){
    //Nada, este modulo no tiene capabilities que setear
}

void ConnectionsHandlers::OnGetNetwork_Request(GetServiceData* request, SynapseConnector* peer){
    GetServiceDataResp* resp = CUSTOM_STORAGE->getData( request->getService() );
    if (resp == NULL){
        resp = new GetServiceDataResp(request->getSubCommandID(), request->getService(), GetDataResult_UnknownService);
    }
    peer->write( (Packet&) *resp);
    delete resp;
}

void ConnectionsHandlers::OnGetUserHash_Request(UserValidation* request, SynapseConnector* peer){
    //Nada, este modulo no tiene el servicio de validacion
}

void ConnectionsHandlers::OnGetUserSecurityLevel_Request(UserSecurityLevelQuery* request, SynapseConnector* peer){
    //Nada, este modulo no tiene el servicio de validacion
}                 

void ConnectionsHandlers::OnGetAnalogValue_Request(GetAnalogValue* request, SynapseConnector* peer){
    //Nada, este modulo no tiene el capabilities que leer
}

void ConnectionsHandlers::OnGetDigitalValue_Request(GetDigitalValue* request, SynapseConnector* peer){
    //Nada, este modulo no tiene el capabilities que leer
}

/* **************** Response Callbacks **************** */
void ConnectionsHandlers::OnIdent_Response(IdentResp* response, SynapseConnector* peer){
    
    if (identifiedHosts_.find( peer->getID() ) != identifiedHosts_.end()) {
        identifiedHosts_.at( peer->getID() ) = response->getIdent();
    } else {
        identifiedHosts_.insert( std::pair<std::string, Ident>( peer->getID(), response->getIdent() ) );
    }

    Request reqServ(CommandID_Services);
    peer->write( (Packet&) reqServ);        
    
    Request reqCap(CommandID_Capabilities);
    peer->write( (Packet&) reqCap);      

}

void ConnectionsHandlers::OnServices_Response(ServicesResp* response, SynapseConnector* peer){
    
    ServicesList list = response->getServices();
    
    SERVICES->addForeignServices( identifiedHosts_.at( peer->getID() ), list);    
    
    for (ServicesList::iterator it = list.begin(); it != list.end(); it++){
        ServiceDefinition def = *it;
        if (def.getType() == ServiceType_UserValidation){
            std::string usr, pwd;
            PARAMETERS->getValue("user", usr);
            PARAMETERS->getValue("password", pwd);                
            UserValidation request( def, usr, pwd );
            peer->write( (Packet&) request);
        }
    }

}

void ConnectionsHandlers::OnCapabilities_Response(CapabilitiesResp* response, SynapseConnector* peer){
    
    CapabilitiesList list = response->getCapabilities();
    
    CAPABILITIES->addForeignCapabilities(identifiedHosts_.at( peer->getID() ), list); 
}

void ConnectionsHandlers::OnSubscription_Response(SubscribeResp* response, SynapseConnector* peer){
}

void ConnectionsHandlers::OnRevokeSubscription_Response(RevokeSubscriptionResp* request, SynapseConnector* peer){
}


void ConnectionsHandlers::OnUnsubscription_Response(UnsubscribeResp* response, SynapseConnector* peer){
}

void ConnectionsHandlers::OnSetAnalogValue_Response(SetValueResp* response, SynapseConnector* peer){
}

void ConnectionsHandlers::OnSetDigitalValue_Response(SetValueResp* response, SynapseConnector* peer){
}

void ConnectionsHandlers::OnGetNetwork_Response(NetworkResp* response, SynapseConnector* peer){
    //Nada, este modulo es el que provee la red
}

void ConnectionsHandlers::OnGetUserHash_Response(UserValidationResp* response, SynapseConnector* peer){
    if (response->getResult() == ValidationResult_Ok){
        CUSTOM_STORAGE->setUserHash( response->getHash() );
    } else {
        CUSTOM_STORAGE->setUserHash( "" );
    }   
}

void ConnectionsHandlers::OnGetUserSecurityLevel_Response(UserSecurityLevelQueryResp* response, SynapseConnector* peer){
    //Nada, los servicios y capacidades de este modulo no necesitan validar usuario 
}                 

void ConnectionsHandlers::OnGetAnalogValue_Response(GetAnalogValueResp* response, SynapseConnector* peer){
    //Nada, este modulo no solicita ningun valor
}

void ConnectionsHandlers::OnGetDigitalValue_Response(GetDigitalValueResp* response, SynapseConnector* peer){
    //Nada, este modulo no solicita ningun valor
}    

void ConnectionsHandlers::OnNetwork_Update(NetworkUpdate* update, SynapseConnector* peer){
    //Nada, este modulo es el que provee la red    
}

void ConnectionsHandlers::OnResources_Update(ResourcesUpdate* update, SynapseConnector* peer){
    //Nada, este modulo no se subscribe a eso
}


void ConnectionsHandlers::OnAnalogValue_Update(AnalogValueUpdate* update, SynapseConnector* peer){
    //Nada, este modulo no solicita ningun valor
}

void ConnectionsHandlers::OnDigitalValue_Update(DigitalValueUpdate* update, SynapseConnector* peer){
    //Nada, este modulo no solicita ningun valor
} 

void ConnectionsHandlers::OnRevokedUser_Update(RevokedUserUpdate* update, SynapseConnector* peer){
}