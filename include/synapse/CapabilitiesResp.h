/* 
 * File:   CapabilitiesResp.h
 * Author: cleo
 *
 * Created on July 14, 2014, 4:27 PM
 */

#ifndef CAPABILITIESRESP_H
#define	CAPABILITIESRESP_H

#include <synapse/items/CapabilityDefinition.h>
#include <synapse/Response.h>
#include <vector>
#include <algorithm>

typedef std::vector<CapabilityDefinition>       CapabilitiesList;

class CapabilitiesResp : public Response {	
public:    
    CapabilitiesResp();
    CapabilitiesResp(Header& header);
    virtual ~CapabilitiesResp();
    
    CapabilitiesList getCapabilities() {
        return capabilitiesList_;
    }    
    
    void addCapability(CapabilityDefinition cap);

    void setCapabilities(CapabilitiesList capList);

    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    CapabilitiesList capabilitiesList_;
    
};

#endif	/* CAPABILITIESRESP_H */

