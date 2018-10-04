/* 
 * File:   CapabilityModule.h
 * Author: cleo
 *
 * Created on July 29, 2014, 3:25 PM
 */

#ifndef CAPABILITYMODULE_H
#define	CAPABILITYMODULE_H

#include <GlobalDefinitions.h>

#include <SignalsManager.h>
#include <Parameters.h>
#include <Log.h>

class CapabilityModule : SignalsHandler {
public:
    CapabilityModule(std::string routeToConfigFile = "");
    virtual ~CapabilityModule();
    
    void run();

    void onSignalTrapped(int signal_number);
    
private:
    void initializeSignalsManager();

    void loadParameters(std::string routeToConfigFile);

    
    bool        exitRequired_;
};

#endif	/* CAPABILITYMODULE_H */

