/* 
 * File:   ServicesResp.h
 * Author: cleo
 *
 * Created on July 14, 2014, 4:27 PM
 */

#ifndef SERVICESRESP_H
#define	SERVICESRESP_H

#include <synapse/items/ServiceDefinition.h>
#include <synapse/Response.h>
#include <vector>
#include <algorithm>

typedef std::vector<ServiceDefinition>       ServicesList;

class ServicesResp : public Response {	
public:    
    ServicesResp();
    ServicesResp(Header& header) : Response(header) {};
    virtual ~ServicesResp(){};
    
    void addService(ServiceDefinition& cap) {
        servicesList_.push_back(cap);
    }

    void setServices(ServicesList servList) {
        servicesList_.clear();
        servicesList_.reserve( servList.size() );
        std::copy ( servList.begin(), servList.end(), std::back_inserter(servicesList_) );
    }    
    
    ServicesList getServices() {
        return servicesList_;
    }

    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    ServicesList servicesList_;
    
};

#endif	/* SERVICESRESP_H */

