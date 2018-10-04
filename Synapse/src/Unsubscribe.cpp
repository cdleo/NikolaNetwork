
#include <synapse/Unsubscribe.h>

Unsubscribe::Unsubscribe(CapabilityDefinition capability, std::string userHash) 
: Request(CommandID_Unsubscribe), capability_(capability), userHash_(userHash) {
    
}

Unsubscribe::Unsubscribe(Unsubscribe* other)
: Request(CommandID_Unsubscribe), capability_(other->getCapability()), userHash_(other->getUserHash()) {
}

std::string Unsubscribe::encode() {
    std::ostringstream stream;
    stream << capability_.encode() << FIELD_SEPARATOR << userHash_.c_str() << FIELD_SEPARATOR; 

    return Request::encode( stream.str() );
}

bool Unsubscribe::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!Request::decode( buffer, len, pos ) )
        return false; 
    
    std::string streamCap;
    if (!getField( aux, pos, streamCap ) )
        return false;        
    
    if (!capability_.decode(streamCap) )
        return false;        
    
    if (!getField( aux, pos, userHash_ ) )      //Si el hash está vacío, solo es válido si el nivel de seguridad es 0 y es de lectura
        if ((capability_.getAccess() != AccessType_Read) && (capability_.getMinReadSecuritylevel() != 0))
            return false;    
    
    LOG->LogMessage(LOG_DEBUG,"Unsubscribe::decode(): Capability type=%d id=%d", (int)capability_.getType(), capability_.getId());
    return true;
}

