/* 
 * File:   BusinessLogicManager.cpp
 * Author: cleo
 * 
 * Created on August 14, 2014, 11:35 AM
 */

#include <BusinessLogicManager.h>

#include <support/PendingOperationsManager.h>
#include <support/SubscriptorsManager.h>
#include <support/CapabilitiesManager.h>
#include <support/ServicesManager.h>
#include <support/DataStorage.h>
#include <ConnectionsHandlers.h>

BusinessLogicManager::BusinessLogicManager() {
}

BusinessLogicManager::~BusinessLogicManager() throw() {
}

void BusinessLogicManager::SetAnalogValue_Request(SetAnalogValue* request, std::string peerID){

    SetValueResult result = SetValueResult_Undefined;
    CapabilityDefinition def = request->getCapability();
    if (CAPABILITIES->existCapability(request->getCapability())){
        if ((def.getAccess() == AccessType_Write) || (def.getAccess() == AccessType_RW)) {
            if (def.getMinReadSecuritylevel() == 0) {
                result = STORAGE->setValue(def.getId(), request->getValue());
            } else {
                long userLevel;
                if (!STORAGE->getUserSecurityLevel( request->getUserHash(), userLevel )) {
                    Ident serviceHostIdent;
                    ServiceDefinition service;
                    if (!SERVICES->getForeignServiceByType(ServiceType_UserQuery, service, serviceHostIdent)) {
                        result = SetValueResult_NoUserValidationModuleAvailable;
                        
                    } else { 
                        GetServiceData userRequest(SubCommandID_GetUserSecurityLevel, service);
                        CONN_HANDLERS->sendMessageToClient( serviceHostIdent, (Packet&) userRequest);
                        
                        if (PENDING->addPendingOperation(request, peerID)){
                            result = SetValueResult_PendingValidation;
                        } else {
                            result = SetValueResult_GenericError;
                        }
                    }
                    
                } else if (def.getMinWriteSecuritylevel() > (unsigned int) userLevel){
                    result = SetValueResult_InsufficientPrivileges;
                } else {
                    result = STORAGE->setValue(def.getId(), request->getValue());
                }
            }
                
        } else {
            result = SetValueResult_UnableToSetValue;
        }
        
    } else {
        result =  SetValueResult_UnknownCapability;
    }    
    
    SetValueResp response(request->getSubCommandID(), request->getCapability(),request->getUserHash(), result);
    CONN_HANDLERS->sendMessageToClient(peerID, (Packet&) response);
}

void BusinessLogicManager::SetDigitalValue_Request(SetDigitalValue* request, std::string peerID){
    
    SetValueResult result = SetValueResult_Undefined;
    CapabilityDefinition def = request->getCapability();
    if (CAPABILITIES->existCapability(request->getCapability())){
        if ((def.getAccess() == AccessType_Write) || (def.getAccess() == AccessType_RW)) {
            if (def.getMinWriteSecuritylevel() == 0) {
                result = STORAGE->setValue(def.getId(), request->getValue());
            } else {
                long userLevel;
                if (!STORAGE->getUserSecurityLevel( request->getUserHash(), userLevel )) {
                    Ident serviceHostIdent;
                    ServiceDefinition service;
                    if (!SERVICES->getForeignServiceByType(ServiceType_UserQuery, service, serviceHostIdent)) {
                        result = SetValueResult_NoUserValidationModuleAvailable;
                        
                    } else { 
                        GetServiceData userRequest(SubCommandID_GetUserSecurityLevel, service);
                        CONN_HANDLERS->sendMessageToClient( serviceHostIdent, (Packet&) userRequest);
                        
                        if (PENDING->addPendingOperation(request, peerID)){
                            result = SetValueResult_PendingValidation;
                        } else {
                            result = SetValueResult_GenericError;
                        }
                    }
                    
                } else if (def.getMinWriteSecuritylevel() > (unsigned int) userLevel){
                    result = SetValueResult_InsufficientPrivileges;
                } else {
                    result = STORAGE->setValue(def.getId(), request->getValue());
                }
            }
                
        } else {
            result = SetValueResult_UnableToSetValue;
        }
        
    } else {
        result =  SetValueResult_UnknownCapability;
    }    
    
    SetValueResp response(request->getSubCommandID(), request->getCapability(), request->getUserHash(), result);
    CONN_HANDLERS->sendMessageToClient(peerID, (Packet&) response);  
}

