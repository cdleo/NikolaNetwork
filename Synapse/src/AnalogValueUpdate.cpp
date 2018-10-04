
#include <synapse/AnalogValueUpdate.h>

AnalogValueUpdate::AnalogValueUpdate(CapabilityDefinition def, long value) 
: Update(SubCommandID_AnalogValue, def), value_(value) {
    
}

std::string AnalogValueUpdate::encode() {
    std::ostringstream stream;
    stream << value_ << FIELD_SEPARATOR;
    
    return Update::encode( stream.str() );
}

bool AnalogValueUpdate::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);
    
    unsigned int pos = 0;
    if (!Update::decode( buffer, len, pos ) )
        return false;  
    
    if (!getField( aux, pos, value_ ) )
        return false;

    LOG->LogMessage(LOG_DEBUG,"AnalogValueUpdate::decode(): ID=%d value=%d!",getCapability().getId(), value_);
    return true;
}
