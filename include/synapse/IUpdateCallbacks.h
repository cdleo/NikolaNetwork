/* 
 * File:   IUpdateCallbacks.h
 * Author: cleo
 *
 * Created on September 30, 2014, 2:12 PM
 */

#ifndef IUPDATECALLBACKS_H
#define	IUPDATECALLBACKS_H

#include <synapse/Messages.h>

class SynapseConnector;

class UpdateCallbacks {
public:
    virtual void OnNetwork_Update(NetworkUpdate* update, SynapseConnector* peer) = 0;
    virtual void OnResources_Update(ResourcesUpdate* update, SynapseConnector* peer) = 0;
    virtual void OnRevokedUser_Update(RevokedUserUpdate* update, SynapseConnector* peer) = 0;
    virtual void OnAnalogValue_Update(AnalogValueUpdate* update, SynapseConnector* peer) = 0;
    virtual void OnDigitalValue_Update(DigitalValueUpdate* update, SynapseConnector* peer) = 0;    
    
};

#endif	/* IUPDATECALLBACKS_H */