void BusinessLogicManager::GetAnalogValue_Request(GetAnalogValue* request, std::string peerID){
    
    GetDataResult result = GetDataResult_Undefined;
    long value = 0;
    CapabilityDefinition def = request->getCapability();
    if (CAPABILITIES->existCapability(request->getCapability())){
        if ((def.getAccess() == AccessType_Read) || (def.getAccess() == AccessType_RW)) {
            if (def.getMinReadSecuritylevel() == 0) {
                result = STORAGE->getValue(def.getId(), value);
            } else {
                long userLevel;
                if (!STORAGE->getUserSecurityLevel( request->getUserHash(), userLevel )) {
                    Ident serviceHostIdent;
                    ServiceDefinition service;
                    if (!SERVICES->getForeignServiceByType(ServiceType_UserQuery, service, serviceHostIdent)) {
                        result = GetDataResult_NoUserValidationModuleAvailable;
                        
                    } else { 
                        GetServiceData userRequest(SubCommandID_GetUserSecurityLevel, service);
                        CONN_HANDLERS->sendMessageToClient( serviceHostIdent, (Packet&) userRequest);
                        
                        if (PENDING->addPendingOperation(request, peerID)){
                            result = GetDataResult_PendingValidation;
                        } else {
                            result = GetDataResult_GenericError;
                        }
                    }
                    
                } else if (def.getMinReadSecuritylevel() > (unsigned int) userLevel){
                    result = GetDataResult_InsufficientPrivileges;
                } else {
                    result = STORAGE->getValue(def.getId(), value);
                }
            }
                
        } else {
            result = GetDataResult_UnableToGetValue;
        }
        
    } else {
        result =  GetDataResult_UnknownCapability;
    }    
    
    GetAnalogValueResp response(request->getCapability(), request->getUserHash(), result, value);
    CONN_HANDLERS->sendMessageToClient(peerID, (Packet&) response);     
    
}

void BusinessLogicManager::GetDigitalValue_Request(GetDigitalValue* request, std::string peerID){
    
    CapabilityDefinition def = request->getCapability();
    std::string userHash = request->getUserHash();
    GetDataResult result = GetDataResult_Undefined;
    bool value = false;
    
    if (CAPABILITIES->existCapability(request->getCapability())){
        if ((def.getAccess() == AccessType_Read) || (def.getAccess() == AccessType_RW)) {
            if (def.getMinReadSecuritylevel() == 0) {
                result = STORAGE->getValue(def.getId(), value);
            } else {
                long userLevel;
                if (!STORAGE->getUserSecurityLevel( userHash, userLevel )) {
                    Ident serviceHostIdent;
                    ServiceDefinition service;
                    if (!SERVICES->getForeignServiceByType(ServiceType_UserQuery, service, serviceHostIdent)) {
                        result = GetDataResult_NoUserValidationModuleAvailable;
                        
                    } else { 
                        GetServiceData userRequest(SubCommandID_GetUserSecurityLevel, service);
                        CONN_HANDLERS->sendMessageToClient( serviceHostIdent, (Packet&) userRequest);
                        
                        if (PENDING->addPendingOperation(request, peerID)){
                            result = GetDataResult_PendingValidation;
                        } else {
                            result = GetDataResult_GenericError;
                        }
                    }
                    
                } else if (def.getMinReadSecuritylevel() > (unsigned int) userLevel){
                    result = GetDataResult_InsufficientPrivileges;
                } else {
                    result = STORAGE->getValue(def.getId(), value);
                }
            }
                
        } else {
            result = GetDataResult_UnableToGetValue;
        }
        
    } else {
        result =  GetDataResult_UnknownCapability;
    }    
    
    GetDigitalValueResp response(def, userHash, result, value);
    CONN_HANDLERS->sendMessageToClient(peerID, (Packet&) response);     
    
}

