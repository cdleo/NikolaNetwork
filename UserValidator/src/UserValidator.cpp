/* 
 * File:   UserValidator.cpp
 * Author: cleo
 * 
 * Created on July 29, 2014, 3:25 PM
 */

#include <UserValidator.h>

#include <support/CapabilitiesManager.h>
#include <support/ServicesManager.h>
#include <DatabaseManager.h>
#include <DataStorage.h>

#include <BaseUtilities.h>

UserValidator::UserValidator(std::string routeToConfigFile) 
 : exitRequired_(false) {
    
    initializeSignalsManager();
    
    loadParameters(routeToConfigFile);
    
    if (!DB->connect())
        LOG->WARNING("[UserValidator::error] Unable to connect to database! Exitting!");
        
    if (!CUSTOM_STORAGE->initialize())
        LOG->WARNING("[UserValidator::error] Unable to initialize storage! Exitting!");
    
    if (!addPredefinedServices())
        LOG->WARNING("[UserValidator::error] Unable to set predefined services! Exitting!");
    
    if (!addPredefinedCapabilities())
        LOG->WARNING("[UserValidator::error] Unable to set predefined capabilities! Exitting!");
    
    if (!CONN_HANDLERS->initialize())
        LOG->WARNING("[UserValidator::error] Unable to initialize connections! Exitting!");
}

UserValidator::~UserValidator() {
    DB->disconnect();
}

void UserValidator::initializeSignalsManager(){
    std::vector<int> signals;
    signals.push_back( SIGINT );
    SIGNALS->initialize(this, signals);
}

void UserValidator::loadParameters(std::string routeToConfigFile){
    
    if (routeToConfigFile.empty()) {
        std::string selfPath = UTILITIES->get_selfpath("./uservalidator");
        routeToConfigFile.assign(selfPath).append(".conf");
    }
    
    if (!PARAMETERS->initialize(routeToConfigFile)){
        //Cargamos los valores por default
        PARAMETERS->setValue("moduleDescription","userValidator");
        PARAMETERS->setValue("privateKey","/home/cleo/key.pem");
        PARAMETERS->setValue("certificate","/home/cleo/cert.pem");
        PARAMETERS->setValue("pollTimeout", POLL_TIMEOUT_MS_DEFAULT); 
        PARAMETERS->setValue("acceptorBasePort",ACCEPTOR_BASE_PORT_DEFAULT);
        PARAMETERS->setValue("maxAcceptorsPerHost",MAX_ACCEPTORS_PER_HOST_DEFAULT);      
        PARAMETERS->setValue("user", "");
        PARAMETERS->setValue("password", "");          
        PARAMETERS->setValue("dbHost","localhost");
        PARAMETERS->setValue("dbName","Nikola");
        PARAMETERS->setValue("dbUsr","nikola");
        PARAMETERS->setValue("dbPwd","Nikola2014");
    }
    
}

bool UserValidator::addPredefinedServices(){
    ServiceDefinition def;
    def.setId( ID_USER_VALIDATION_SERVICE );
    def.setType( ServiceType_UserValidation );
    def.setDescription( "UserValidation" );  
    SERVICES->addLocalService( def );
    
    def.setId( ID_USER_SECURITY_LEVEL_QUERY_SERVICE );
    def.setType( ServiceType_UserQuery );
    def.setDescription( "UserSecurityLevelQuery" );  
    SERVICES->addLocalService( def );    
    
    return (SERVICES->getLocalServices().size() == 2);   
}


bool UserValidator::addPredefinedCapabilities(){
    CapabilityDefinition def;
    def.setId( ID_REVOKED_USERS_CAPABILITY );
    def.setType( Capability_Users );
    def.setAccess( AccessType_Read );
    def.setAllowMultipleReadSubscriptions(true);
    def.setAllowMultipleWriteSubscriptions(false);
    def.setDescription( "UserRevoked_Update" );
    def.setMinReadSecuritylevel(0);
    def.setMinWriteSecuritylevel(0);
    CAPABILITIES->addLocalCapability( def );
    
    return (CAPABILITIES->getLocalCapabilities().size() == 1);   
}

void UserValidator::onSignalTrapped(int signal_number){
    exitRequired_ = true;
}

void UserValidator::run() {
    
    while (!exitRequired_) {
        sleep(2);
    }    
}
