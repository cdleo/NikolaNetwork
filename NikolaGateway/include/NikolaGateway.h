/* 
 * File:   NikolaGateway.h
 * Author: cleo
 *
 * Created on July 29, 2014, 3:25 PM
 */

#ifndef NIKOLAGATEWAY_H
#define	NIKOLAGATEWAY_H

#include <GlobalDefinitions.h>

#include <SignalsManager.h>
#include <Parameters.h>
#include <Log.h>

class NikolaGateway : SignalsHandler {
public:
    NikolaGateway(std::string routeToConfigFile = "");
    virtual ~NikolaGateway();
    
    void run();

    void onSignalTrapped(int signal_number);
    
private:
    void initializeSignalsManager();

    void loadParameters(std::string routeToConfigFile);
    
    bool addPredefinedCapabilities();
    
    bool        exitRequired_;
};

#endif	/* NIKOLAGATEWAY_H */

