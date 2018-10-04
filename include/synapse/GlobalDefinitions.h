/* 
 * File:   GlobalDefinitions.h
 * Author: cleo
 *
 * Created on May 20, 2014, 9:27 AM
 */

#ifndef GLOBALDEFINITIONS_SYNAPSE_H
#define	GLOBALDEFINITIONS_SYNAPSE_H

#define	FIELD_SEPARATOR         '|'
#define	SUB_FIELD_SEPARATOR     '&'

#include <string>    
#include <string.h>

namespace Synapse {
    
    #define GATEWAY_PORT_DEFAULT              "9000"
    #define ACCEPTOR_BASE_PORT_DEFAULT        "8000"
    #define MAX_ACCEPTORS_PER_HOST_DEFAULT    "4"
    #define POLL_TIMEOUT_MS_DEFAULT           "1000"

    typedef enum MsgType {
        MsgType_Undefined = 0,
        MsgType_Request = 1,
        MsgType_Response = 2
    } MsgType;

    typedef enum CommandID {
        CommandID_Undefined = 0,
        CommandID_Ident = 1,
        CommandID_Services = 2,
        CommandID_Capabilities = 3,
        CommandID_Subscribe = 4,
        CommandID_Unsubscribe = 5,
        CommandID_Revoke = 6,
        CommandID_SetValue = 7,
        CommandID_GetValue = 8,
        CommandID_Update = 9,           //TODO: Revisar si se puede quitar el Update y manejar todo como si fuese GetValue
        CommandID_Ping = 10
    } CommandID;

    typedef enum SubCommandID {
        SubCommandID_Undefined = 0,
        SubCommandID_GetNetwork = 1,
        SubCommandID_GetUserHash = 2,
        SubCommandID_GetUserSecurityLevel = 3,
        SubCommandID_GetUserRevoked = 4,
        SubCommandID_AnalogValue = 5,
        SubCommandID_DigitalValue = 6,
        SubCommandID_ResourcesUpdate = 7
    } SubCommandID;    
    
    typedef enum CommandStatus {
        CommandResult_OK = 0,
        CommandResult_Failed = 1,
        CommandResult_Invalid = 2,
        CommandResult_UserInvalid = 3,
        CommandResult_InsufficientPrivileges = 4
    } CommandStatus;
    
    typedef enum CapabilitiesTypes {
        Capability_Undefined = 0,
        Capability_AnalogInput = 1,
        Capability_AnalogOutput = 2,
        Capability_DigitalInput = 3,
        Capability_DigitalOutput = 4,
        Capability_Users = 5,
        Capability_Network = 6,
        Capability_Notification = 7,
        Capability_Value = 8,
        Capability_List = 9    
    } CapabilitiesTypes;    
    
    typedef enum SourceData {
        SourceData_Undefined = 0,
        SourceData_Local = 1,
        SourceData_Foreign = 2,
        SourceData_All = 3
    } SourceData;     
    
    typedef enum ServiceTypes {
        ServiceType_Undefined = 0,
        ServiceType_Network = 1,
        ServiceType_UserValidation = 2,
        ServiceType_UserQuery = 3
    } ServiceTypes;      
    
    typedef enum AccessTypes {
        AccessType_Undefined = 0,
        AccessType_Read = 1,
        AccessType_Write = 2,
        AccessType_RW = 3
    } AccessTypes;

    typedef enum GetDataResult {
        GetDataResult_Undefined = 0,
        GetDataResult_Ok = 1,
        GetDataResult_UnknownCapability = 2,
        GetDataResult_UnknownService = 3,
        GetDataResult_NoDataAvailable = 4,
        GetDataResult_NoUserValidationModuleAvailable = 5,
        GetDataResult_PendingValidation = 6,
        GetDataResult_InsufficientPrivileges = 7,
        GetDataResult_UnableToGetValue = 8,
        GetDataResult_UserInvalid = 9,
        GetDataResult_GenericError = 10
    } GetDataResult; 

    typedef enum SetValueResult {
        SetValueResult_Undefined = 0,
        SetValueResult_Ok = 1,
        SetValueResult_UnknownCapability = 2,
        SetValueResult_NoUserValidationModuleAvailable = 3,
        SetValueResult_PendingValidation = 4,
        SetValueResult_InsufficientPrivileges = 5,
        SetValueResult_UnableToSetValue = 6,
        SetValueResult_UserInvalid = 7,
        SetValueResult_GenericError = 8
    } SetValueResult; 
    
    typedef enum SubscribeResult {
        SubscribeResult_Undefined = 0,
        SubscribeResult_Ok = 1,
        SubscribeResult_UnknownCapability = 2,
        SubscribeResult_CapabilityDoNotAdmitSubscription = 3,
        SubscribeResult_NoUserValidationModuleAvailable = 4,
        SubscribeResult_SubscriptionPending = 5,
        SubscribeResult_InsufficientPrivileges = 6,
        SubscribeResult_AlreadySubscribed = 7,
        SubscribeResult_GenericError = 8
    } SubscribeResult;    

    typedef enum UnsubscribeResult {
        UnsubscribeResult_Undefined = 0,
        UnsubscribeResult_Ok = 1,
        UnsubscribeResult_UnknownCapability = 2,
        UnsubscribeResult_SubscriptionRevoked = 3,
        UnsubscribeResult_UnsubscriptionPending = 4,
        UnsubscribeResult_NotSubscribedToCapability = 5
    } UnsubscribeResult;  

    typedef enum RevokeResult {
        RevokeResult_Undefined = 0,
        RevokeResult_Ok = 1,
        RevokeResult_UnableToRevoke = 2,
        RevokeResult_UnknownCapability = 3,
        RevokeResult_PendingValidation = 4,
        RevokeResult_InsufficientPrivileges = 5,
        RevokeResult_NotSubscribedToCapability = 6,
        RevokeResult_NoUserValidationModuleAvailable = 7,
        RevokeResult_GenericError = 8
    } RevokeResult;      
    
    typedef enum ValidationResult {
        ValidationResult_Undefined = 0,
        ValidationResult_Ok = 1,
        ValidationResult_UnknownUser = 2,
        ValidationResult_InvalidPassword = 3,
        ValidationResult_InvalidHash = 4,
        ValidationResult_GenericError = 5,
        ValidationResult_NoUserValidationModuleAvailable = 6
    } ValidationResult;      
    
    typedef enum DataContainerType {
        DataContainerType_Undefined = 0,
        DataContainerType_GenericList = 1,
        DataContainerType_Network = 2,
        DataContainerType_AnalogInput = 3,
        DataContainerType_AnalogOutput = 4,
        DataContainerType_DigitalInput = 5,
        DataContainerType_DigitalOutput = 6                
    } DataContainerType;     

    typedef enum NetworkItem_Status {
        NetworkItemStatus_Undefined = 0,
        NetworkItemStatus_Added = 1,
        NetworkItemStatus_Removed = 2
    } NetworkItem_Status;       
    
    typedef enum ModuleType {
        ModuleType_Undefined = 0,
        ModuleType_NetworkDiscover = 1,
        ModuleType_UserValidator = 2,
        ModuleType_Gateway = 3,
        ModuleType_IOTerminal = 4,
        ModuleType_ROTerminal = 5       
    } ModuleType;    

    typedef enum ResourceUpdateType {
        ResourceUpdateType_Undefined = 0,
        ResourceUpdateType_Services = 1,
        ResourceUpdateType_Capabilities = 2,
        ResourceUpdateType_Both = 3     
    } ResourceUpdateType;      
    
    
};

#endif	/* GLOBALDEFINITIONS_SYNAPSE_H */

