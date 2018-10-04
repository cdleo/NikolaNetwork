/* 
 * File:   ConnectionsManager.cpp
 * Author: cleo
 * 
 * Created on Aug 11, 2014, 11:09 AM
 */

#include <support/ConnectionsManager.h>

#include <sockets/Poller.h>
#include <synapse/Messages.h>

using namespace Synapse;

/* *********** Static Handlers *********** */
void* listenStatic(void* context) {
    ConnectionsManager* conn = static_cast<ConnectionsManager*>(context);
    conn->listenForClients();
    pthread_exit(0);
    return NULL;
}    

ConnectionsManager::ConnectionsManager()
: acceptor_(NULL), listenThread_(0), connectionCallbacks_(NULL), 
  requestCallbacks_(NULL), responseCallbacks_(NULL), updateCallbacks_(NULL) {
    pthread_mutex_init(&mutex_, NULL);
}

ConnectionsManager::~ConnectionsManager() throw() {
    freeConnections();
    pthread_mutex_destroy(&mutex_);
}

bool ConnectionsManager::initialize(std::string certificateFile, std::string keyFile, unsigned int timeoutms){

    certificateFile_.assign(certificateFile);
    keyFile_.assign(keyFile);    
    
    POLLER->setTimeout( timeoutms );
    
    try {
        start();
        return true;
        
    } catch (ThreadsExceptions::GeneralException& ex) {
        return false;
    }
}

bool ConnectionsManager::connectToClient(std::string host, const int port, const int timeout) {

    pthread_mutex_lock(&mutex_);
    
    std::string connIDAux = buildConnectorID(host.c_str(),port);
    SynapseConnector* peer;
    bool result = false;
    if (connections_.find( connIDAux ) != connections_.end()) {
        peer = connections_.at( connIDAux ).peer;
        if (peer->isConnected()){
            pthread_mutex_unlock(&mutex_);
            return true;
        } else {
            result = peer->connectToHost(host.c_str(), port, timeout);
        }
        
    } else {
        peer = new SynapseConnector(certificateFile_.c_str(), keyFile_.c_str());           
        if (peer->connectToHost(host.c_str(), port, timeout)) {
            Connection conn;
            conn.ident.setHost( host );
            conn.ident.setPort( port );
            conn.peer = peer;
            connections_.insert( std::pair<std::string,Connection>( peer->getID(), conn ) );
            result = true;
        } else {
            delete peer;
        }
    }
    
    pthread_mutex_unlock(&mutex_);
    
    if (result && connectionCallbacks_) 
        connectionCallbacks_->OnClient_Connect(peer);
    
    return result;
}

void ConnectionsManager::disconnectClient(std::string peerID) {

    pthread_mutex_lock(&mutex_);
    CONN_MAP::iterator it = connections_.find(peerID);
    if (it != connections_.end()) {
        SynapseConnector* peer = it->second.peer;
        connections_.erase(it);
        peer->disconnect();
        delete peer;
    }
    pthread_mutex_unlock(&mutex_);
}

bool ConnectionsManager::listen(unsigned int port, unsigned int maxAcceptorsPerHost){
    acceptor_ = new SynapseAcceptor();
    if (acceptor_->listen( port, port + maxAcceptorsPerHost )) {
        LOG->LogMessage(LOG_DEBUG,"Listening for clients on port %u", acceptor_->getPort());
        pthread_create (&listenThread_, NULL,  &listenStatic, this);
        return (listenThread_ != 0);
    } else {
        LOG->WARNING("Unable to listen for clients");
        return false;
    }
}

void ConnectionsManager::listenForClients() {

    SynapseConnector* peer = new SynapseConnector(certificateFile_.c_str(), keyFile_.c_str());    
    
    while (!m_canceled) {
        if (acceptor_->accept(peer,5) == SOCKET_OK_CODE) {
            //disconnectClient( peer->getID() );       
            pthread_mutex_lock(&mutex_);
            Connection conn;
            conn.peer = peer;            
            connections_.insert( std::pair<std::string,Connection>( peer->getID(), conn ) );
            pthread_mutex_unlock(&mutex_);
            
            if (connectionCallbacks_)
                connectionCallbacks_->OnClient_Connect(peer);
            
            peer = new SynapseConnector(certificateFile_.c_str(), keyFile_.c_str());    
        }
    }
}

