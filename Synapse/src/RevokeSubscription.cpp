
#include <synapse/RevokeSubscription.h>

RevokeSubscription::RevokeSubscription(CapabilityDefinition capability, std::string userHash, std::string peerIDRevoked) 
: Request(CommandID_Revoke), capability_(capability), userHash_(userHash), peerIDRevoked_(peerIDRevoked) {
    
}

RevokeSubscription::RevokeSubscription(RevokeSubscription* other)
: Request(CommandID_Revoke), capability_(other->getCapability()), userHash_(other->getUserHash()),
  peerIDRevoked_(other->getPeerIDRevoked()) {
}

std::string RevokeSubscription::encode() {
    std::ostringstream stream;
    stream << capability_.encode() << FIELD_SEPARATOR << userHash_.c_str() 
           << FIELD_SEPARATOR << peerIDRevoked_.c_str() << FIELD_SEPARATOR; 
    
    return Request::encode ( stream.str() );
}

bool RevokeSubscription::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!Request::decode( buffer, len, pos ) )
        return false; 
    
    std::string streamCap;
    if (!getField( aux, pos, streamCap ) )
        return false;        
    
    if (!capability_.decode(streamCap) )
        return false;             
    
    if (!getField( aux, pos, userHash_ ) )
        return false;

    if (!getField( aux, pos, peerIDRevoked_ ) )
        return false;    

    LOG->LogMessage(LOG_DEBUG,"RevokeSubscription::decode(): Capability type=%d id=%d", (int)capability_.getType(), capability_.getId());
    return true;
}


