/* 
 * File:   ConnectionsManager.h
 * Author: cleo
 *
 * Created on August 11, 2014, 1:12 PM
 */

#ifndef CONNECTIONSMANAGER_H
#define	CONNECTIONSMANAGER_H

#include <threads/Thread.h>

#include <synapse/SynapseAcceptor.h>
#include <synapse/IUpdateCallbacks.h>
#include <synapse/IRequestCallbacks.h>
#include <synapse/IResponseCallbacks.h>
#include <synapse/IConnectionCallbacks.h>

#include <vector>
#include <string>
#include <map>

typedef struct Connection {
    Connection() : peer(NULL) {};
    
    Connection(const Connection& other){
        ident = other.ident;
        peer = other.peer;    
    }
    Ident               ident;
    SynapseConnector*   peer;
    
} Connection;

typedef std::map<std::string /*IP:Port*/, Connection /* connData */>  CONN_MAP;
typedef std::map<ModuleType, CONN_MAP>  MODULES_CONN_MAP;

class ConnectionsManager : public Thread {
public:
    ConnectionsManager();
    virtual ~ConnectionsManager() throw();
    
    void setConnectionCallbacks(ConnectionCallbacks* callbacks){
        connectionCallbacks_ = callbacks;
    }
    
    void setRequestCallbacks(RequestCallbacks* callbacks){
        requestCallbacks_ = callbacks;
    }

    void setResponseCallbacks(ResponseCallbacks* callbacks){
        responseCallbacks_ = callbacks;
    }

    void setUpdateCallbacks(UpdateCallbacks* callbacks){
        updateCallbacks_ = callbacks;
    }
    
    bool initialize(std::string certificateFile, std::string keyFile, unsigned int timeoutms);
    
    bool listen(unsigned int port, unsigned int maxAcceptorsPerHost);
    
    unsigned int getAcceptorPort(){ return (!acceptor_) ? 0 : acceptor_->getPort(); }
    
    bool connectToClient(std::string host, const int port, const int timeout);
    void disconnectClient(std::string host);
    
    void sendMessageToClient(std::string clientID, Packet& msg );
    void sendMessageToClient( Ident ident, Packet& msg );
    
    bool haveClients() { return (connections_.size() > 0); }
    
    void listenForClients();
    
    void run();    
    
private:
    std::string buildConnectorID(const char* host, const int port);
    
    void freeConnections();   
    
    MODULES_CONN_MAP                    conn_;
    CONN_MAP                            connections_;
    SynapseAcceptor*                    acceptor_;
    
    pthread_t                           listenThread_;
    pthread_mutex_t                     mutex_;  
    
    std::string                         certificateFile_;
    std::string                         keyFile_;     
    
    ConnectionCallbacks*                connectionCallbacks_;
    RequestCallbacks*                   requestCallbacks_;
    ResponseCallbacks*                  responseCallbacks_;
    UpdateCallbacks*                    updateCallbacks_;
    
};

#endif	/* CONNECTIONSMANAGER_H */

