#include <GUIGatewayTerminal.h>

#include <ConnectionsHandlers.h>
#include <GlobalDefinitions.h>
#include <BusinessLogic.h>
#include <BaseUtilities.h>
#include <GUIManager.h>

#include <support/ServicesManager.h>

GUIGatewayTerminal::GUIGatewayTerminal(std::string routeToConfigFile, QWidget *parent)
 : QMainWindow(parent)
{
    loadParameters(routeToConfigFile);

    if (!addPredefinedServices()) {
        LOG->WARNING("[GatewayTerminal::error] Unable to set predefined services! Exitting!");
        exit(0);
    }  
    
    if (!CONN_HANDLERS->initialize ( )) {
        LOG->WARNING("[GatewayTerminal::error] Unable to initialize connections! Exitting!");
        std::cout << "[GatewayTerminal::error] Unable to initialize connections! Exitting!" << std::endl;
        exit(0);
    }    
    
    if (!GUI->initialize( this )) {
        LOG->WARNING("[GatewayTerminal::error] Unable to initialize GUI! Exitting!");
        std::cout << "[GatewayTerminal::error] Unable to initialize GUI! Exitting!" << std::endl;
        exit(0);
    } 
    
}

GUIGatewayTerminal::~GUIGatewayTerminal() {

}

void GUIGatewayTerminal::loadParameters(std::string routeToConfigFile){
    
    if (routeToConfigFile.empty()) {
        std::string selfPath = UTILITIES->get_selfpath(".GUIGatewayTerminal");
        routeToConfigFile.assign(selfPath).append(".conf");
    }
    
    if (!PARAMETERS->initialize(routeToConfigFile)){
        //Cargamos los valores por default
        PARAMETERS->setValue("moduleDescription","GUIGatewayTerminal");
        PARAMETERS->setValue("privateKey","/home/cleo/key.pem");
        PARAMETERS->setValue("certificate","/home/cleo/cert.pem");
        PARAMETERS->setValue("pollTimeout", POLL_TIMEOUT_MS_DEFAULT); 
        PARAMETERS->setValue("gatewayHost", "127.0.0.1");
        PARAMETERS->setValue("gatewayPort", GATEWAY_PORT_DEFAULT);         
        PARAMETERS->setValue("user", "user");
        PARAMETERS->setValue("password", "password");         
        PARAMETERS->setValue("resourcesPath", "./img");         
    }
    
}

bool GUIGatewayTerminal::addPredefinedServices(){
    ServiceDefinition def;
    def.setId( ID_CAPABILITIES_LIST_SERVICE );
    def.setType( ServiceType_Undefined );
    def.setDescription( "CapabilitiesList" );  
    SERVICES->addLocalService( def );
        
    def.setId( ID_LOAD_CONTROLS_SERVICE );
    def.setType( ServiceType_Undefined );
    def.setDescription( "Load" );  
    SERVICES->addLocalService( def );      
    
    def.setId( ID_SAVE_CONTROLS_SERVICE );
    def.setType( ServiceType_Undefined );
    def.setDescription( "Save" );  
    SERVICES->addLocalService( def );        
    
    return (SERVICES->getLocalServices().size() == 3);
}
