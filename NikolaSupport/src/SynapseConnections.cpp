/* 
 * File:   SynapseConnections.cpp
 * Author: cleo
 * 
 * Created on Aug 11, 2014, 11:09 AM
 */

#include <support/SynapseConnections.h>

#include <synapse/Messages.h>

using namespace Synapse;

SynapseConnections::SynapseConnections(RequestCallbacks* requestCallbacks, ResponseCallbacks* responseCallbacks, std::string certificateFile, std::string keyFile)
: acceptor_(NULL), requestCallbacks_(requestCallbacks), responseCallbacks_(responseCallbacks), listenThread_(0), readThread_(0), exitRequired_(false) {
    certificateFile_.assign(certificateFile);
    keyFile_.assign(keyFile);
    pthread_mutex_init(&mutex_, NULL);
}

SynapseConnections::~SynapseConnections() throw() {
    freeConnections();
    pthread_mutex_destroy(&mutex_);
}

bool SynapseConnections::connectToClient(std::string host, const int port, const int timeout) {

    std::string connIDAux = SynapseConnector::buildID(host.c_str(),port);

    SynapseConnector* peer;
    if (connections_.find( connIDAux ) != connections_.end()) {
        peer = connections_.at( connIDAux ).peer;
        if (peer->isConnected()){
            return true;
        } else {
            return peer->connectToHost(host.c_str(), port, timeout);
        }
        
    } else {
        peer = new SynapseConnector(certificateFile_.c_str(), keyFile_.c_str());           
        if (peer->connectToHost(host.c_str(), port, timeout)) {
            pthread_mutex_lock(&mutex_);
            Connection conn;
            strncpy(conn.ident.host, host.c_str(), sizeof(conn.ident.host) );
            conn.ident.port = port;
            conn.peer = peer;
            connections_.insert( std::pair<std::string,Connection>( connIDAux, conn ) );
            pthread_mutex_unlock(&mutex_);
            return true;
        }
        delete peer;
    }
    return false;
}

void SynapseConnections::disconnectClient(std::string peerID) {

    if (connections_.find(peerID) != connections_.end()) {
        pthread_mutex_lock(&mutex_);
        SynapseConnector* peer = connections_.at(peerID).peer;
        peer->disconnect();
        delete peer;
        connections_.erase(peerID);
        pthread_mutex_unlock(&mutex_);
    }
}

bool SynapseConnections::listen(unsigned int port, unsigned int maxAcceptorsPerHost){
    acceptor_ = new SynapseAcceptor();
    if (acceptor_->listen( port, port + maxAcceptorsPerHost )) {
        LOG->LogMessage(LOG_DEBUG,"Listening for clients on port %ud ", acceptor_->getPort());
        pthread_create (&listenThread_, NULL,  &SynapseConnections::listenStatic, this);
        //listenThread_ = new boost::thread( boost::bind(&SynapseConnections::listenForClients,this) );
        return (listenThread_ != 0);
    } else {
        LOG->WARNING("Unable to listen for clients");
        return false;
    }
}

void SynapseConnections::listenForClients() {

    SynapseConnector* peer = new SynapseConnector(certificateFile_.c_str(), keyFile_.c_str());    
    
    while (!exitRequired_) {
        if (acceptor_->accept(peer,5) == SOCKET_OK_CODE) {
            disconnectClient( peer->getID() );       
            pthread_mutex_lock(&mutex_);
            Connection conn;
            conn.peer = peer;            
            connections_.insert( std::pair<std::string,Connection>( peer->getID(), conn ) );
            pthread_mutex_unlock(&mutex_);
            peer = new SynapseConnector(certificateFile_.c_str(), keyFile_.c_str());    
        }
    }
}

bool SynapseConnections::start(){
    pthread_create (&readThread_, NULL, &SynapseConnections::getStatic, this);
    //readThread_ = new boost::thread( boost::bind(&SynapseConnections::getIncomingMessages,this) );
    return (readThread_);
}

