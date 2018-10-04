
#include <synapse/ResourcesUpdate.h>

ResourcesUpdate::ResourcesUpdate(CapabilityDefinition def, ResourceUpdateType type) 
: Update(SubCommandID_ResourcesUpdate, def), type_(type) {
    
}

std::string ResourcesUpdate::encode() {
    std::ostringstream stream;
    stream << type_ << FIELD_SEPARATOR;
    
    return Update::encode( stream.str() );
}

bool ResourcesUpdate::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);
    
    unsigned int pos = 0;
    if (!Update::decode( buffer, len, pos ) )
        return false;   
    
    long auxResult;
    if (!getField( aux, pos, auxResult ) )
        return false;
    type_ = static_cast<ResourceUpdateType>(auxResult);
    
    LOG->LogMessage(LOG_DEBUG,"ResourcesUpdate::decode(): %d type updated!", type_);
    return true;
}
