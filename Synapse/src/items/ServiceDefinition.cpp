
#include <synapse/items/ServiceDefinition.h>

ServiceDefinition::ServiceDefinition() 
: id(0), type(ServiceType_Undefined), description("") {
}

ServiceDefinition::ServiceDefinition(const ServiceDefinition& other){
    id = other.id;
    type = other.type;
    description.assign( other.description );
}    

std::string ServiceDefinition::encode(){
    std::ostringstream stream;
    stream << id << SUB_FIELD_SEPARATOR << type << SUB_FIELD_SEPARATOR << description.c_str() << SUB_FIELD_SEPARATOR;  
    return stream.str();    
}

bool ServiceDefinition::decode(std::string buffer){
    unsigned int pos = 0;
    if (!getSubField( buffer, pos, id ) )
        return false;

    long aux;
    if (!getSubField( buffer, pos, aux ) )
        return false;
    type = static_cast<ServiceTypes>(aux);
    
    if (!getSubField( buffer, pos, description ) )
        return false; 
    
    return true;
}


