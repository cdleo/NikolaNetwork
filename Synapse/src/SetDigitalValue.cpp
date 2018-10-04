
#include <synapse/SetDigitalValue.h>

SetDigitalValue::SetDigitalValue(CapabilityDefinition capability, std::string userHash, bool value) 
: SetValue(SubCommandID_DigitalValue, capability, userHash), value_(value) {
    
}

SetDigitalValue::SetDigitalValue(SetDigitalValue* other) 
: SetValue(SubCommandID_DigitalValue, other->getCapability(), other->getUserHash()), value_(other->getValue()) {
    
}

std::string SetDigitalValue::encode() {
    std::ostringstream stream;
    stream << value_ << FIELD_SEPARATOR; 

    return SetValue::encode( stream.str() );
}

bool SetDigitalValue::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!SetValue::decode(buffer, len, pos))
        return false;
    
    if (!getField( aux, pos, value_ ) )
        return false;        
    
    LOG->LogMessage(LOG_DEBUG,"SetDigitalValue::decode(): Capability type=%d id=%d value=%s", (int)getCapability().getType(), getCapability().getId(), (value_) ? "true" : "false");
    return true;
}