void ConnectionsManager::run() {
    
    try {
        while (!m_canceled) {
            pthread_mutex_lock(&mutex_);
            CONN_MAP connectionsCopy = connections_;
            pthread_mutex_unlock(&mutex_);

            for(CONN_MAP::iterator it = connectionsCopy.begin(); it != connectionsCopy.end(); it++) {

                SynapseConnector* peer = it->second.peer;
                
                if (!peer->isConnected()) {
                    if (connectionCallbacks_)
                        connectionCallbacks_->OnClient_Disconnect(peer);                    
                    disconnectClient(peer->getID());
                    continue;
                }
                
                Packet* packet = NULL;
                SocketReturnCode result = peer->read(packet);
                while (result == SocketReturnCode_OK) {
                    if (requestCallbacks_ && (packet->getMsgType() == MsgType_Request)) {
                        switch (packet->getCommandID()) {
                            case CommandID_Ident:
                            {
                                requestCallbacks_->OnIdent_Request((Request*)packet, peer);
                                break;
                            }

                            case CommandID_Capabilities:
                            {   
                                requestCallbacks_->OnCapabilities_Request((Request*)packet, peer);
                                break;
                            }                 

                            case CommandID_Services:
                            {   
                                requestCallbacks_->OnServices_Request((Request*)packet, peer);
                                break;
                            }                          

                            case CommandID_Subscribe:
                            {
                                requestCallbacks_->OnSubscription_Request((Subscribe*)packet, peer);
                                break;
                            }

                            case CommandID_Revoke:
                            {
                                requestCallbacks_->OnRevokeSubscription_Request((RevokeSubscription*)packet, peer);
                                break;
                            }                               
                            
                            case CommandID_Unsubscribe:
                            {
                                requestCallbacks_->OnUnsubscription_Request((Unsubscribe*)packet, peer);
                                break;
                            }                        

                            case CommandID_SetValue:
                            {
                                switch (packet->getSubCommandID()) {
                                    case SubCommandID_AnalogValue:
                                        requestCallbacks_->OnSetAnalogValue_Request((SetAnalogValue*)packet, peer);
                                        break;

                                    case SubCommandID_DigitalValue:
                                        requestCallbacks_->OnSetDigitalValue_Request((SetDigitalValue*)packet, peer);
                                        break;

                                    default:
                                        break;
                                }
                                break;
                            }                          

                            case CommandID_GetValue:
                            {
                                switch(packet->getSubCommandID()) {   
                                   case SubCommandID_GetNetwork:
                                       requestCallbacks_->OnGetNetwork_Request((GetServiceData*)packet, peer);
                                        break;

                                    case SubCommandID_GetUserHash:
                                        requestCallbacks_->OnGetUserHash_Request((UserValidation*)packet, peer);
                                        break;

                                    case SubCommandID_GetUserSecurityLevel:
                                        requestCallbacks_->OnGetUserSecurityLevel_Request((UserSecurityLevelQuery*)packet, peer);                       
                                        break;

                                    case SubCommandID_AnalogValue:
                                        requestCallbacks_->OnGetAnalogValue_Request((GetAnalogValue*)packet, peer); 
                                        break;

                                    case SubCommandID_DigitalValue:
                                        requestCallbacks_->OnGetDigitalValue_Request((GetDigitalValue*)packet, peer);                         
                                        break;

                                    default:
                                        break;
                                } 
                                break;                            
                            }                         

                            case CommandID_Ping:    //El ping lo procesamos en esta capa
                            {
                                Response res(CommandID_Ping);
                                peer->write( (Packet&)res);  
                                break;
                            }

                            default:
                                 break;
                        }

                    } else if (responseCallbacks_ && (packet->getMsgType() == MsgType_Response)) {
                        switch (packet->getCommandID()) {
                            case CommandID_Ident:
                            {
                                if (connections_.find( peer->getID() ) != connections_.end()) {     //Actualizamos el resto de la informacion de Ident
                                    
                                    Ident ident = ((IdentResp*)packet)->getIdent();
                                    std::string peerIdReal = buildConnectorID( ident.getHost().c_str(), ident.getPort() );
                                        
                                    if ( (peerIdReal.compare(  peer->getID() ) != 0) &&                  // Significa que ahora que se quien es,
                                         (connections_.find( peerIdReal ) != connections_.end() ) ) {    // me doy cuenta que ya estaba conectado
                                        peer->disconnect();
                                        
                                    } else {
                                        pthread_mutex_lock(&mutex_);
                                        
                                        Connection conn = connections_.at( peer->getID() );
                                        connections_.erase( peer->getID() );

                                        conn.ident = ident;
                                        peer->updateID( conn.ident.getHost().c_str(), conn.ident.getPort() );

                                        connections_.insert( std::pair<std::string,Connection>( peer->getID(), conn ) );  
                                        
                                        pthread_mutex_unlock(&mutex_);
                                        
                                        responseCallbacks_->OnIdent_Response((IdentResp*)packet, peer);
                                    }
                                }
                                break;
                            }

                            case CommandID_Capabilities:
                            {                    
                                responseCallbacks_->OnCapabilities_Response((CapabilitiesResp*) packet, peer);
                                break;
                            }                 

                            case CommandID_Services:
                            {   
                                responseCallbacks_->OnServices_Response((ServicesResp*) packet, peer);
                                break;
                            }                        

                            case CommandID_Subscribe:
                            {
                                responseCallbacks_->OnSubscription_Response((SubscribeResp*) packet, peer);
                                break;
                            }

                            case CommandID_Revoke:
                            {
                                responseCallbacks_->OnRevokeSubscription_Response((RevokeSubscriptionResp*) packet, peer);
                                break;
                            }                               
                            
                            case CommandID_Unsubscribe:
                            {
                                responseCallbacks_->OnUnsubscription_Response((UnsubscribeResp*) packet, peer);
                                break;
                            }      

                            case CommandID_SetValue:
                            {
                                switch(packet->getSubCommandID()) {                                  
                                    case SubCommandID_AnalogValue:
                                        responseCallbacks_->OnSetAnalogValue_Response((SetValueResp*) packet, peer);
                                        break;                                    

                                    case SubCommandID_DigitalValue:                    
                                        responseCallbacks_->OnSetDigitalValue_Response((SetValueResp*) packet, peer);
                                        break;

                                    default:
                                        break;
                                }
                                break;
                            }                          

                            case CommandID_GetValue:
                            {
                                switch(packet->getSubCommandID()) {          
                                    case SubCommandID_GetNetwork:
                                        responseCallbacks_->OnGetNetwork_Response((NetworkResp*) packet, peer);
                                        break;

                                    case SubCommandID_GetUserHash:
                                        responseCallbacks_->OnGetUserHash_Response((UserValidationResp*) packet, peer);
                                        break;                                    

                                    case SubCommandID_GetUserSecurityLevel:
                                        responseCallbacks_->OnGetUserSecurityLevel_Response((UserSecurityLevelQueryResp*) packet, peer);
                                        break;                    

                                    case SubCommandID_AnalogValue:
                                        responseCallbacks_->OnGetAnalogValue_Response((GetAnalogValueResp*) packet, peer);
                                        break; 

                                    case SubCommandID_DigitalValue:
                                        responseCallbacks_->OnGetDigitalValue_Response((GetDigitalValueResp*) packet, peer);
                                        break;  

                                    default:
                                         break; 
                                }                               
                                break;
                            }                      

                            case CommandID_Update:
                            {
                                if (updateCallbacks_) {
                                    switch(packet->getSubCommandID()) {          
                                        case SubCommandID_GetNetwork:
                                            updateCallbacks_->OnNetwork_Update((NetworkUpdate*) packet, peer);
                                            break;                                      

                                        case SubCommandID_ResourcesUpdate:
                                            updateCallbacks_->OnResources_Update((ResourcesUpdate*) packet, peer);
                                            break;                                                                              

                                        case SubCommandID_AnalogValue:
                                            updateCallbacks_->OnAnalogValue_Update((AnalogValueUpdate*) packet, peer);
                                            break;                                                                          

                                        case SubCommandID_DigitalValue:
                                            updateCallbacks_->OnDigitalValue_Update((DigitalValueUpdate*) packet, peer);
                                            break;                                                                          

                                        case SubCommandID_GetUserRevoked:
                                            updateCallbacks_->OnRevokedUser_Update((RevokedUserUpdate*) packet, peer);
                                            break;                                              

                                        default:
                                            break;                   
                                    }
                                    break;
                                }
                            }                                              

                            case CommandID_Ping: //El ping lo procesamos en esta capa
                            default:
                                 break;                        
                        }

                    } 

                    delete packet;
                    result = peer->read(packet);
                }
                
            }

            //Dormimos 1ms el hilo, para que nos libere la CPU
            usleep(1000);    
        }
        
    } catch(std::runtime_error& er) {
        LOG->LogMessage(LOG_WARNING, "ConnectionsManager::getIncomingMessages() ERROR: %s", er.what());
        
    } catch (std::exception& ex) {
        LOG->LogMessage(LOG_WARNING, "ConnectionsManager::getIncomingMessages() ERROR: %s", ex.what());
    }
}

void ConnectionsManager::sendMessageToClient( std::string clientID, Packet& msg ) {

    if (connections_.find(clientID) != connections_.end()){
        SynapseConnector* peer = connections_.at(clientID).peer;
        if (peer->write( msg ) != SocketReturnCode_OK){
            disconnectClient( clientID );
        }
    }
}

void ConnectionsManager::sendMessageToClient( Ident ident, Packet& msg ) {

    std::string clientID = buildConnectorID(ident.getHost().c_str(), ident.getPort());
    sendMessageToClient(clientID, msg);
}

std::string ConnectionsManager::buildConnectorID(const char* host, const int port){
    std::ostringstream stream;
    stream << host << ":" << port;
    return stream.str();
}

void ConnectionsManager::freeConnections() {
    
    stop();
    join();
    
    LOG->DEBUG("[ConnectionsManager::freeConnections] Closing connections... ");
    
    if (listenThread_) {
        pthread_join(listenThread_, NULL);
        listenThread_ = 0;
    }

    if (acceptor_) {
        delete acceptor_;
        acceptor_ = NULL;
    }    
    
    for(CONN_MAP::iterator it = connections_.begin(); it != connections_.end(); it++){
        disconnectClient( it->first );
    }
}

