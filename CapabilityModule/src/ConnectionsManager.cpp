/* 
 * File:   ConnectionsManager.cpp
 * Author: cleo
 * 
 * Created on July 15, 2014, 11:09 AM
 */

#include <ConnectionsManager.h>
#include <CapabilitiesManager.h>
#include <SubscriptorsManager.h>
#include <ServicesManager.h>
#include <BusinessLogicManager.h>
#include <PendingOperationsManager.h>
#include <DataStorage.h>
#include <Parameters.h>

#include <synapse/Messages.h>

using namespace Synapse;

ConnectionsManager::ConnectionsManager()
{
    pthread_mutex_init(&mutex_, NULL);
    
    std::string key,cert;
    PARAMETERS->getValue("certificate", cert); 
    PARAMETERS->getValue("privateKey", key);         
    synapConn_ = new SynapseConnections(this, this, cert, key);    
}

ConnectionsManager::~ConnectionsManager() throw() {
    
    if (synapConn_) {
        delete synapConn_;
        synapConn_ = NULL;
    }
    pthread_mutex_destroy(&mutex_);
}

void ConnectionsManager::start(){
    
    synapConn_->start();
    
    long basePort, maxAcceptors;
    PARAMETERS->getValue("acceptorBasePort", basePort);
    PARAMETERS->getValue("maxAcceptorsPerHost", maxAcceptors);    
    synapConn_->listen( basePort, maxAcceptors );
}

void ConnectionsManager::sendMessageToClient( std::string clientID, Packet& msg, const int timeoutSeconds ) {

    synapConn_->sendMessageToClient(clientID, msg, timeoutSeconds);
    
}

void ConnectionsManager::sendMessageToClient( Ident ident, Packet& msg, const int timeoutSeconds ) {

    synapConn_->sendMessageToClient( SynapseConnector::buildID( ident.getHost().c_str(), ident.getPort() ), msg, timeoutSeconds);
    
}

/* **************** Request Callbacks **************** */
void ConnectionsManager::OnIdent_Request(Request* request, SynapseConnector* peer){
    
    std::string moduleDescription;
    PARAMETERS->getValue("moduleDescription", moduleDescription);
    Ident ident(peer->getLocalIP().c_str(), synapConn_->getAcceptorPort(), ModuleType_IOTerminal, moduleDescription.c_str());
    IdentResp resp(ident);
    peer->write( (Packet&) resp, 1);    
}

void ConnectionsManager::OnServices_Request(Request* request, SynapseConnector* peer){
    pthread_mutex_lock(&mutex_);
    ServicesResp servResp;
    servResp.setServices( SERVICES->getLocalServices() );
    peer->write( (Packet&) servResp, 1);    
    pthread_mutex_unlock(&mutex_);
}

void ConnectionsManager::OnCapabilities_Request(Request* request, SynapseConnector* peer){
    pthread_mutex_lock(&mutex_);
    CapabilitiesResp capResp;
    capResp.setCapabilities(  CAPABILITIES->getLocalCapabilities() );
    peer->write(capResp,1);
    pthread_mutex_unlock(&mutex_);
}

void ConnectionsManager::OnSubscription_Request(Subscribe* request, SynapseConnector* peer){
    LOGIC->Subscription_Request(request, peer->getID());
}

void ConnectionsManager::OnUnsubscription_Request(Unsubscribe* request, SynapseConnector* peer){
    pthread_mutex_lock(&mutex_);
    UnsubscribeResult result = SUBSCRIPTORS->removeSubscription( request->getCapability().getId(), peer->getID() );
    UnsubscribeResp resp(request->getCapability(), result);
    peer->write( (Packet&) resp, 1);    
    pthread_mutex_unlock(&mutex_);
}

void ConnectionsManager::OnSetAnalogValue_Request(SetAnalogValue* request, SynapseConnector* peer){
    
    LOGIC->SetAnalogValue_Request(request, peer->getID());

}

void ConnectionsManager::OnSetDigitalValue_Request(SetDigitalValue* request, SynapseConnector* peer){
    
    LOGIC->SetDigitalValue_Request(request, peer->getID());
    
}

void ConnectionsManager::OnGetNetwork_Request(GetServiceData* request, SynapseConnector* peer){
    //Nada, este modulo no tiene el servicio de network
}

void ConnectionsManager::OnGetUserHash_Request(UserValidation* request, SynapseConnector* peer){
    //Nada, este modulo no tiene el servicio de validacion
}

void ConnectionsManager::OnGetUserSecurityLevel_Request(UserSecurityLevelQuery* request, SynapseConnector* peer){
    //Nada, este modulo no tiene el servicio de validacion
}                 

void ConnectionsManager::OnGetAnalogValue_Request(GetAnalogValue* request, SynapseConnector* peer){

    LOGIC->GetAnalogValue_Request(request, peer->getID());

}

void ConnectionsManager::OnGetDigitalValue_Request(GetDigitalValue* request, SynapseConnector* peer){
    
    LOGIC->GetDigitalValue_Request(request, peer->getID());

}

