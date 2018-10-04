/* 
 * File:   Terminal.h
 * Author: cleo
 *
 * Created on July 29, 2014, 3:25 PM
 */

#ifndef TERMINAL_H
#define	TERMINAL_H

#include <GlobalDefinitions.h>

#include <SignalsManager.h>
#include <Parameters.h>
#include <Log.h>

class Terminal : SignalsHandler {
public:
    Terminal(std::string routeToConfigFile = "");
    virtual ~Terminal();
    
    void run();

    void onSignalTrapped(int signal_number);
    
private:
    void initializeSignalsManager();

    void loadParameters(std::string routeToConfigFile);

    
    bool        exitRequired_;
};

#endif	/* TERMINAL_H */

