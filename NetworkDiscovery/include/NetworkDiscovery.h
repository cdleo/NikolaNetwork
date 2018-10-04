/* 
 * File:   NetworkDiscovery.h
 * Author: cleo
 *
 * Created on March 25, 2014, 2:18 PM
 */

#ifndef NETWORKDISCOVERY_H
#define	NETWORKDISCOVERY_H

#include <GlobalDefinitions.h>
#include <ConnectionsHandlers.h>
#include <NetworkBrowser.h>

#include <SignalsManager.h>
#include <Parameters.h>

using namespace std;

class NetworkDiscovery : SignalsHandler {
public:
    NetworkDiscovery(std::string routeToConfigFile = "");
    virtual ~NetworkDiscovery();
    
    void run();
    
    void onSignalTrapped(int signal_number);
    
    void tryConnections();
    
private:
    void initializeSignalsManager();
    
    void loadParameters(std::string routeToConfigFile);
    
    bool addPredefinedServices();
    
    bool addPredefinedCapabilities();
    
    void getInterfaces(MAP_IF_NEIGHBOR& newNetworkMap);
    void findNeighbors(MAP_IF_NEIGHBOR& newNetworkMap);
    
    void resolvePingThreads(PingerThread* vThreads[], int threadCount, MAP_NEIGHBOR& neighbors);
    
    void fillMapInformation(std::string IP, bool isAlive, bool isLocalAddress, MAP_NEIGHBOR& networkMap);
    
    //void InformKnownNetwork();
    void InformNetworkChanges();    
    void ShowNetworkChanges();

    NetworkBrowser*     browser_;
    
    pthread_t           connThread_;
   
    bool                exitRequired_;
};

#endif	/* NETWORKDISCOVERY_H */

