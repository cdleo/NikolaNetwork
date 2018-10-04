
#include <synapse/ServicesResp.h>

ServicesResp::ServicesResp() 
: Response(CommandID_Services) {
    
}

std::string ServicesResp::encode() {
    std::ostringstream stream;
    stream << servicesList_.size() << FIELD_SEPARATOR;
    for (ServicesList::iterator it = servicesList_.begin(); it != servicesList_.end(); it++) {
        stream << it->encode() << FIELD_SEPARATOR;
    }    
    
    return Response::encode( stream.str() );
}

bool ServicesResp::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);
    
    unsigned int pos = 0;
    if (!Response::decode(buffer,len, pos))
        return false;
    
    unsigned int cantServ = 0;
    if (!getField( aux, pos, cantServ ) )
        return false;

    for (unsigned int i = 0; i < cantServ; i++) {
        ServiceDefinition serv;
        if (!getField( aux, pos, &serv ) )
            return false;
        
        servicesList_.push_back( serv );
    }     
    
    LOG->LogMessage(LOG_DEBUG,"ServicesResp::decode(): %u services returned!",servicesList_.size());
    return true;
}