/* **************** Response Callbacks **************** */
void ConnectionsManager::OnIdent_Response(IdentResp* response, SynapseConnector* peer){
    
    if (identifiedHosts_.find( peer->getID() ) != identifiedHosts_.end()) {
        identifiedHosts_.at( peer->getID() ) = response->getIdent();
    } else {
        identifiedHosts_.insert( std::pair<std::string, Ident>( peer->getID(), response->getIdent() ) );
    }

    Request reqServ(CommandID_Services);
    peer->write( (Packet&) reqServ, 1);            
    
    Request reqCap(CommandID_Capabilities);
    peer->write( (Packet&) reqCap, 1);          

}

void ConnectionsManager::OnServices_Response(ServicesResp* response, SynapseConnector* peer){
    
    ServicesList list = response->getServices();
    
    SERVICES->addForeignServices( identifiedHosts_.at( peer->getID() ), list);    
    
    for (ServicesList::iterator it = list.begin(); it != list.end(); it++){
        ServiceDefinition def = *it;
        if (def.getType() == ServiceType_Network){
            GetServiceData request(SubCommandID_GetNetwork, def);
            peer->write( (Packet&) request, 1);
            
        } else if (def.getType() == ServiceType_UserValidation){
            std::string usr, pwd;
            PARAMETERS->getValue("user", usr);
            PARAMETERS->getValue("password", pwd);                
            UserValidation request( def, usr, pwd );
            peer->write( (Packet&) request, 1);
        }
    }

}

void ConnectionsManager::OnCapabilities_Response(CapabilitiesResp* response, SynapseConnector* peer){
    
    CapabilitiesList list = response->getCapabilities();
    
    CAPABILITIES->addForeignCapabilities(identifiedHosts_.at( peer->getID() ), list);
    
    for (CapabilitiesList::iterator it = list.begin(); it != list.end(); it++){
        CapabilityDefinition def = *it;
        if (def.getType() == Capability_Network){
            Subscribe request(def, STORAGE->getUserHash() );
            peer->write( (Packet&) request, 1);
        }
    }    
}

void ConnectionsManager::OnSubscription_Response(SubscribeResp* response, SynapseConnector* peer){
}

void ConnectionsManager::OnUnsubscription_Response(UnsubscribeResp* response, SynapseConnector* peer){
}

void ConnectionsManager::OnSetAnalogValue_Response(SetValueResp* response, SynapseConnector* peer){
}

void ConnectionsManager::OnSetDigitalValue_Response(SetValueResp* response, SynapseConnector* peer){
}

void ConnectionsManager::OnGetNetwork_Response(NetworkResp* response, SynapseConnector* peer){
    long basePort, maxAcceptors;
    PARAMETERS->getValue("acceptorBasePort", basePort);
    PARAMETERS->getValue("maxAcceptorsPerHost", maxAcceptors);
    
    NetworkList neighbors = response->getNetworkList();
    for (NetworkList::iterator it = neighbors.begin(); it != neighbors.end(); it++){
        for (long port = basePort; port < basePort + maxAcceptors; port++) {
            
            if ( (peer->getLocalIP().compare( it->getIp() ) == 0) && (port == (long) synapConn_->getAcceptorPort()) ) //Soy yo mismo
                continue;
            
            synapConn_->connectToClient( it->getIp(), port, 2);
        }
    }
}

void ConnectionsManager::OnGetUserHash_Response(UserValidationResp* response, SynapseConnector* peer){
    if (response->getResult() == ValidationResult_Ok){
        STORAGE->setUserHash( response->getHash() );
    } else {
        STORAGE->setUserHash( "" );
    }   
}

void ConnectionsManager::OnGetUserSecurityLevel_Response(UserSecurityLevelQueryResp* response, SynapseConnector* peer){

    LOGIC->OnUserSecurityLevelReceived( response->getResult(), response->getHash(), response->getSecurityLevel() );

}                 

void ConnectionsManager::OnGetAnalogValue_Response(GetAnalogValueResp* response, SynapseConnector* peer){
    STORAGE->setValue(response->getCapability().getId(), response->getValue());
}

void ConnectionsManager::OnGetDigitalValue_Response(GetDigitalValueResp* response, SynapseConnector* peer){
    STORAGE->setValue(response->getCapability().getId(), response->getValue());
}    

void ConnectionsManager::OnNetwork_Update(NetworkUpdate* update, SynapseConnector* peer){
    long basePort, maxAcceptors;
    PARAMETERS->getValue("acceptorBasePort", basePort);
    PARAMETERS->getValue("maxAcceptorsPerHost", maxAcceptors);
    
    NetworkList neighbors = update->getNetworkList();
    for (NetworkList::iterator it = neighbors.begin(); it != neighbors.end(); it++){
        if (it->getType() == NetworkItemStatus_Added) {
            for (long port = basePort; port < basePort + maxAcceptors; port++) {
            
                if ( (peer->getLocalIP().compare( it->getIp() ) == 0) && (port == (long) synapConn_->getAcceptorPort()) ) //Soy yo mismo
                    continue;
                            
                synapConn_->connectToClient( it->getIp(), port, 2 );
            }
        }
    }    
}

void ConnectionsManager::OnAnalogValue_Update(AnalogValueUpdate* update, SynapseConnector* peer){
    STORAGE->setValue(update->getCapability().getId(), update->getValue());
}

void ConnectionsManager::OnDigitalValue_Update(DigitalValueUpdate* update, SynapseConnector* peer){
    STORAGE->setValue(update->getCapability().getId(), update->getValue());
} 
