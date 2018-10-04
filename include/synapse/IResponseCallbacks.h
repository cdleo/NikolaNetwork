/* 
 * File:   IResponseCallbacks.h
 * Author: cleo
 *
 * Created on August 11, 2014, 12:14 PM
 */

#ifndef IRESPONSECALLBACKS_H
#define	IRESPONSECALLBACKS_H

#include <synapse/Messages.h>

class SynapseConnector;

class ResponseCallbacks {
public:
    virtual void OnIdent_Response(IdentResp* response, SynapseConnector* peer) = 0;
    virtual void OnServices_Response(ServicesResp* response, SynapseConnector* peer) = 0;
    virtual void OnCapabilities_Response(CapabilitiesResp* response, SynapseConnector* peer) = 0;
    virtual void OnSubscription_Response(SubscribeResp* response, SynapseConnector* peer) = 0;
    virtual void OnUnsubscription_Response(UnsubscribeResp* response, SynapseConnector* peer) = 0;
    virtual void OnSetAnalogValue_Response(SetValueResp* response, SynapseConnector* peer) = 0;
    virtual void OnSetDigitalValue_Response(SetValueResp* response, SynapseConnector* peer) = 0;
    virtual void OnRevokeSubscription_Response(RevokeSubscriptionResp* request, SynapseConnector* peer) = 0;
    
    virtual void OnGetNetwork_Response(NetworkResp* response, SynapseConnector* peer) = 0;
    virtual void OnGetUserHash_Response(UserValidationResp* response, SynapseConnector* peer) = 0;
    virtual void OnGetUserSecurityLevel_Response(UserSecurityLevelQueryResp* response, SynapseConnector* peer) = 0;                 
    virtual void OnGetAnalogValue_Response(GetAnalogValueResp* response, SynapseConnector* peer) = 0;
    virtual void OnGetDigitalValue_Response(GetDigitalValueResp* response, SynapseConnector* peer) = 0;     
};


#endif	/* IRESPONSECALLBACKS_H */