void BusinessLogicManager::Subscription_Request(Subscribe* request, std::string peerID){

    SubscribeResult result = SubscribeResult_Undefined;
    CapabilityDefinition def = request->getCapability();
    if (CAPABILITIES->existCapability(def)){
        if ((def.getAccess() == AccessType_Read) || (def.getAccess() == AccessType_RW)) {
            if (def.getMinReadSecuritylevel() == 0) {
                result = SUBSCRIPTORS->addSubscription(def.getId(), request->getUserHash(), peerID);
                
            } else {
                long userLevel;
                if (!STORAGE->getUserSecurityLevel( request->getUserHash(), userLevel )) {
                    Ident serviceHostIdent;
                    ServiceDefinition service;
                    if (!SERVICES->getForeignServiceByType(ServiceType_UserQuery, service, serviceHostIdent)) {
                        result = SubscribeResult_NoUserValidationModuleAvailable;
                        
                    } else { 
                        UserSecurityLevelQuery userRequest(service, request->getUserHash());
                        CONN_HANDLERS->sendMessageToClient( serviceHostIdent, (Packet&) userRequest);
                        
                        if (PENDING->addPendingOperation(request, peerID)){
                            result = SubscribeResult_SubscriptionPending;
                        } else {
                            result = SubscribeResult_GenericError;
                        }
                    }
                    
                } else if (def.getMinReadSecuritylevel() > (unsigned int) userLevel){
                    result = SubscribeResult_InsufficientPrivileges;
                } else {
                    result = SUBSCRIPTORS->addSubscription(def.getId(), request->getUserHash(), peerID);
                }
            }
                
        } else {
            result = SubscribeResult_CapabilityDoNotAdmitSubscription;
        }
        
    } else {
        result =  SubscribeResult_UnknownCapability;
    }    
    
    SubscribeResp response(request->getCapability(), request->getUserHash(), result);
    CONN_HANDLERS->sendMessageToClient(peerID, (Packet&) response);     
    
}

void BusinessLogicManager::Revoke_Request(RevokeSubscription* request, std::string peerID){
    
    RevokeResult result = RevokeResult_Undefined;
    CapabilityDefinition def = request->getCapability();
    if (CAPABILITIES->existCapability(def)){
        std::string currentUserSubscribed;
        if (!SUBSCRIPTORS->isSubscribed(def.getId(), request->getPeerIDRevoked(), currentUserSubscribed)) {
            result = RevokeResult_NotSubscribedToCapability;

        } else {
            long userLevel = -1;
            long revokedUserLevel = -1;
            if ( (!STORAGE->getUserSecurityLevel( request->getUserHash(), userLevel )) ||
                 (!STORAGE->getUserSecurityLevel( currentUserSubscribed, revokedUserLevel )) ) {
                Ident serviceHostIdent;
                ServiceDefinition service;
                if (!SERVICES->getForeignServiceByType(ServiceType_UserQuery, service, serviceHostIdent)) {
                    result = RevokeResult_NoUserValidationModuleAvailable;

                } else {
                    if (userLevel < 0) {
                        UserSecurityLevelQuery userRequest(service, request->getUserHash());
                        CONN_HANDLERS->sendMessageToClient( serviceHostIdent, (Packet&) userRequest);
                    }

                    if (revokedUserLevel < 0) {
                        UserSecurityLevelQuery userRequest(service, currentUserSubscribed);
                        CONN_HANDLERS->sendMessageToClient( serviceHostIdent, (Packet&) userRequest);
                    }                        

                    if (PENDING->addPendingOperation(request, peerID)){
                        result = RevokeResult_PendingValidation;
                    } else {
                        result = RevokeResult_GenericError;
                    }
                }

            } else if (userLevel > revokedUserLevel){
                if (SUBSCRIPTORS->removeSubscription(def.getId(), currentUserSubscribed, request->getPeerIDRevoked()) == UnsubscribeResult_Ok){
                    UnsubscribeResp resp(def, currentUserSubscribed, UnsubscribeResult_SubscriptionRevoked);
                    CONN_HANDLERS->sendMessageToClient( request->getPeerIDRevoked(), (Packet&) resp);
                    result = RevokeResult_Ok;
                } else {
                    result = RevokeResult_UnableToRevoke;
                }

            } else {
                result = RevokeResult_InsufficientPrivileges;
            }
        }
        
    } else {
        result = RevokeResult_UnknownCapability;
    }    
    
    RevokeSubscriptionResp response(request->getCapability(), result);
    CONN_HANDLERS->sendMessageToClient(peerID, (Packet&) response);      
    
}

