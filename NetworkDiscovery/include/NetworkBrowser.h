/* 
 * File:   NetworkBrowser.h
 * Author: cleo
 *
 * Created on August 30, 2014, 10:37 PM
 */

#ifndef NETWORKBROWSER_H
#define	NETWORKBROWSER_H

#include <GlobalDefinitions.h>
#include <PingerThread.h>
#include <Parameters.h>

using namespace std;

class NetworkBrowser {
public:
    NetworkBrowser();
    virtual ~NetworkBrowser();
    
    void discover();
    
    void setExitFlag(bool value) {
        exitRequired_ = value;
    }
    
private:    
    void getInterfaces(MAP_IF_NEIGHBOR& newNetworkMap);
    
    bool isInterfaceEnabled(const char* ifName);
    
    void findNeighbors(MAP_IF_NEIGHBOR& newNetworkMap);
    
    void resolvePingThreads(PingerThread* vThreads[], int threadCount, MAP_NEIGHBOR& neighbors);
    
    void fillMapInformation(std::string IP, bool isAlive, bool isLocalAddress, MAP_NEIGHBOR& networkMap);

    bool        exitRequired_;   
};

#endif	/* NETWORKBROWSER_H */

