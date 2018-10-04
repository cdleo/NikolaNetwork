/* 
 * File:   SynapseAcceptor.cpp
 * Author: cleo
 * 
 * Created on July 4, 2014, 3:55 PM
 */

#include <synapse/SynapseAcceptor.h>

SynapseAcceptor::SynapseAcceptor()
: server_( new Acceptor() )
{
}

SynapseAcceptor::~SynapseAcceptor(){
    server_->stopListen();
}

bool SynapseAcceptor::listen(unsigned int startPort, unsigned int endPort){
    
    unsigned int port = startPort;
    
    bool result;
    do {
        result = server_->startListen(port, 200);
        port++;    
    } while ((!result) && (port <= endPort));
    
    return result;
    
}

int SynapseAcceptor::accept(SynapseConnector* peer, int requestTimeout) {
    return server_->acceptRequest(peer, requestTimeout); 
}