void SynapseConnections::getIncomingMessages() {
    
    try {
        while (!exitRequired_) {
            if (connections_.empty()) {
                sleep(2);
                continue;
            }

            pthread_mutex_lock(&mutex_);
            CONN_MAP connectionsCopy = connections_;
            pthread_mutex_unlock(&mutex_);

            for(CONN_MAP::iterator it = connectionsCopy.begin(); it != connectionsCopy.end(); it++) {

                SynapseConnector* peer = it->second.peer;
                if (!peer->isConnected()) {
                    disconnectClient( peer->getID() );
                    continue;
                }

                Packet* packet = NULL;
                while (peer->read(packet, 1) > 0) {
                    if (packet->getMsgType() == MsgType_Request) {
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
                                peer->write( (Packet&)res, 1);  
                                break;
                            }

                            default:
                                 break;
                        }

                    } else if (packet->getMsgType() == MsgType_Response) {
                        switch (packet->getCommandID()) {
                            case CommandID_Ident:
                            {
                                if (connections_.find( peer->getID() ) != connections_.end()) {     //Actualizamos el resto de la informacion de Ident
                                    Connection conn = connections_.at( peer->getID() );
                                    connections_.erase( peer->getID() );

                                    conn.ident = ((IdentResp*)packet)->getIdent();
                                    peer->setID( SynapseConnector::buildID(conn.ident.host, conn.ident.port) );
                                    connections_.insert( std::pair<std::string,Connection>( peer->getID(), conn ) );
                                }
                                responseCallbacks_->OnIdent_Response((IdentResp*)packet, peer);
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
                                switch(packet->getSubCommandID()) {          
                                    case SubCommandID_GetNetwork:
                                        responseCallbacks_->OnNetwork_Update((NetworkUpdate*) packet, peer);
                                        break;                                      

                                    case SubCommandID_AnalogValue:
                                        responseCallbacks_->OnAnalogValue_Update((AnalogValueUpdate*) packet, peer);
                                        break;                                                                          

                                    case SubCommandID_DigitalValue:
                                        responseCallbacks_->OnDigitalValue_Update((DigitalValueUpdate*) packet, peer);
                                        break;                                                                          

                                    default:
                                        break;                   
                                }  
                            }                                              


                            case CommandID_Ping: //El ping lo procesamos en esta capa
                            default:
                                 break;                        
                        }

                    } 

                    delete packet;
                }
            }

            //pthread_mutex_unlock(&mutex_);
            sleep(1);
        }
    } catch(std::runtime_error& er) {
        std::cout << "SynapseConnections::getIncomingMessages() ERROR: " << er.what() << std::endl;
        
    } catch (std::exception& ex) {
        std::cout << "SynapseConnections::getIncomingMessages() ERROR: " << ex.what() << std::endl;
    }
}

void SynapseConnections::sendMessageToClient( std::string clientID, Packet& msg, const int timeoutSeconds ) {

    if (connections_.find(clientID) != connections_.end()){
        SynapseConnector* peer = connections_.at(clientID).peer;
        if (peer->write( msg, timeoutSeconds ) <= 0){
            disconnectClient( clientID );
        }
    }
}

void SynapseConnections::freeConnections() {
    
    exitRequired_ = true;
  
    LOG->DEBUG("[SynapseConnections::freeConnections] Closing connections... ");
    
    if (listenThread_) {
        pthread_join(listenThread_, NULL);
        listenThread_ = 0;
    }
    
    if (readThread_) {
        pthread_join(readThread_, NULL);
        readThread_ = 0;
    }    

    if (acceptor_) {
        delete acceptor_;
        acceptor_ = NULL;
    }    
    
    for(CONN_MAP::iterator it = connections_.begin(); it != connections_.end(); it++){
        disconnectClient( it->first );
    }
}

