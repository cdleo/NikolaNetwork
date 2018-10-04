/* 
 * File:   NetworkBrowser.cpp
 * Author: cleo
 * 
 * Created on March 25, 2014, 2:18 PM
 */

#include <NetworkBrowser.h>

#include <IPRangeCalculator.h>
#include <SubnetCalculator.h>
#include <DataStorage.h>

#include <sys/socket.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <arpa/inet.h>
#include <netinet/in.h> 
#include <net/if.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <cstdlib>
#include <stdlib.h>

#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>

#include <iostream>
#include <stdexcept>

NetworkBrowser::NetworkBrowser()
: exitRequired_(false) {

}

NetworkBrowser::~NetworkBrowser() {
    
}

void NetworkBrowser::discover() {

    #ifndef _DAEMON_
        timeval discoveryBeginTime;
        timeval discoveryEndTime;

        std::cout << "Discovering... "  << std::endl;
        
        gettimeofday(&discoveryBeginTime, NULL);
    #endif

    MAP_IF_NEIGHBOR newNetworkMap;
    
    getInterfaces(newNetworkMap);

    findNeighbors(newNetworkMap);

    #ifndef _DAEMON_
        gettimeofday(&discoveryEndTime, NULL);

        float elapsed_secs = (discoveryEndTime.tv_sec - discoveryBeginTime.tv_sec) +
                             ((float) (discoveryEndTime.tv_usec - discoveryBeginTime.tv_usec) / 1000000.0);

        std::cout << "Discovery elapsed time: " << std::fixed << std::setprecision(3) << elapsed_secs << " segundos." << std::endl;
    #endif     
}

void NetworkBrowser::getInterfaces(MAP_IF_NEIGHBOR& newNetworkMap) {
    
    try {
        struct ifaddrs* ifAddrStruct=NULL;
        struct ifaddrs* ifa=NULL;
        void * tmpAddrPtr=NULL;

        #ifndef _DAEMON_
        std::cout << "Getting Interfaces... "  << std::endl;
        #endif        
        getifaddrs(&ifAddrStruct);

        for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
            if ( (ifa->ifa_addr != NULL) && (ifa->ifa_addr->sa_family==AF_INET) ) { // Check is a IP4 Address
                
                if (!isInterfaceEnabled(ifa->ifa_name))
                    continue;
                
                char addressBuffer[INET_ADDRSTRLEN];
                char netBuffer[INET_ADDRSTRLEN];
                char broadBuffer[INET_ADDRSTRLEN];

                tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
                inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);

                tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_netmask)->sin_addr;
                inet_ntop(AF_INET, tmpAddrPtr, netBuffer, INET_ADDRSTRLEN);

                if ((ifa->ifa_flags & IFF_BROADCAST) == IFF_BROADCAST) {
                    tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_ifu.ifu_broadaddr)->sin_addr;
                } else {
                    tmpAddrPtr=&((struct sockaddr_in *)ifa->ifa_ifu.ifu_dstaddr)->sin_addr;
                }
                inet_ntop(AF_INET, tmpAddrPtr, broadBuffer, INET_ADDRSTRLEN); 

                IF_NEIGHBOR IFNeighbor;
                IFNeighbor.interface.name.assign( ifa->ifa_name );
                IFNeighbor.interface.IPAddress.assign( addressBuffer );
                IFNeighbor.interface.NetMask.assign( netBuffer );
                IFNeighbor.interface.BroadcastAddress.assign( broadBuffer );

                SubnetCalculator calculator(IFNeighbor.interface.IPAddress, IFNeighbor.interface.NetMask);
                IFNeighbor.interface.firstUsableHost = calculator.getFirstUsableHost();
                IFNeighbor.interface.lastUsableHost = calculator.getLastUsableHost();
                IFNeighbor.interface.networkSize = calculator.getSubnetSize();

                newNetworkMap[ IFNeighbor.interface.name ] = IFNeighbor;
            }
        }

        if (ifAddrStruct!=NULL) { 
            freeifaddrs(ifAddrStruct);
        } else {
            #ifndef _DAEMON_
                std::cout << "Unable to get interfaces! Errno: " << errno << std::endl;
            #else
                LOG->LogMessage(LOG_WARNING, "[NetworkBrowser::checkAddInterfaces] Unable to get interfaces! Errno: %d", errno);
            #endif           
        }
        
    } catch (std::runtime_error ex) {
        LOG->LogMessage(LOG_WARNING, "[NetworkBrowser::checkAddInterfaces] Runtime error: %s", ex.what());
    }
}

