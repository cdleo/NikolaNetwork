
#include <synapse/NetworkUpdate.h>

NetworkUpdate::NetworkUpdate(CapabilityDefinition def) 
: Update(SubCommandID_GetNetwork, def) {
    
}

std::string NetworkUpdate::encode() {
    std::ostringstream stream;
    stream << networkList_.size() << FIELD_SEPARATOR;
    for (unsigned int i = 0; i < networkList_.size(); i++) {
        stream << networkList_.at(i).encode() << FIELD_SEPARATOR;
    }
    
    return Update::encode( stream.str() );
}

bool NetworkUpdate::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);
    
    unsigned int pos = 0;
    if (!Update::decode( buffer, len, pos ) )
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
    
    LOG->LogMessage(LOG_DEBUG,"NetworkUpdate::decode(): %u items returned!",networkList_.size());
    return true;
}
