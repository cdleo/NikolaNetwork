
#include <synapse/SetValue.h>

SetValue::SetValue(SubCommandID subCommandId, CapabilityDefinition capability, std::string userHash) 
: Request(CommandID_SetValue, subCommandId), capability_(capability), userHash_(userHash) {
    
}

std::string SetValue::encode() {
    std::ostringstream stream;
    stream << capability_.encode() << FIELD_SEPARATOR << userHash_.c_str() << FIELD_SEPARATOR; 

    return Request::encode( stream.str() );
}

std::string SetValue::encode( std::string currentData ) {
    std::ostringstream stream;
    stream << capability_.encode() << FIELD_SEPARATOR << userHash_.c_str() << FIELD_SEPARATOR << currentData; 
        
    return Request::encode( stream.str() );
}

bool SetValue::decode(const void* buffer, size_t len) {

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
    
    LOG->LogMessage(LOG_DEBUG,"SetValue::decode(): Capability type=%d id=%d", (int)capability_.getType(), capability_.getId());
    return true;
}

bool SetValue::decode(const void* buffer, size_t len, unsigned int& pos) {

    std::string aux = std::string((const char*) buffer, len);

    if (!Request::decode( buffer, len, pos))
        return false;    
    
    std::string streamCap;
    if (!getField( aux, pos, streamCap ) )
        return false;        
    
    if (!capability_.decode(streamCap) )
        return false;               
    
    if (!getField( aux, pos, userHash_ ) )
        return false;

    LOG->LogMessage(LOG_DEBUG,"SetValue::decode(): Capability type=%d id=%d", (int)capability_.getType(), capability_.getId());
    return true;
}


