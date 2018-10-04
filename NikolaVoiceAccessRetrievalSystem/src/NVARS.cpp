/* 
 * File:   NVARS.cpp
 * Author: cleo
 * 
 * Created on July 29, 2014, 3:25 PM
 */

#include <NVARS.h>
#include <ConnectionsHandlers.h>
#include <BusinessLogic.h>

#include <BaseUtilities.h>

NVARS::NVARS(std::string routeToConfigFile) 
 : exitRequired_(false) {
    
    initializeSignalsManager();
    
    loadParameters(routeToConfigFile);
    
    if (!CONN_HANDLERS->initialize())
        LOG->WARNING("[NVARS::error] Unable to initialize connections! Exitting!");
}

NVARS::~NVARS() {

}

void NVARS::initializeSignalsManager(){
    std::vector<int> signals;
    signals.push_back(SIGINT);
    SIGNALS->initialize(this, signals);
}

void NVARS::loadParameters(std::string routeToConfigFile){
    
    if (routeToConfigFile.empty()) {
        std::string selfPath = UTILITIES->get_selfpath(".terminal");
        routeToConfigFile.assign(selfPath).append(".conf");
    }
    
    if (!PARAMETERS->initialize(routeToConfigFile)){
        //Cargamos los valores por default
        PARAMETERS->setValue("moduleDescription","terminal");
        PARAMETERS->setValue("privateKey","/home/cleo/key.pem");
        PARAMETERS->setValue("certificate","/home/cleo/cert.pem");
        PARAMETERS->setValue("pollTimeout", POLL_TIMEOUT_MS_DEFAULT); 
        PARAMETERS->setValue("acceptorBasePort", ACCEPTOR_BASE_PORT_DEFAULT);
        PARAMETERS->setValue("maxAcceptorsPerHost", MAX_ACCEPTORS_PER_HOST_DEFAULT);         
        PARAMETERS->setValue("user", "user");
        PARAMETERS->setValue("password", "password");         
    }
    
}

void NVARS::onSignalTrapped(int signal_number){
    exitRequired_ = true;
}

void NVARS::run() {
    
    while (!exitRequired_) {
        int myChoice = 0;
	
	std::cout << "********** Main Menu **********" << std::endl;
	std::cout << "(1): GetUserHash" << std::endl;
	std::cout << "(2): Subscribe to all capabilities" << std::endl;
        std::cout << "(3): Unsubscribe to all capabilities" << std::endl;
	std::cout << "(4): Set analog value" << std::endl;
        std::cout << "(5): Set digital value" << std::endl;
	std::cout << "(6): Get analog value" << std::endl;
        std::cout << "(7): Get digital value" << std::endl;        
        std::cout << "(9): Exit" << std::endl;
	std::cin >> myChoice;
	
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }        
        
	switch(myChoice)
	{
            case 1:
                    LOGIC->GetUserHash();
                    break;
            case 2:
                    LOGIC->SubscribeAll();
                    break;
            case 3:
                    LOGIC->UnsubscribeAll();
                    break;
            case 4:
                    LOGIC->SetAnalogData();
                    break;                    
            case 5:
                    LOGIC->SetDigitalData();
                    break;  
            case 6:
                    LOGIC->GetAnalogData();
                    break;                    
            case 7:
                    LOGIC->GetDigitalData();
                    break;                       
            case 9:
                    exitRequired_ = true;
                    break;                     
            default:
                break;
	}        
    }    
}
