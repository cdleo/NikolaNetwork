/* 
 * File:   CapabilityModule.cpp
 * Author: cleo
 * 
 * Created on July 29, 2014, 3:25 PM
 */

#include <CapabilityModule.h>

#include <ConnectionsHandlers.h>
#include <support/CapabilitiesManager.h>
#include <support/SubscriptorsManager.h>
#include <support/ServicesManager.h>
#include <support/ActionsManager.h>
#include <support/DataStorage.h>

#include <BaseUtilities.h>

CapabilityModule::CapabilityModule(std::string routeToConfigFile) 
 : exitRequired_(false) {
    
    initializeSignalsManager();
    
    loadParameters(routeToConfigFile);
    
    SERVICES->load();
    
    CAPABILITIES->load();
    
    ACTIONS->load();
   
    if (!CONN_HANDLERS->initialize())
        LOG->WARNING("[NetworkDiscovery::error] Unable to initialize connections! Exitting!");    
}

CapabilityModule::~CapabilityModule() {

}

void CapabilityModule::initializeSignalsManager(){
    std::vector<int> signals;
    signals.push_back( SIGINT );
    SIGNALS->initialize(this, signals);
}

void CapabilityModule::loadParameters(std::string routeToConfigFile){
    
    if (routeToConfigFile.empty()) {
        std::string selfPath = UTILITIES->get_selfpath("./capabilitymodule");
        routeToConfigFile.assign(selfPath).append(".conf");
    }
    
    if (!PARAMETERS->initialize(routeToConfigFile)){
        //Cargamos los valores por default
        PARAMETERS->setValue("moduleDescription","capabilityModule");
        PARAMETERS->setValue("privateKey","/home/cleo/key.pem");
        PARAMETERS->setValue("certificate","/home/cleo/cert.pem");
        PARAMETERS->setValue("pollTimeout", POLL_TIMEOUT_MS_DEFAULT); 
        PARAMETERS->setValue("acceptorBasePort", ACCEPTOR_BASE_PORT_DEFAULT);
        PARAMETERS->setValue("maxAcceptorsPerHost", MAX_ACCEPTORS_PER_HOST_DEFAULT);         
        PARAMETERS->setValue("user", "user");
        PARAMETERS->setValue("password", "password");         
    }
    
}

void CapabilityModule::onSignalTrapped(int signal_number){
    exitRequired_ = true;
}

void CapabilityModule::run() {
    
    while (!exitRequired_) {
        sleep(1);
    }    
}
