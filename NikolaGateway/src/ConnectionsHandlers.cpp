/* 
 * File:   ConnectionsHandlers.cpp
 * Author: cleo
 * 
 * Created on July 15, 2014, 11:09 AM
 */

#include <ConnectionsHandlers.h>
#include <GatewaySubscriptorsManager.h>
#include <GatewayDataStorage.h>
#include <BusinessLogic.h>
#include <Parameters.h>

#include <support/CapabilitiesManager.h>
#include <support/SubscriptorsManager.h>
#include <support/ServicesManager.h>

#include <synapse/Messages.h>

#include "synapse/PredefinedServicesAndCaps.h"

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

    return result;
}

void ConnectionsHandlers::sendMessageToClient( std::string clientID, Packet& msg ) {

    connections_->sendMessageToClient(clientID, msg);
    
}

void ConnectionsHandlers::sendMessageToClient( Ident ident, Packet& msg ) {

    connections_->sendMessageToClient( ident, msg);
    
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
    Ident ident(peer->getLocalIP().c_str(), connections_->getAcceptorPort(), ModuleType_Gateway, moduleDescription.c_str());
    IdentResp resp(ident);
    peer->write( (Packet&) resp);    
}

void ConnectionsHandlers::OnServices_Request(Request* request, SynapseConnector* peer){
    /* Hacia adentro de la red, NO exponemos ningun servicio */
    ServicesResp servResp;
    peer->write( (Packet&) servResp);    
}

void ConnectionsHandlers::OnCapabilities_Request(Request* request, SynapseConnector* peer){
    /* Hacia adentro de la red, NO exponemos ninguna capability */
    CapabilitiesResp capResp;
    peer->write( (Packet&) capResp);
}

void ConnectionsHandlers::OnSubscription_Request(Subscribe* request, SynapseConnector* peer){
    SubscribeResp resp(request->getCapability(), request->getUserHash(), SubscribeResult_UnknownCapability);
    peer->write( (Packet&) resp);    
}

void ConnectionsHandlers::OnUnsubscription_Request(Unsubscribe* request, SynapseConnector* peer){
    UnsubscribeResp resp(request->getCapability(), request->getUserHash(), UnsubscribeResult_UnknownCapability);
    peer->write( (Packet&) resp);    
}

void ConnectionsHandlers::OnSetAnalogValue_Request(SetAnalogValue* request, SynapseConnector* peer){
    SetValueResp resp(request->getSubCommandID(), request->getCapability(), request->getUserHash(), SetValueResult_UnknownCapability);
    peer->write( (Packet&) resp);
}

void ConnectionsHandlers::OnSetDigitalValue_Request(SetDigitalValue* request, SynapseConnector* peer){
    SetValueResp resp(request->getSubCommandID(), request->getCapability(), request->getUserHash(), SetValueResult_UnknownCapability);
    peer->write( (Packet&) resp);
}

void ConnectionsHandlers::OnGetNetwork_Request(GetServiceData* request, SynapseConnector* peer){
    GetServiceDataResp resp(request->getSubCommandID(), request->getService(), GetDataResult_UnknownService);
    peer->write( (Packet&) resp);    
}

void ConnectionsHandlers::OnGetUserHash_Request(UserValidation* request, SynapseConnector* peer){
    GetServiceDataResp resp(request->getSubCommandID(), request->getService(), GetDataResult_UnknownService);
    peer->write( (Packet&) resp);    
}

void ConnectionsHandlers::OnGetUserSecurityLevel_Request(UserSecurityLevelQuery* request, SynapseConnector* peer){
    GetServiceDataResp resp(request->getSubCommandID(), request->getService(), GetDataResult_UnknownService);
    peer->write( (Packet&) resp);    
}                 

void ConnectionsHandlers::OnGetAnalogValue_Request(GetAnalogValue* request, SynapseConnector* peer){
    GetValueResp resp(request->getSubCommandID(), request->getCapability(), request->getUserHash(), GetDataResult_UnknownCapability);
    peer->write( (Packet&) resp);
}

