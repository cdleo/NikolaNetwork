
#include <synapse/GetValue.h>

GetValue::GetValue(SubCommandID subCommandId, CapabilityDefinition capability, std::string userHash) 
: Request(CommandID_GetValue, subCommandId), capability_(capability), userHash_(userHash) {
    
}

std::string GetValue::encode() {
    std::ostringstream stream;
    stream << capability_.encode() << FIELD_SEPARATOR << userHash_.c_str() << FIELD_SEPARATOR; 

    return Request::encode( stream.str() );
}

bool GetValue::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!Request::decode(buffer, len, pos))
        return false;
    
    std::string streamCap;
    if (!getField( aux, pos, streamCap ) )
        return false;        
    
    if (!capability_.decode(streamCap) )
        return false;            
    
    if (!getField( aux, pos, userHash_ ) )
        return false;
    
    LOG->LogMessage(LOG_DEBUG,"GetValue::decode(): Capability type=%d id=%d", (int)capability_.getType(), capability_.getId());
    return true;
}

