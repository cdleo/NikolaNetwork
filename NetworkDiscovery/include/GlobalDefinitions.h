
#ifndef GLOBALDEFINITIONS_H
#define	GLOBALDEFINITIONS_H

#include <synapse/PredefinedServicesAndCaps.h>

#include <map>
#include <vector>
#include <string>

//#define _DAEMON_

#define IF_LOOPBACK_NAME "lo"

#define THREADS_COUNT 5

typedef struct IFDetails {
    
    IFDetails() { name.clear(); IPAddress.clear(); NetMask.clear(); BroadcastAddress.clear(); networkSize=0; firstUsableHost.clear(); lastUsableHost.clear(); }
    
    std::string name;
    std::string IPAddress;
    std::string NetMask;
    std::string BroadcastAddress;
    
    long long   networkSize;
    
    std::vector<int> firstUsableHost;
    std::vector<int> lastUsableHost;
    
} IFDetails;

typedef enum HostStatus {
    HostStatus_Undefined = 0,
    HostStatus_BecomeOnline = 1,
    HostStatus_Online = 2,
    HostStatus_Offline = 3,
    HostStatus_NotResponding = 4        //(Become Offline??)
} HostStatus;

typedef struct NEIGHBOR {
    
    NEIGHBOR() { IPAddress.clear(); lastUpdate.clear(); lastStatus = HostStatus_Undefined; isLocalAddress=false; }
    
    std::string IPAddress;
    std::string lastUpdate;
    HostStatus  lastStatus;
    bool        isLocalAddress;
} NEIGHBOR;
typedef std::map<std::string, NEIGHBOR>         MAP_NEIGHBOR;

typedef struct IF_NEIGHBOR {
    
    IF_NEIGHBOR() { neighbors.clear(); }
    
    IFDetails        interface;
    MAP_NEIGHBOR     neighbors;
} IF_NEIGHBOR;
typedef std::map<std::string,IF_NEIGHBOR>       MAP_IF_NEIGHBOR;


#endif /* GLOBALDEFINITIONS_H */
