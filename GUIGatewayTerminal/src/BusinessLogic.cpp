/* 
 * File:   BusinessLogic.cpp
 * Author: cleo
 * 
 * Created on August 20, 2014, 10:01 AM
 */

#include <BusinessLogic.h>

#include <ConnectionsHandlers.h>
#include <support/ServicesManager.h>
#include <support/CapabilitiesManager.h>
#include <support/DataStorage.h>
#include <support/SubscriptionsManager.h>
#include <Parameters.h>

#include <synapse/Messages.h>


BusinessLogic::BusinessLogic() {
}

BusinessLogic::~BusinessLogic() throw() {
}

void BusinessLogic::GetUserHash() {

    Ident ident;
    ServiceDefinition def;
    
    if (SERVICES->getForeignServiceByType(ServiceType_UserValidation, def, ident)) {
        std::string usr, pwd;
        PARAMETERS->getValue("user", usr);
        PARAMETERS->getValue("password", pwd);                
        UserValidation request( def, usr, pwd );
        CONN_HANDLERS->sendMessageToClient( ident, (Packet&) request );
        std::cout << "User validation request sended!" << std::endl;
    } else {
        std::cout << "No user validation module available!" << std::endl;
    }
}

void BusinessLogic::SubscribeAll(){

    ModuleCapabilitiesList list = CAPABILITIES->getWideCapabilities();
    for (ModuleCapabilitiesList::iterator it = list.begin(); it != list.end(); it++) {
        CapabilitiesSet capSet = it->capabilities;
        for (CapabilitiesSet::iterator itcap = capSet.begin(); itcap != capSet.end(); itcap++) {
            if (!SUBSCRIPTIONS->existsSubscription(itcap->getId())) {
                Subscribe request(*itcap, STORAGE->getUserHash());
                CONN_HANDLERS->sendMessageToClient( it->module, (Packet&) request );
            }
        }    
    }

}

void BusinessLogic::UnsubscribeAll(){

    ModuleCapabilitiesList list = CAPABILITIES->getWideCapabilities();
    for (ModuleCapabilitiesList::iterator it = list.begin(); it != list.end(); it++) {
        CapabilitiesSet capSet = it->capabilities;
        for (CapabilitiesSet::iterator itcap = capSet.begin(); itcap != capSet.end(); itcap++) {
            if (SUBSCRIPTIONS->existsSubscription(itcap->getId())) {
                Unsubscribe request(*itcap, STORAGE->getUserHash());
                CONN_HANDLERS->sendMessageToClient( it->module, (Packet&) request );
            }
        }    
    }

}

void BusinessLogic::SetAnalogData(){
    int myChoice = 0;
    long value = 0;
    
    std::cout << "Insert CapabilityID: " << std::endl;
    std::cin >> myChoice;
    std::cout << "Insert value: " << std::endl;
    std::cin >> value;
    
    Ident module;
    CapabilityDefinition cap;
    if (CAPABILITIES->getForeignCapabilityById(myChoice, cap, module)){
        SetAnalogValue request(cap, STORAGE->getUserHash(), value);
        CONN_HANDLERS->sendMessageToClient( module, (Packet&) request ); 
        return;    
    }    
}
    
void BusinessLogic::SetDigitalData(){
    
    int myChoice = 0;
    int value = 0;
    
    std::cout << "Insert CapabilityID: " << std::endl;
    std::cin >> myChoice;
    std::cout << "Insert value [0-1]: " << std::endl;
    std::cin >> value;
    
    Ident module;
    CapabilityDefinition cap;
    if (CAPABILITIES->getForeignCapabilityById(myChoice, cap, module)){
        SetDigitalValue request(cap, STORAGE->getUserHash(), (value != 0));
        CONN_HANDLERS->sendMessageToClient( module, (Packet&) request ); 
        return;    
    }
}

void BusinessLogic::GetAnalogData(){
    
    int myChoice = 0;
    std::cout << "Insert CapabilityID: " << std::endl;
    std::cin >> myChoice;
    
    Ident module;
    CapabilityDefinition cap;
    if (CAPABILITIES->getForeignCapabilityById(myChoice, cap, module)){
        if ((cap.getType()== Capability_AnalogInput) || (cap.getType()== Capability_AnalogOutput)) {
            GetAnalogValue request(cap, STORAGE->getUserHash());
            CONN_HANDLERS->sendMessageToClient( module, (Packet&) request ); 
        }else {
            std::cout << "Invalid data type for Capability! " << std::endl;
        }
        return;    
    }    

}
    
void BusinessLogic::GetDigitalData(){
    
    int myChoice = 0;
    std::cout << "Insert CapabilityID: " << std::endl;
    std::cin >> myChoice;
    
    Ident module;
    CapabilityDefinition cap;
    if (CAPABILITIES->getForeignCapabilityById(myChoice, cap, module)){
        if ((cap.getType()== Capability_DigitalInput) || (cap.getType()== Capability_DigitalOutput)) {
            GetDigitalValue request(cap, STORAGE->getUserHash());
            CONN_HANDLERS->sendMessageToClient( module, (Packet&) request ); 
        }else {
            std::cout << "Invalid data type for Capability! " << std::endl;
        }
        return;    
    }   
}