void ConnectionsHandlers::OnGetDigitalValue_Request(GetDigitalValue* request, SynapseConnector* peer){
    GetValueResp resp(request->getSubCommandID(), request->getCapability(), request->getUserHash(), GetDataResult_UnknownCapability);
    peer->write( (Packet&) resp);
}

void ConnectionsHandlers::OnRevokeSubscription_Request(RevokeSubscription* request, SynapseConnector* peer){
    //request->
    //if (SUBSCRIPTIONS->)
}

/* **************** Response Callbacks **************** */
void ConnectionsHandlers::OnIdent_Response(IdentResp* response, SynapseConnector* peer){
    
    if (identifiedHosts_.find( peer->getID() ) != identifiedHosts_.end()) {
        identifiedHosts_.at( peer->getID() ) = response->getIdent();
    } else {
        identifiedHosts_.insert( std::pair<std::string, Ident>( peer->getID(), response->getIdent() ) );
    }
    
    LOG->LogMessage(LOG_DEBUG,"ConnectionsHandlers::OnIdent_Response(): Connection from %s received! IP: %s Port: %u PeerID: %s", 
                    response->getIdent().getModuleDescription().c_str(), response->getIdent().getHost().c_str(), 
                    response->getIdent().getPort(), peer->getID().c_str());

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
        if (def.getType() == ServiceType_Network){
        /*  GetServiceData request(SubCommandID_GetNetwork, def);
            peer->write( (Packet&) request);*/
            
        } else if (def.getType() == ServiceType_UserValidation){
            SERVICES->addLocalService(def);     /* Este es el unico tipo de servicio que se expone */
        }
    }
    
    CapabilityDefinition def;
    if (CAPABILITIES->getCapabilityById(ID_RESOURCESUPDATE_CAPABILITY, SourceData_Local, def)) {
        ResourcesUpdate update(def, ResourceUpdateType_Services);
        SUBSCRIPTORS->onCapabilityUpdate(def.getId(), (Packet&) update);
    }

}

void ConnectionsHandlers::OnCapabilities_Response(CapabilitiesResp* response, SynapseConnector* peer){
    
    CapabilitiesList list = response->getCapabilities();
    CAPABILITIES->addForeignCapabilities(identifiedHosts_.at( peer->getID() ), list);  
    
    CapabilityDefinition def;
    if (CAPABILITIES->getCapabilityById(ID_RESOURCESUPDATE_CAPABILITY, SourceData_Local, def)) {
        ResourcesUpdate update(def, ResourceUpdateType_Capabilities);
        SUBSCRIPTORS->onCapabilityUpdate(def.getId(), (Packet&) update);
    }    
}

void ConnectionsHandlers::OnSubscription_Response(SubscribeResp* response, SynapseConnector* peer){
    LOGIC->OnSubscription_Response( response, peer->getID() );
}

void ConnectionsHandlers::OnUnsubscription_Response(UnsubscribeResp* response, SynapseConnector* peer){
    LOGIC->OnUnsubscription_Response(response, peer->getID() );
}

void ConnectionsHandlers::OnSetAnalogValue_Response(SetValueResp* response, SynapseConnector* peer){
    LOGIC->OnSetValue_Response( response, peer->getID() );
}

void ConnectionsHandlers::OnSetDigitalValue_Response(SetValueResp* response, SynapseConnector* peer){
    LOGIC->OnSetValue_Response( response, peer->getID() );
}

void ConnectionsHandlers::OnGetAnalogValue_Response(GetAnalogValueResp* response, SynapseConnector* peer){
    LOGIC->OnGetValue_Response( response, peer->getID() );
}

void ConnectionsHandlers::OnGetDigitalValue_Response(GetDigitalValueResp* response, SynapseConnector* peer){
    LOGIC->OnGetValue_Response( response, peer->getID() );
}    

