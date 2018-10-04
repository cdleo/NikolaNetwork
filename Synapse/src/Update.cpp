
#include <synapse/Update.h>

Update::Update(SubCommandID subCommandId, CapabilityDefinition capability) 
: Response(CommandID_Update, subCommandId), capability_(capability) {
    
}

std::string Update::encode( std::string currentData ) {
    std::ostringstream stream;
    stream << capability_.encode() << FIELD_SEPARATOR << currentData; 
        
    return Response::encode( stream.str() );
}

bool Update::decode(const void* buffer, size_t len, unsigned int& pos) {

    std::string aux = std::string((const char*) buffer, len);

    if (!Response::decode( buffer, len, pos))
        return false;    
    
    std::string streamCap;
    if (!getField( aux, pos, streamCap ) )
        return false;        
    
    if (!capability_.decode(streamCap) )
        return false;             

    LOG->LogMessage(LOG_DEBUG,"Update::decode(): Capability type=%d id=%d", (int)capability_.getType(), capability_.getId());
    return true;
}