bool NetworkBrowser::isInterfaceEnabled(const char* ifName) {
    std::vector<std::string> enabledInterfaces;
    PARAMETERS->getValue("interfaces", enabledInterfaces);
    
    if (!enabledInterfaces.size()){     //Si no hay interfaces configuradas, están todas habilitadas excepto el loopback
        return (strcmp( ifName, IF_LOOPBACK_NAME ) != 0);
    }
        
    
    for (std::vector<std::string>::iterator it = enabledInterfaces.begin(); it != enabledInterfaces.end(); it++){
        if (it->compare(ifName) == 0)
            return true;
    }
    return false;
}

void NetworkBrowser::findNeighbors(MAP_IF_NEIGHBOR& newNetworkMap) {

    std::string currentIP;    
    #ifndef _DAEMON_
        std::cout << "Buscando \"vecinos\"..." << std::endl;
    #endif     

    try {
        
        MAP_NEIGHBOR networkMap = CUSTOM_STORAGE->getNetworkMap();
        
        for(MAP_IF_NEIGHBOR::iterator it = newNetworkMap.begin(); it != newNetworkMap.end(); it++) { 
            
            IPRangeCalculator ipRange(it->second.interface.firstUsableHost, it->second.interface.lastUsableHost);
            
            Utils::toString(it->second.interface.firstUsableHost, currentIP);   
            
            PingerThread* vThreads[THREADS_COUNT];
            int iThread = 0;            
            
            #ifndef _DAEMON_
            long long current = 0;
            std::string progressBarLegend = std::string("IF ").append(it->second.interface.name).append(": ");
            #endif 
            
            while (!currentIP.empty() && (it->second.interface.networkSize > 0) && (!exitRequired_)) {
                
                #ifndef _DAEMON_
                Utils::drawProgressBar(progressBarLegend, current, it->second.interface.networkSize);
                current++;
                #endif 

                if (it->second.interface.IPAddress.compare(currentIP) != 0) {           //Si no soy yo mismo
                    vThreads[iThread] = new PingerThread(currentIP);
                    vThreads[iThread]->start();
                    iThread++;
                } else {                        //Esto es solo como prueba para cuando no lo lanzo como root
                    fillMapInformation( currentIP, true, true, networkMap);
                }
                   
                if (iThread >= THREADS_COUNT) {     
                    resolvePingThreads(vThreads, iThread, networkMap);
                    iThread = 0;
                }
                ipRange.nextValue( currentIP );
            }
            
            resolvePingThreads(vThreads, iThread, networkMap);
        }
        
        CUSTOM_STORAGE->setNetworkMap( networkMap );
        
    } catch (std::runtime_error ex) {
        LOG->LogMessage(LOG_WARNING, "[NetworkBrowser::findNeighborsByExtension] Runtime error: %s", ex.what());
    }    
}

void NetworkBrowser::resolvePingThreads(PingerThread* vThreads[], int threadCount, MAP_NEIGHBOR& neighbors) {
    
    for(int i = 0; i < threadCount; i++) {
        PingerThread* pt = vThreads[i];
        pt->join();
        
        fillMapInformation( pt->getRemoteIP(), pt->isAlive(), false, neighbors );
        
        delete pt;
    }            
    return;
}

void NetworkBrowser::fillMapInformation(std::string IP, bool isAlive, bool isLocalAddress, MAP_NEIGHBOR& networkMap) {
    
    NEIGHBOR neighbor;
    MAP_NEIGHBOR::iterator it =  networkMap.find(IP);
    if (it != networkMap.end()) {
        neighbor = it->second;
        switch (neighbor.lastStatus) {
            case HostStatus_BecomeOnline:
            case HostStatus_Online:
            {
                neighbor.lastStatus = (isAlive) ? HostStatus_Online : HostStatus_NotResponding;
                break;
            }
                
            case HostStatus_Offline:
            case HostStatus_NotResponding:
            {
                neighbor.lastStatus = (isAlive) ? HostStatus_BecomeOnline : HostStatus_Offline;
                break;
            }
            
            case HostStatus_Undefined:
            default:
            {
                networkMap.erase(it);
                return;
            }            
        }
        neighbor.lastUpdate.assign( Utils::getNowAsString() );   
        it->second = neighbor;

    } else {
        neighbor.IPAddress.assign( IP );
        neighbor.lastUpdate.assign( Utils::getNowAsString() );        
        neighbor.lastStatus = (isAlive) ? HostStatus_BecomeOnline : HostStatus_Offline;
        neighbor.isLocalAddress = isLocalAddress;
        
        networkMap.insert( std::pair<std::string,NEIGHBOR>(IP, neighbor) );
    }

}
