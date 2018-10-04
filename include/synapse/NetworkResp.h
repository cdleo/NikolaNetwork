/* 
 * File:   NetworkResp.h
 * Author: cleo
 *
 * Created on July 7, 2014, 9:46 PM
 */

#ifndef NETWORKRESP_H
#define	NETWORKRESP_H

#include <synapse/items/NetworkItem.h>
#include <synapse/GetServiceDataResp.h>
#include <vector>
#include <algorithm>

typedef std::vector<NetworkItem>       NetworkList;

class NetworkResp : public GetServiceDataResp {	
public:    
    NetworkResp(ServiceDefinition service, GetDataResult result);
    NetworkResp(Header& header) : GetServiceDataResp(header) {};
    virtual ~NetworkResp(){};
    
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

#endif	/* NETWORKRESP_H */

