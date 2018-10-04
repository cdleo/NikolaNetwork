/* 
 * File:   GatewayTerminal.h
 * Author: cleo
 *
 * Created on July 29, 2014, 3:25 PM
 */

#ifndef GATEWAYTERMINAL_H
#define	GATEWAYTERMINAL_H

#include <GlobalDefinitions.h>

#include <SignalsManager.h>
#include <Parameters.h>
#include <Log.h>

class GatewayTerminal : SignalsHandler {
public:
    GatewayTerminal(std::string routeToConfigFile = "");
    virtual ~GatewayTerminal();
    
    void run();

    void onSignalTrapped(int signal_number);
    
private:
    void initializeSignalsManager();

    void loadParameters(std::string routeToConfigFile);

    
    bool        exitRequired_;
};

#endif	/* GATEWAYTERMINAL_H */