void BusinessLogicManager::OnUserSecurityLevelReceived(ValidationResult result, std::string userHash, int securityLevel){
    
    PendingOperationsList pendingList;
    PENDING->getPendingOperationsForUser( userHash, pendingList );
    
    if (result == ValidationResult_Ok) {
        STORAGE->setUserSecurityLevel(userHash, securityLevel);
        
        for ( PendingOperationsList::iterator it = pendingList.begin(); it != pendingList.end(); it++ ) {

            switch (it->operation->getCommandID()){
                case CommandID_Subscribe:
                    this->Subscription_Request( (Subscribe*) it->operation, it->requestedBy);
                    break;                    

                case CommandID_Revoke:
                    this->Revoke_Request( (RevokeSubscription*) it->operation, it->requestedBy);
                    break; 
                    
                case CommandID_SetValue:
                    switch (it->operation->getSubCommandID()){
                        case SubCommandID_AnalogValue:
                            this->SetAnalogValue_Request( (SetAnalogValue*) it->operation, it->requestedBy);
                            break;

                        case SubCommandID_DigitalValue:
                            this->SetDigitalValue_Request( (SetDigitalValue*) it->operation, it->requestedBy);
                            break;

                        default:
                            break;
                    }
                    break;

                case CommandID_GetValue:
                    switch (it->operation->getSubCommandID()){
                        case SubCommandID_AnalogValue:
                            this->GetAnalogValue_Request( (GetAnalogValue*) it->operation, it->requestedBy);
                            break;

                        case SubCommandID_DigitalValue:
                            this->GetDigitalValue_Request( (GetDigitalValue*) it->operation, it->requestedBy);
                            break;

                        default:
                            break;
                    }
                    break;

                default:
                    break;
            }        
        }
        
    } else {
        for ( PendingOperationsList::iterator it = pendingList.begin(); it != pendingList.end(); it++ ){
            
            switch (it->operation->getCommandID()){
                case CommandID_Subscribe:
                {
                    Subscribe* subscribe = (Subscribe*) it->operation;
                    SubscribeResp response( subscribe->getCapability(), subscribe->getUserHash(), SubscribeResult_InsufficientPrivileges);
                    CONN_HANDLERS->sendMessageToClient(it->requestedBy, (Packet&) response);                     
                    break;   
                }
                
                case CommandID_Revoke:
                {
                    RevokeSubscriptionResp response( ((RevokeSubscription*) it->operation)->getCapability(), RevokeResult_InsufficientPrivileges);
                    CONN_HANDLERS->sendMessageToClient(it->requestedBy, (Packet&) response);                     
                    break;   
                }                
                
                case CommandID_SetValue:
                {
                    SetValue* setvalue = (SetValue*) it->operation;
                    SetValueResp response(it->operation->getSubCommandID(), setvalue->getCapability(), setvalue->getUserHash(), SetValueResult_UserInvalid);
                    CONN_HANDLERS->sendMessageToClient(it->requestedBy, (Packet&) response);                     
                    break;
                }
                
                case CommandID_GetValue:
                {
                    GetValue* getvalue = (GetValue*) it->operation;
                    GetValueResp* response = NULL;
                    switch (getvalue->getSubCommandID()){
                        case SubCommandID_AnalogValue:
                            response = new GetAnalogValueResp( getvalue->getCapability(), getvalue->getUserHash(), GetDataResult_UserInvalid, 0);
                            break;

                        case SubCommandID_DigitalValue:
                            response = new GetDigitalValueResp( getvalue->getCapability(), getvalue->getUserHash(), GetDataResult_UserInvalid, false);
                            break;

                        default:
                            break;
                    }
                    
                    if (response != NULL) {
                        CONN_HANDLERS->sendMessageToClient(it->requestedBy, (Packet&) *response);                     
                        delete response;
                    }
                    break;
                }
                
                default:
                    break;
            }
        }
    }
    
    PENDING->erasePendingOperationsForUser( userHash, pendingList );
}
