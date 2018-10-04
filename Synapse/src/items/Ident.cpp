
#include <synapse/items/Ident.h>

Ident::Ident() 
: host_(""), port_(0), moduletype_(ModuleType_Undefined), moduleDescription_("") {
}

Ident::Ident(const char* host, unsigned int port, ModuleType moduletype, const char* moduleDescription) {
    host_.assign( host );
    port_ = port;
    moduletype_ = moduletype;
    moduleDescription_.assign( moduleDescription );
}        

Ident::Ident(const Ident& other) {
    host_.assign( other.host_ );
    port_ = other.port_;
    moduletype_ = other.moduletype_;
    moduleDescription_.assign( other.moduleDescription_ );
}

std::string Ident::encode(){
    std::ostringstream stream;
    stream << host_ << SUB_FIELD_SEPARATOR << port_ << SUB_FIELD_SEPARATOR 
           << moduletype_ <<  SUB_FIELD_SEPARATOR << moduleDescription_ << SUB_FIELD_SEPARATOR;  
    return stream.str();    
}

bool Ident::decode(std::string buffer){
    unsigned int pos = 0;   
    
    if (!getSubField( buffer, pos, host_ ) )
        return false;

    if (!getSubField( buffer, pos, port_ ) )
        return false;

    long aux;
    if (!getSubField( buffer, pos, aux ) )
        return false; 
    moduletype_ = static_cast<ModuleType>(aux);
    
    if (!getSubField( buffer, pos, moduleDescription_ ) )
        return false;      
    
    return true;
}

bool Ident::equals(const Ident& other) {
    return ( ( host_.compare( other.host_ ) == 0) && 
             ( port_ == other.port_) &&
             ( moduletype_ == other.moduletype_ ) &&
             ( moduleDescription_.compare( other.moduleDescription_) == 0 ) );
}