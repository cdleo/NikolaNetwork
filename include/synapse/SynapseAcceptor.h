/* 
 * File:   SynapseAcceptor.h
 * Author: cleo
 *
 * Created on July 4, 2014, 3:55 PM
 */

#ifndef SYNAPSEACCEPTOR_H
#define	SYNAPSEACCEPTOR_H

#include <sockets/Acceptor.h>
#include <synapse/SynapseConnector.h>

class SynapseAcceptor {
public:
    SynapseAcceptor();
    virtual ~SynapseAcceptor();
    
    bool listen(unsigned int startPort, unsigned int endPort);
    
    int accept(SynapseConnector* peer, int requestTimeout);
    
    unsigned int getPort() { return (server_ && server_->isListening()) ? server_->getPort() : 0; };
    
private:
    Acceptor* server_;
};

#endif	/* SYNAPSEACCEPTOR_H */

