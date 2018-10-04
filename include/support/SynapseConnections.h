/* 
 * File:   SynapseConnections.h
 * Author: cleo
 *
 * Created on August 11, 2014, 1:12 PM
 */

#ifndef SYNAPSECONNECTIONS_H
#define	SYNAPSECONNECTIONS_H

#include <synapse/SynapseAcceptor.h>
#include <synapse/IRequestCallbacks.h>
#include <synapse/IResponseCallbacks.h>

#include <SingletonBase.h>
#include <boost/thread.hpp>
#include <vector>
#include <string>

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

class SynapseConnections {
public:
    SynapseConnections(RequestCallbacks* requestCallbacks, ResponseCallbacks* responseCallbacks, std::string certificateFile, std::string keyFile);
    virtual ~SynapseConnections() throw();

    bool listen(unsigned int port, unsigned int maxAcceptorsPerHost);
    
    bool start();
    
    unsigned int getAcceptorPort(){ return (!acceptor_) ? 0 : acceptor_->getPort(); }
    
    bool connectToClient(std::string host, const int port, const int timeout);
    void disconnectClient(std::string host);
    
    void sendMessageToClient(std::string clientID, Packet& msg, const int timeoutSeconds );
    
    bool haveClients() { return (connections_.size() > 0); }
    
private:  
    static void* listenStatic(void* context) {
        ( (SynapseConnections*) context)->listenForClients();
        return NULL;
    }    

    static void* getStatic(void* context) {
        ( (SynapseConnections*) context)->getIncomingMessages();
        return NULL;
    }    
    
    void listenForClients();
    void getIncomingMessages();
    
    void freeConnections();   
    
    MODULES_CONN_MAP                    conn_;
    CONN_MAP                            connections_;
    SynapseAcceptor*                    acceptor_;
    
    RequestCallbacks*                   requestCallbacks_;
    ResponseCallbacks*                  responseCallbacks_;
    
    pthread_t                           listenThread_;
    pthread_t                           readThread_;
    //boost::thread*                      listenThread_;
    //boost::thread*                      readThread_;
    pthread_mutex_t                     mutex_;
    //boost::mutex                        mutex_;    
    
    std::string                         certificateFile_;
    std::string                         keyFile_;
    
    bool                                exitRequired_;
};

#endif	/* SYNAPSECONNECTIONS_H */

