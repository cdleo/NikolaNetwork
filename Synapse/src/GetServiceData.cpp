
#include <synapse/GetServiceData.h>

GetServiceData::GetServiceData(SubCommandID subcommandID, ServiceDefinition service) 
: Request(CommandID_GetValue, subcommandID), service_(service) {
    
}

std::string GetServiceData::encode() {
    std::ostringstream stream;
    stream << service_.encode() << FIELD_SEPARATOR; 

    return Request::encode( stream.str() );
}

std::string GetServiceData::encode( std::string currentData ) {
    std::ostringstream stream;
    stream << service_.encode() << FIELD_SEPARATOR << currentData; 

    return Request::encode( stream.str() );
}

bool GetServiceData::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!Request::decode(buffer, len, pos))
        return false;
    
    if (!getField( aux, pos, &service_ ) )
        return false;        
    
    LOG->LogMessage(LOG_DEBUG,"GetServiceData::decode(): Service type=%d id=%d", (int)service_.getType(), service_.getId());
    return true;
}

bool GetServiceData::decode(const void* buffer, size_t len, unsigned int& pos) {

    std::string aux = std::string((const char*) buffer, len);

    if (!Request::decode(buffer, len, pos))
        return false;    
    
    if (!getField( aux, pos, &service_ ) )
        return false;   

    LOG->LogMessage(LOG_DEBUG,"GetServiceData::decode(): Service type=%d id=%d", (int)service_.getType(), service_.getId());
    return true;
}