void ConnectionsHandlers::OnGetNetwork_Response(NetworkResp* response, SynapseConnector* peer){
    long basePort, maxAcceptors;
    PARAMETERS->getValue("acceptorBasePort", basePort);
    PARAMETERS->getValue("maxAcceptorsPerHost", maxAcceptors);
    
    NetworkList neighbors = response->getNetworkList();
    
    LOG->LogMessage(LOG_DEBUG,"ConnectionsHandlers::OnGetNetwork_Response(): %d neighbors obtained from %s", 
                    neighbors.size(), peer->getID().c_str());    
    
    for (NetworkList::iterator it = neighbors.begin(); it != neighbors.end(); it++){
        
        LOG->LogMessage(LOG_DEBUG,"ConnectionsHandlers::OnGetNetwork_Response(): Neighbor %s, Status %d", 
                        it->getIp().c_str(), (int) it->getType());            
        
        for (long port = basePort; port < basePort + maxAcceptors; port++) {

            if ( (peer->getLocalIP().compare( it->getIp() ) == 0) && (port == (long) connections_->getAcceptorPort()) ) //Soy yo mismo
                continue;
            
            bool result = connections_->connectToClient( it->getIp(), port, 2);
            
            LOG->LogMessage(LOG_DEBUG,"ConnectionsHandlers::OnGetNetwork_Response(): Neighbor %s:%ld connection Result: %s", 
                            it->getIp().c_str(), port, (result) ? "OK" : "Failed");                
        }
    }
}

void ConnectionsHandlers::OnGetUserHash_Response(UserValidationResp* response, SynapseConnector* peer){
    LOGIC->OnGetUserHash_Response( response, peer->getID() );
}

void ConnectionsHandlers::OnGetUserSecurityLevel_Response(UserSecurityLevelQueryResp* response, SynapseConnector* peer){
    //Nada, como no tenemos servicios ni capabilities, no verificamos la identidad de nadie
}                 

void ConnectionsHandlers::OnRevokeSubscription_Response(RevokeSubscriptionResp* request, SynapseConnector* peer){
    
}

void ConnectionsHandlers::OnNetwork_Update(NetworkUpdate* update, SynapseConnector* peer){
    long basePort, maxAcceptors;
    PARAMETERS->getValue("acceptorBasePort", basePort);
    PARAMETERS->getValue("maxAcceptorsPerHost", maxAcceptors);
    
    NetworkList neighbors = update->getNetworkList();    
    for (NetworkList::iterator it = neighbors.begin(); it != neighbors.end(); it++){
        
        LOG->LogMessage(LOG_DEBUG,"ConnectionsHandlers::OnGetNetwork_Response(): Neighbor %s, Status %d", 
                        it->getIp().c_str(), (int) it->getType());            
        
        if (it->getType() == NetworkItemStatus_Added) {
            for (long port = basePort; port < basePort + maxAcceptors; port++) {
                if ( (peer->getLocalIP().compare( it->getIp() ) == 0) && (port == (long) connections_->getAcceptorPort()) ) //Soy yo mismo
                    continue;

                bool result = connections_->connectToClient( it->getIp(), port, 2);

                LOG->LogMessage(LOG_DEBUG,"ConnectionsHandlers::OnGetNetwork_Response(): Neighbor %s:%ld connection Result: %s", 
                                it->getIp().c_str(), port, (result) ? "OK" : "Failed");    
            }
        }
    }    
}

void ConnectionsHandlers::OnResources_Update(ResourcesUpdate* update, SynapseConnector* peer){
}

void ConnectionsHandlers::OnAnalogValue_Update(AnalogValueUpdate* update, SynapseConnector* peer){
    GATEWAY_SUBSCRIPTORS->onCapabilityUpdate( update->getCapability().getId(), (Packet&) *update );
}

void ConnectionsHandlers::OnDigitalValue_Update(DigitalValueUpdate* update, SynapseConnector* peer){
    GATEWAY_SUBSCRIPTORS->onCapabilityUpdate( update->getCapability().getId(), (Packet&) *update );    
} 

void ConnectionsHandlers::OnRevokedUser_Update(RevokedUserUpdate* update, SynapseConnector* peer){
    GATEWAY_SUBSCRIPTORS->revokeSubscriptions(update->getCapability(), update->getRevokedUserHash());
}
