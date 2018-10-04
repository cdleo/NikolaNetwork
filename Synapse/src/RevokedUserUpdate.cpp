
#include <synapse/RevokedUserUpdate.h>

RevokedUserUpdate::RevokedUserUpdate(CapabilityDefinition def, std::string revokedUserHash) 
: Update(SubCommandID_GetUserRevoked, def), revokedUserHash_(revokedUserHash) {
    
}

std::string RevokedUserUpdate::encode() {
    std::ostringstream stream;
    stream << revokedUserHash_.c_str() << FIELD_SEPARATOR;
    
    return Update::encode( stream.str() );
}

bool RevokedUserUpdate::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);
    
    unsigned int pos = 0;
    if (!Update::decode( buffer, len, pos ) )
        return false;   

    if (!getField( aux, pos, revokedUserHash_ ) )
        return false; 
    
    LOG->LogMessage(LOG_DEBUG,"RevokedUserUpdate::decode(): User revoked: %s!",revokedUserHash_.c_str());
    return true;
}
