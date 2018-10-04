/* 
 * File:   NikolaGateway.cpp
 * Author: cleo
 * 
 * Created on July 29, 2014, 3:25 PM
 */

#include <NikolaGateway.h>
#include <BusinessLogic.h>
#include <GatewayConnHandlers.h>
#include <ConnectionsHandlers.h>

#include <BaseUtilities.h>

#include <support/CapabilitiesManager.h>

#include <synapse/PredefinedServicesAndCaps.h>

NikolaGateway::NikolaGateway(std::string routeToConfigFile) 
 : exitRequired_(false) {
    
    initializeSignalsManager();
    
    loadParameters(routeToConfigFile);
    
    if (!addPredefinedCapabilities())
        LOG->WARNING("[NikolaGateway::error] Unable to add predefined Capabilities! Exitting!");
    
    if (!CONN_HANDLERS->initialize())
        LOG->WARNING("[NikolaGateway::error] Unable to initialize connections! Exitting!");
    
    if (!GATEWAY_CONN_HANDLERS->initialize())
        LOG->WARNING("[NikolaGateway::error] Unable to initialize connections! Exitting!");    
}

NikolaGateway::~NikolaGateway() {

}

void NikolaGateway::initializeSignalsManager(){
    std::vector<int> signals;
    signals.push_back(SIGINT);
    SIGNALS->initialize(this, signals);
}

void NikolaGateway::loadParameters(std::string routeToConfigFile){
    
    if (routeToConfigFile.empty()) {
        std::string selfPath = UTILITIES->get_selfpath("./nikolagateway");
        routeToConfigFile.assign(selfPath).append(".conf");
    }
    
    if (!PARAMETERS->initialize(routeToConfigFile)){
        //Cargamos los valores por default
        PARAMETERS->setValue("moduleDescription","nikolaGateway");
        PARAMETERS->setValue("privateKey","/home/cleo/key.pem");
        PARAMETERS->setValue("certificate","/home/cleo/cert.pem");
        PARAMETERS->setValue("acceptorBasePort", ACCEPTOR_BASE_PORT_DEFAULT);
        PARAMETERS->setValue("maxAcceptorsPerHost", MAX_ACCEPTORS_PER_HOST_DEFAULT);         
        PARAMETERS->setValue("gatewayPrivateKey","/home/cleo/key.pem");
        PARAMETERS->setValue("gatewayCertificate","/home/cleo/cert.pem");
        PARAMETERS->setValue("gatewayPort", GATEWAY_PORT_DEFAULT);                 
    }
    
}

bool NikolaGateway::addPredefinedCapabilities(){
    CapabilityDefinition def;
    def.setId( ID_RESOURCESUPDATE_CAPABILITY );
    def.setType( Capability_Notification );
    def.setAccess( AccessType_Read );
    def.setAllowMultipleReadSubscriptions(true);
    def.setAllowMultipleWriteSubscriptions(false);
    def.setDescription( "Resources_Update" );
    def.setMinReadSecuritylevel(0);
    def.setMinWriteSecuritylevel(0);
    CAPABILITIES->addLocalCapability( def );
    
    return (CAPABILITIES->getLocalCapabilities().size() == 1);   
}

void NikolaGateway::onSignalTrapped(int signal_number){
    exitRequired_ = true;
}

void NikolaGateway::run() {
    
    while (!exitRequired_) {
        sleep(1);   
    }    
}
