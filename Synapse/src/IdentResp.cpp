
#include <synapse/IdentResp.h>

IdentResp::IdentResp(const Ident& ident) 
: Response(CommandID_Ident), ident_(ident) {    
}

IdentResp::IdentResp(Header& header) 
: Response(header), ident_() {

}

std::string IdentResp::encode() {
    std::ostringstream stream;
    stream << ident_.encode() << FIELD_SEPARATOR; 
    
    return Response::encode( stream.str() );
}

bool IdentResp::decode(const void* buffer, size_t len) {

    std::string aux = std::string((const char*) buffer, len);

    unsigned int pos = 0;
    if (!Response::decode(buffer, len, pos))
        return false;
    
    if (!getField( aux, pos, &ident_ ) )
        return false;
    
    LOG->LogMessage(LOG_DEBUG,"IdentResp::decode(): host=%s port= %u type=%d name=%s",
                    ident_.getHost().c_str(), ident_.getPort(), ident_.getModuletype(), ident_.getModuleDescription().c_str());
    return true;
}

