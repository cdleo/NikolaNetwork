
#include <synapse/GetValueResp.h>

GetValueResp::GetValueResp(SubCommandID subCommandId, CapabilityDefinition capability, std::string userHash, GetDataResult result) 
: Response(CommandID_GetValue, subCommandId), capability_(capability), userHash_(userHash), result_(result) {

}

std::string GetValueResp::encode( ) {
    std::ostringstream stream;
    stream << capability_.encode() << FIELD_SEPARATOR << userHash_.c_str() << FIELD_SEPARATOR << result_ << FIELD_SEPARATOR; 
        
    return Response::encode( stream.str() );
}

std::string GetValueResp::encode( std::string currentData ) {
    std::ostringstream stream;
    stream << capability_.encode() << FIELD_SEPARATOR << userHash_.c_str() << FIELD_SEPARATOR << result_ << FIELD_SEPARATOR << currentData; 
        
    return Response::encode( stream.str() );
}

bool GetValueResp::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!Response::decode( buffer, len, pos))
        return false;
    
    std::string streamCap;
    if (!getField( aux, pos, streamCap ) )
        return false;        
    
    if (!capability_.decode(streamCap) )
        return false;            
    
    if (!getField( aux, pos, userHash_ ) ) {     //Si el hash está vacío, solo es válido si el nivel de seguridad es 0 y es de lectura
        if ((capability_.getAccess() != AccessType_Read) && (capability_.getMinReadSecuritylevel() != 0)) {
            return false;    
        } else {
            pos++;
        }   
    }
    
    if (!getField( aux, pos, (long&) result_ ) )
        return false;

    LOG->LogMessage(LOG_DEBUG,"GetValueResp::decode(): Capability type=%d id=%d result=%d", (int)capability_.getType(), capability_.getId(), (int) result_);
    return true;
}

bool GetValueResp::decode(const void* buffer, size_t len, unsigned int& pos) {

    std::string aux = std::string((const char*) buffer, len);

    if (!Response::decode( buffer, len, pos))
        return false;    
    
    std::string streamCap;
    if (!getField( aux, pos, streamCap ) )
        return false;        
    
    if (!capability_.decode(streamCap) ) 
        return false;     
    
    if (!getField( aux, pos, userHash_ ) )      //Si el hash está vacío, solo es válido si el nivel de seguridad es 0 y es de lectura
        if ((capability_.getAccess() != AccessType_Read) && (capability_.getMinReadSecuritylevel() != 0))
            return false;        
    
    long auxResult;
    if (!getField( aux, pos, auxResult ) )
        return false;
    result_ = static_cast<GetDataResult>(auxResult);
    
    LOG->LogMessage(LOG_DEBUG,"GetValueResp::decode(): Capability type=%d id=%d result=%d", (int)capability_.getType(), capability_.getId(), (int) result_);
    return true;
}

