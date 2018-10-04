/* 
 * File:   UserValidator.h
 * Author: cleo
 *
 * Created on July 29, 2014, 3:25 PM
 */

#ifndef USERVALIDATOR_H
#define	USERVALIDATOR_H

#include <GlobalDefinitions.h>
#include <ConnectionsHandlers.h>

#include <SignalsManager.h>
#include <Parameters.h>
#include <Log.h>

class UserValidator : SignalsHandler {
public:
    UserValidator(std::string routeToConfigFile = "");
    virtual ~UserValidator();
    
    void run();

    void onSignalTrapped(int signal_number);
    
private:
    void initializeSignalsManager();

    void loadParameters(std::string routeToConfigFile);

    bool addPredefinedServices();
    
    bool addPredefinedCapabilities();
    
    bool        exitRequired_;
};

#endif	/* USERVALIDATOR_H */

