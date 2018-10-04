/* 
 * File:   NetworkDiscovery.cpp
 * Author: cleo
 * 
 * Created on March 25, 2014, 2:18 PM
 */

#include <NetworkDiscovery.h>

#include <support/SubscriptionsManager.h>
#include <support/SubscriptorsManager.h>
#include <support/CapabilitiesManager.h>
#include <support/ServicesManager.h>

#include <DataStorage.h>
#include <ConnectionsHandlers.h>
#include <synapse/NetworkUpdate.h>
#include <BaseUtilities.h>

#include <iostream>
#include <stdexcept>

void* connThreadHandler(void* arg){
    NetworkDiscovery* pThis = static_cast<NetworkDiscovery*>(arg);
    pThis->tryConnections();
    pthread_exit(0); 
}

NetworkDiscovery::NetworkDiscovery(std::string routeToConfigFile) 
 : browser_(NULL), connThread_(0), exitRequired_(false)  {
    
    initializeSignalsManager();
    
    loadParameters(routeToConfigFile);
    
    if (!addPredefinedServices())
        LOG->WARNING("[NetworkDiscovery::error] Unable to set predefined services! Exitting!");
    
    if (!addPredefinedCapabilities())
        LOG->WARNING("[NetworkDiscovery::error] Unable to set predefined capabilities! Exitting!");    
    
    if (!CUSTOM_STORAGE->initialize())
        LOG->WARNING("[NetworkDiscovery::error] Unable to initialize storage! Exitting!");  
    
    if (!CONN_HANDLERS->initialize())
        LOG->WARNING("[NetworkDiscovery::error] Unable to initialize connections! Exitting!");
    
    browser_ = new NetworkBrowser();
}

NetworkDiscovery::~NetworkDiscovery() {
    if (connThread_ != 0) {
        pthread_join(connThread_, NULL);
        connThread_ = 0;
    }
    if(browser_){
        delete browser_;
        browser_ = NULL;
    }
}

void NetworkDiscovery::initializeSignalsManager(){
    std::vector<int> signals;
    signals.push_back( SIGINT );
    SIGNALS->initialize(this, signals);
}

void NetworkDiscovery::loadParameters(std::string routeToConfigFile){
    
    if (routeToConfigFile.empty()) {
        std::string selfPath = UTILITIES->get_selfpath("./networkdiscovery");
        routeToConfigFile.assign(selfPath).append(".conf");
    }
    
    if (!PARAMETERS->initialize(routeToConfigFile)){
        //Cargamos los valores por default
        PARAMETERS->setValue("moduleDescription","networkDiscover");
        PARAMETERS->setValue("privateKey","/home/cleo/key.pem");
        PARAMETERS->setValue("certificate","/home/cleo/cert.pem");
        PARAMETERS->setValue("pollTimeout",POLL_TIMEOUT_MS_DEFAULT);
        PARAMETERS->setValue("acceptorBasePort",ACCEPTOR_BASE_PORT_DEFAULT);
        PARAMETERS->setValue("maxAcceptorsPerHost",MAX_ACCEPTORS_PER_HOST_DEFAULT);        
        PARAMETERS->setValue("discoveryInterval","10");
    }
    
}

bool NetworkDiscovery::addPredefinedServices(){
    ServiceDefinition def;
    def.setId( ID_FULLNETWORK_SERVICE );
    def.setType( ServiceType_Network );
    def.setDescription( "NetworkMap" );  
    SERVICES->addLocalService( def );
    
    return (SERVICES->getLocalServices().size() == 1);   
}

bool NetworkDiscovery::addPredefinedCapabilities(){
    CapabilityDefinition def;
    def.setId( ID_NETWORKUPDATE_CAPABILITY );
    def.setType( Capability_Network );
    def.setAccess( AccessType_Read );    
    def.setMinReadSecuritylevel( 0 );
    def.setMinWriteSecuritylevel( 0 );
    def.setAllowMultipleReadSubscriptions(true);
    def.setAllowMultipleWriteSubscriptions(true);
    def.setDescription( "Network_Updates" ); 
    CAPABILITIES->addLocalCapability( def );
    
    return (CAPABILITIES->getLocalCapabilities().size() == 1);   
}

