/* 
 * File:   NVARS.h
 * Author: cleo
 *
 * Created on July 29, 2014, 3:25 PM
 */

#ifndef NVARS_H
#define	NVARS_H

#include <GlobalDefinitions.h>

#include <SignalsManager.h>
#include <Parameters.h>
#include <Log.h>

class NVARS : SignalsHandler {
public:
    NVARS(std::string routeToConfigFile = "");
    virtual ~NVARS();
    
    void run();

    void onSignalTrapped(int signal_number);
    
private:
    void initializeSignalsManager();

    void loadParameters(std::string routeToConfigFile);

    
    bool        exitRequired_;
};

#endif	/* NVARS_H */

