
#include <synapse/SetAnalogValue.h>

SetAnalogValue::SetAnalogValue(CapabilityDefinition capability, std::string userHash, long value) 
: SetValue(SubCommandID_AnalogValue, capability, userHash), value_(value) {
    
}

SetAnalogValue::SetAnalogValue(SetAnalogValue* other)
: SetValue(SubCommandID_AnalogValue, other->getCapability(), other->getUserHash()), value_(other->getValue()) {
}

std::string SetAnalogValue::encode() {
    std::ostringstream stream;
    stream << value_ << FIELD_SEPARATOR; 

    return SetValue::encode( stream.str() );
}

bool SetAnalogValue::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!SetValue::decode(buffer, len, pos))
        return false;
    
    if (!getField( aux, pos, value_ ) )
        return false;        
    
    LOG->LogMessage(LOG_DEBUG,"SetAnalogValue::decode(): Capability type=%d id=%d value=%d", (int)getCapability().getType(), getCapability().getId(), value_);
    return true;
}



