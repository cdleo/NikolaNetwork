
#include <synapse/UserSecurityLevelQuery.h>

UserSecurityLevelQuery::UserSecurityLevelQuery(ServiceDefinition service, std::string userHash) 
: GetServiceData(SubCommandID_GetUserSecurityLevel, service), userHash_(userHash) {
    
}

std::string UserSecurityLevelQuery::encode() {
    std::ostringstream stream;
    stream << userHash_.c_str() << FIELD_SEPARATOR; 
    
    return GetServiceData::encode( stream.str() );
}

bool UserSecurityLevelQuery::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!GetServiceData::decode(aux.c_str(),len, pos))
        return false;
    
    if (!getField( aux, pos, userHash_ ) )
        return false;
    
    LOG->LogMessage(LOG_DEBUG,"UserSecurityLevelQuery::decode(): UserHash=%s", userHash_.c_str());
    return true;
}

