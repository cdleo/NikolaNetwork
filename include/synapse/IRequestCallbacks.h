/* 
 * File:   IRequestCallbacks.h
 * Author: cleo
 *
 * Created on August 11, 2014, 12:14 PM
 */

#ifndef IREQUESTCALLBACKS_H
#define	IREQUESTCALLBACKS_H

#include <synapse/Messages.h>

class SynapseConnector;

class RequestCallbacks {
public:
    virtual void OnIdent_Request(Request* request, SynapseConnector* peer) = 0;
    virtual void OnServices_Request(Request* request, SynapseConnector* peer) = 0;
    virtual void OnCapabilities_Request(Request* request, SynapseConnector* peer) = 0;
    virtual void OnSubscription_Request(Subscribe* request, SynapseConnector* peer) = 0;
    virtual void OnUnsubscription_Request(Unsubscribe* request, SynapseConnector* peer) = 0;
    virtual void OnSetAnalogValue_Request(SetAnalogValue* request, SynapseConnector* peer) = 0;
    virtual void OnSetDigitalValue_Request(SetDigitalValue* request, SynapseConnector* peer) = 0;
    virtual void OnRevokeSubscription_Request(RevokeSubscription* request, SynapseConnector* peer) = 0;
    
    virtual void OnGetNetwork_Request(GetServiceData* request, SynapseConnector* peer) = 0;
    virtual void OnGetUserHash_Request(UserValidation* request, SynapseConnector* peer) = 0;
    virtual void OnGetUserSecurityLevel_Request(UserSecurityLevelQuery* request, SynapseConnector* peer) = 0;                 
    virtual void OnGetAnalogValue_Request(GetAnalogValue* request, SynapseConnector* peer) = 0;
    virtual void OnGetDigitalValue_Request(GetDigitalValue* request, SynapseConnector* peer) = 0;
    
};

#endif	/* IREQUESTCALLBACKS_H */

