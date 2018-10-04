
#include <synapse/CapabilitiesResp.h>
#include <BaseUtilities.h>

CapabilitiesResp::CapabilitiesResp() 
: Response(CommandID_Capabilities) {
    capabilitiesList_.clear(); 
}

CapabilitiesResp::CapabilitiesResp(Header& header)
: Response(header) { 
    capabilitiesList_.clear(); 
}

CapabilitiesResp::~CapabilitiesResp() {
    capabilitiesList_.clear(); 
}

void CapabilitiesResp::addCapability(CapabilityDefinition cap) {
    capabilitiesList_.push_back(cap);
}

void CapabilitiesResp::setCapabilities(CapabilitiesList capList) {
    capabilitiesList_.clear();
    capabilitiesList_.reserve( capList.size() );
    std::copy ( capList.begin(), capList.end(), std::back_inserter(capabilitiesList_) );
}    

std::string CapabilitiesResp::encode() {
    std::ostringstream stream;
    stream << capabilitiesList_.size() << FIELD_SEPARATOR;
    for (CapabilitiesList::iterator it = capabilitiesList_.begin(); it != capabilitiesList_.end(); it++) {
        stream << it->encode() << FIELD_SEPARATOR;         
    }
    return Response::encode( stream.str() );
}

bool CapabilitiesResp::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);
    
    unsigned int pos = 0;
    if (!Response::decode(buffer, len, pos))
        return false;
    
    unsigned int cantCap = 0;
    if (!getField( aux, pos, cantCap ) )
        return false;

    for (unsigned int i = 0; i < cantCap; i++) {
        CapabilityDefinition cap;
        std::string streamCap;
        if (!getField( aux, pos, streamCap ) )
            return false;
        
        if (!cap.decode( streamCap ))
            return false;
        
        capabilitiesList_.push_back( cap );
    }     
    
    LOG->LogMessage(LOG_DEBUG,"CapabilitiesResp::decode(): %u capabilities returned!",capabilitiesList_.size());
    return true;
}


