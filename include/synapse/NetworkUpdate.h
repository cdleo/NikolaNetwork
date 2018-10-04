/* 
 * File:   NetworkUpdate.h
 * Author: cleo
 *
 * Created on July 7, 2014, 9:46 PM
 */

#ifndef NETWORKUPDATE_H
#define	NETWORKUPDATE_H

#include <synapse/items/NetworkItem.h>
#include <synapse/Update.h>
#include <vector>
#include <algorithm>

typedef std::vector<NetworkItem>       NetworkList;

class NetworkUpdate : public Update {	
public:    
    NetworkUpdate(CapabilityDefinition def);
    NetworkUpdate(Header& header) : Update(header) {};
    virtual ~NetworkUpdate(){};
    
    void addItem(NetworkItem& item) {
        networkList_.push_back(item);
    }

    void setNetworkList(NetworkList& netList) {
        networkList_.clear();
        std::copy ( netList.begin(), netList.end(), networkList_.begin() );
    }    
    
    NetworkList getNetworkList() {
        return networkList_;
    }

    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    NetworkList                 networkList_;
};

#endif	/* NETWORKUPDATE_H */

