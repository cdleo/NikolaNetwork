
#include <synapse/UserValidation.h>

UserValidation::UserValidation(ServiceDefinition service, std::string user, std::string pwd) 
: GetServiceData(SubCommandID_GetUserHash, service), user_(user), pwd_(pwd) {
    
}

std::string UserValidation::encode() {
    std::ostringstream stream;
    stream << user_.c_str() << FIELD_SEPARATOR << pwd_.c_str() << FIELD_SEPARATOR; 
    
    return GetServiceData::encode( stream.str() );
}

bool UserValidation::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!GetServiceData::decode(aux.c_str(),len, pos))
        return false;
    
    if (!getField( aux, pos, user_ ) )
        return false;

    if (!getField( aux, pos, pwd_ ) )
        return false;
    
    LOG->LogMessage(LOG_DEBUG,"UserValidation::decode(): User=%s", user_.c_str());
    return true;
}

