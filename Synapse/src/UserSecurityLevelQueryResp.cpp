#include <synapse/UserSecurityLevelQueryResp.h>

UserSecurityLevelQueryResp::UserSecurityLevelQueryResp(ServiceDefinition service, ValidationResult result, std::string hash, int securityLevel) 
: GetServiceDataResp(SubCommandID_GetUserSecurityLevel, service, GetDataResult_Undefined), result_(result), userHash_(hash), securityLevel_(securityLevel) {
    
}

std::string UserSecurityLevelQueryResp::encode() {
    std::ostringstream stream;
    stream << result_ << FIELD_SEPARATOR << userHash_ << FIELD_SEPARATOR << securityLevel_ << FIELD_SEPARATOR; 
    
    return GetServiceDataResp::encode( stream.str() );
}

bool UserSecurityLevelQueryResp::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);
    
    unsigned int pos = 0;
    if (!GetServiceDataResp::decode(buffer, len, pos))
        return false;    
    
    long tempValue;
    if (!getField( aux, pos, tempValue ) )
        return false;    
    result_ = static_cast<ValidationResult>(tempValue);
    
    if (!getField( aux, pos, userHash_ ) )
        return false;

    if (!getField( aux, pos, tempValue ) )
        return false;    
    securityLevel_ = tempValue;
    
    LOG->LogMessage(LOG_DEBUG,"UserSecurityLevelQueryResp::decode(): Hash=%s result=%d", userHash_.c_str(), (int) result_);
    return true;
}
