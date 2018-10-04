
#include <synapse/NetworkResp.h>

NetworkResp::NetworkResp(ServiceDefinition service, GetDataResult result) 
: GetServiceDataResp(SubCommandID_GetNetwork, service, result) {
    
}

std::string NetworkResp::encode() {
    std::ostringstream stream;

    stream << networkList_.size() << FIELD_SEPARATOR;
    for (unsigned int i = 0; i < networkList_.size(); i++) {
        stream << networkList_.at(i).encode() << FIELD_SEPARATOR;
    }
    
    return GetServiceDataResp::encode( stream.str() );
}

bool NetworkResp::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);
    
    unsigned int pos = 0;
    if (!GetServiceDataResp::decode(aux.c_str(),len, pos))
        return false;
    
    unsigned int cantHosts = 0;
    if (!getField( aux, pos, cantHosts ) )
        return false;

    for (unsigned int i = 0; i < cantHosts; i++) {
        NetworkItem item;
        if (!getField( aux, pos, &item ) )
            return false;
        networkList_.push_back( item );
    }     
    
    LOG->LogMessage(LOG_DEBUG,"NetworkResp::decode(): %u items returned!",networkList_.size());
    return true;
}
