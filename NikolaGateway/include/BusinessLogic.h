/* 
 * File:   BusinessLogic.h
 * Author: cleo
 *
 * Created on August 20, 2014, 10:01 AM
 */

#ifndef BUSINESSLOGIC_H
#define	BUSINESSLOGIC_H

#include <GlobalDefinitions.h>
#include <SingletonBase.h>

#include <synapse/Messages.h>

#define LOGIC BusinessLogic::getInstance()

class BusinessLogic : public SingletonBase<BusinessLogic> {
public:
    friend class SingletonBase<BusinessLogic>;

    void OnSubscription_Request(Subscribe* request, std::string gatewayPeerID);
    
    void OnSubscription_Response(SubscribeResp* response, std::string peerID);
    
    void OnUnsubscription_Request(Unsubscribe* request, std::string gatewayPeerID);
    
    void OnUnsubscription_Response(UnsubscribeResp* response, std::string peerID);
    
    void OnSetAnalogValue_Request(SetAnalogValue* request, std::string gatewayPeerID);

    void OnSetDigitalValue_Request(SetDigitalValue* request, std::string gatewayPeerID);

    void OnSetValue_Response(SetValueResp* response, std::string peerID);
    
    void OnGetAnalogValue_Request(GetAnalogValue* request, std::string gatewayPeerID);
    
    void OnGetDigitalValue_Request(GetDigitalValue* request, std::string gatewayPeerID);    
    
    void OnGetValue_Response(GetValueResp* response, std::string peerID);
    
    void OnGetUserHash_Request(UserValidation* request, std::string gatewayPeerID );
    
    void OnGetUserHash_Response(UserValidationResp* response, std::string peerID );

private:
    BusinessLogic();
    virtual ~BusinessLogic() throw();    
    
    bool OnRequestOperation(Packet* request, CommandID commandID, SubCommandID subcommandID, 
                            long capabilityID, std::string userHash, std::string pendingPeer);
    
};

#endif	/* BUSINESSLOGIC_H */

