
#include <synapse/UserValidationResp.h>

UserValidationResp::UserValidationResp(ServiceDefinition service, ValidationResult result, std::string user, std::string pwd, std::string hash) 
: GetServiceDataResp(SubCommandID_GetUserHash, service, GetDataResult_Undefined), 
  result_(result), user_(user), pwd_(pwd), userHash_(hash) {
    
}

std::string UserValidationResp::encode() {
    std::ostringstream stream;
    stream << result_ << FIELD_SEPARATOR << user_ << FIELD_SEPARATOR << pwd_ << FIELD_SEPARATOR << userHash_ << FIELD_SEPARATOR; 
    
    return GetServiceDataResp::encode( stream.str() );
}

bool UserValidationResp::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);
    
    unsigned int pos = 0;
    if (!GetServiceDataResp::decode(buffer, len, pos))
        return false;
    
    long tempValue;
    if (!getField( aux, pos, tempValue ) )
        return false;    
    result_ = static_cast<ValidationResult>(tempValue);
    
    if (!getField( aux, pos, user_ ) )
        return false;

    if (!getField( aux, pos, pwd_ ) )
        return false;    
    
    if (!getField( aux, pos, userHash_ ) )
        return false;

    LOG->LogMessage(LOG_DEBUG,"UserValidationResp::decode(): User=%s result=%d", user_.c_str(), (int) result_);
    return true;
}

