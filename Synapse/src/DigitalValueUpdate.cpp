
#include <synapse/DigitalValueUpdate.h>

DigitalValueUpdate::DigitalValueUpdate(CapabilityDefinition def, bool value) 
: Update(SubCommandID_DigitalValue,def), value_(value) {
    
}

std::string DigitalValueUpdate::encode() {
    std::ostringstream stream;
    stream << value_ << FIELD_SEPARATOR;
    
    return Update::encode( stream.str() );
}

bool DigitalValueUpdate::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);
    
    unsigned int pos = 0;
    if (!Update::decode( buffer, len, pos ) )
        return false;  
    
    if (!getField( aux, pos, value_ ) )
        return false;

    LOG->LogMessage(LOG_DEBUG,"DigitalValueUpdate::decode(): ID=%d value=%s!",getCapability().getId(), (value_) ? "true" : "false");
    return true;
}
