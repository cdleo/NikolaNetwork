
#include <synapse/GetDigitalValueResp.h>

GetDigitalValueResp::GetDigitalValueResp(CapabilityDefinition capability, std::string userHash, GetDataResult result, bool value) 
: GetValueResp(SubCommandID_DigitalValue, capability, userHash, result), value_(value) {
    
}

std::string GetDigitalValueResp::encode() {
    std::ostringstream stream;
    stream << value_ << FIELD_SEPARATOR; 

    return GetValueResp::encode( stream.str() );
}

bool GetDigitalValueResp::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!GetValueResp::decode(buffer, len, pos))
        return false;

    if (!getField( aux, pos, value_ ) )
        return false;
    
    LOG->LogMessage(LOG_DEBUG,"GetDigitalValueResp::decode(): Capability type=%d id=%d value=%s", (int)getCapability().getType(), getCapability().getId(), (value_) ? "true" : "false");
    return true;
}
