
#include <synapse/RevokeSubscriptionResp.h>

RevokeSubscriptionResp::RevokeSubscriptionResp(CapabilityDefinition capability, RevokeResult result) 
: Response(CommandID_Revoke), capability_(capability), result_(result) {
    
}

RevokeSubscriptionResp::RevokeSubscriptionResp(RevokeSubscriptionResp* other)
: Response(CommandID_Revoke), capability_(other->getCapability()), result_(other->getResult()) {
}

std::string RevokeSubscriptionResp::encode() {
    std::ostringstream stream;
    stream << capability_.encode() << FIELD_SEPARATOR << result_ << FIELD_SEPARATOR; 
    
    return Response::encode( stream.str() );
}

bool RevokeSubscriptionResp::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!Response::decode( buffer, len, pos ) )
        return false;        
    
    std::string streamCap;
    if (!getField( aux, pos, streamCap ) )
        return false;        
    
    if (!capability_.decode(streamCap) )
        return false;            
    
    long auxResult;
    if (!getField( aux, pos, auxResult ) )
        return false;
    result_ = static_cast<RevokeResult>(auxResult);
    
    LOG->LogMessage(LOG_DEBUG,"RevokeSubscriptionResp::decode(): Capability type=%d id=%d result=%d", (int)capability_.getType(), capability_.getId(), (int) result_);
    return true;
}
