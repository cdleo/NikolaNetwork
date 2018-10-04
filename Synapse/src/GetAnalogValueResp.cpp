
#include <synapse/GetAnalogValueResp.h>

GetAnalogValueResp::GetAnalogValueResp(CapabilityDefinition capability, std::string userHash, GetDataResult result, long value) 
: GetValueResp(SubCommandID_AnalogValue, capability, userHash, result), value_(value) {
    
}

std::string GetAnalogValueResp::encode() {
    std::ostringstream stream;
    stream << value_ << FIELD_SEPARATOR; 

    return GetValueResp::encode( stream.str() );
}

bool GetAnalogValueResp::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!GetValueResp::decode(buffer, len, pos))
        return false;

    if (!getField( aux, pos, value_ ) )
        return false;
    
    LOG->LogMessage(LOG_DEBUG,"GetAnalogValueResp::decode(): Capability type=%d id=%d value=%d", (int)getCapability().getType(), getCapability().getId(), value_);
    return true;
}
