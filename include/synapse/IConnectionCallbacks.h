/* 
 * File:   IConnectionCallbacks.h
 * Author: cleo
 *
 * Created on August 26, 2014, 3:04 PM
 */

#ifndef ICONNECTIONCALLBACKS_H
#define	ICONNECTIONCALLBACKS_H

class SynapseConnector;

class ConnectionCallbacks {
public:
    virtual void OnClient_Connect(SynapseConnector* peer) = 0;
    virtual void OnClient_Disconnect(SynapseConnector* peer) = 0;
};

#endif	/* ICONNECTIONCALLBACKS_H */

