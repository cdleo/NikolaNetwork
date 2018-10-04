
#include <synapse/GetServiceDataResp.h>

GetServiceDataResp::GetServiceDataResp(SubCommandID subcommandID, ServiceDefinition service, GetDataResult result) 
: Response(CommandID_GetValue, subcommandID), service_(service), result_(result) {

}

GetServiceDataResp::~GetServiceDataResp() {
}

std::string GetServiceDataResp::encode( ) {
    std::ostringstream stream;
    stream << service_.encode() << FIELD_SEPARATOR << result_ << FIELD_SEPARATOR; 

    return Response::encode( stream.str() );
}

std::string GetServiceDataResp::encode( std::string currentData ) {
    std::ostringstream stream;
    stream << service_.encode() << FIELD_SEPARATOR << result_ << FIELD_SEPARATOR << currentData; 
        
    return Response::encode( stream.str() );
}

bool GetServiceDataResp::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!Response::decode(buffer, len, pos))
        return false;
    
    if (!getField( aux, pos, &service_ ) )
        return false;        
    
    long auxResult;
    if (!getField( aux, pos, auxResult ) )
        return false;
    result_ = static_cast<GetDataResult>(auxResult);
    
    LOG->LogMessage(LOG_DEBUG,"GetServiceDataResp::decode(): Service type=%d id=%d result=%d", (int)service_.getType(), service_.getId(), (int) result_);
    return true;
}

bool GetServiceDataResp::decode(const void* buffer, size_t len, unsigned int& pos) {

    std::string aux = std::string((const char*) buffer, len);

    if (!Response::decode(buffer, len, pos))
        return false;    
    
    if (!getField( aux, pos, &service_ ) )
        return false;        
    
    long auxResult;
    if (!getField( aux, pos, auxResult ) )
        return false;
    result_ = static_cast<GetDataResult>(auxResult);
    
    LOG->LogMessage(LOG_DEBUG,"GetServiceDataResp::decode(): Service type=%d id=%d result=%d", (int)service_.getType(), service_.getId(), (int) result_);
    return true;
}