void NetworkDiscovery::onSignalTrapped(int signal_number){
    exitRequired_ = true;
    browser_->setExitFlag(exitRequired_);
}

void NetworkDiscovery::run() {

    long sleepTime;
    PARAMETERS->getValue("discoveryInterval",sleepTime);

    while(!exitRequired_) { 
        browser_->discover();
        
        if ( pthread_create (&connThread_, NULL, connThreadHandler, this) != 0 ) {   
            LOG->LogMessage(LOG_WARNING, "[NetworkDiscovery::run()] ERROR: Unable to create connections thread... (pthread_create())");
        }          
        
        #ifndef _DAEMON_
            ShowNetworkChanges();
        #endif

        InformNetworkChanges();

        sleep( sleepTime );

        if (connThread_ != 0) {
            pthread_join(connThread_, NULL);
            connThread_ = 0;
        }        
    }

}

void NetworkDiscovery::tryConnections() {

    long basePort, maxAcceptorsPerHost;
    PARAMETERS->getValue("acceptorBasePort",basePort);
    PARAMETERS->getValue("maxAcceptorsPerHost",maxAcceptorsPerHost);     
    
    MAP_NEIGHBOR networkMap = CUSTOM_STORAGE->getNetworkMap();
    for(MAP_NEIGHBOR::iterator it = networkMap.begin(); it != networkMap.end(); it++) {
        if ((it->second.lastStatus == HostStatus_BecomeOnline) || (it->second.lastStatus == HostStatus_Online)) {
            for(long port = basePort; port < (basePort + maxAcceptorsPerHost); port++) {
                
                if ( it->second.isLocalAddress && (port == (long) CONN_HANDLERS->getAcceptorPort()) ) //Soy yo mismo
                    continue;        
                
                if (exitRequired_) return;
                CONN_HANDLERS->connectToClient(it->first, port);
            }
        }
    }
}

void NetworkDiscovery::InformNetworkChanges() {

    CapabilityDefinition cap;
    if (!CAPABILITIES->getCapabilityById(ID_NETWORKUPDATE_CAPABILITY, SourceData_Local, cap))
        return;
    
    MAP_NEIGHBOR networkMap = CUSTOM_STORAGE->getNetworkMap();
    NetworkUpdate msg( cap );
    for (MAP_NEIGHBOR::iterator it = networkMap.begin(); it != networkMap.end(); it++) {
        if (it->second.lastStatus == HostStatus_BecomeOnline) {
            NetworkItem item(it->first.c_str(),NetworkItemStatus_Added);
            msg.addItem( item );
        } else if (it->second.lastStatus == HostStatus_NotResponding) {
            NetworkItem item(it->first.c_str(),NetworkItemStatus_Removed);
            msg.addItem( item );            
        }
    }
    
    if (msg.getNetworkList().size()) {
        SUBSCRIPTORS->onCapabilityUpdate( cap.getId(), (Packet&) msg );
    }
}

void NetworkDiscovery::ShowNetworkChanges() {
    
    bool haveNews = false;
    MAP_NEIGHBOR networkMap = CUSTOM_STORAGE->getNetworkMap();
    if (networkMap.size()) {
        for (MAP_NEIGHBOR::iterator it = networkMap.begin(); it != networkMap.end(); it++) {
            if (it->second.lastStatus == HostStatus_BecomeOnline) {
                std::cout << "Vecino Agregado: " << it->first.c_str() << std::endl;
                haveNews = true;
            } else if (it->second.lastStatus == HostStatus_NotResponding) {
                std::cout << "Vecino Eliminado: " << it->first.c_str() << std::endl;
                haveNews = true;
            }
        }  
        if (!haveNews)
            std::cout << "No se encontraron cambios en la red!" << std::endl;
    
    } else {
        std::cout << "No se encontraron \"vecinos\"!" << std::endl;
    }  
}
