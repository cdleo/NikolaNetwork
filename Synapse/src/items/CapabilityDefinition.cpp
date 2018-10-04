
#include <synapse/items/CapabilityDefinition.h>

CapabilityDefinition::CapabilityDefinition() 
: id(0), type(Capability_Undefined), access(AccessType_Undefined), minReadSecuritylevel(0), 
  minWriteSecuritylevel(0), allowMultipleReadSubscriptions(true), 
  allowMultipleWriteSubscriptions(false), description("") {
}

CapabilityDefinition::CapabilityDefinition(const CapabilityDefinition& other){
    id = other.id;
    type = other.type;
    access = other.access;
    minReadSecuritylevel = other.minReadSecuritylevel;
    minWriteSecuritylevel = other.minWriteSecuritylevel;
    allowMultipleReadSubscriptions = other.allowMultipleReadSubscriptions;
    allowMultipleWriteSubscriptions = other.allowMultipleWriteSubscriptions;
    description.assign( other.description );
}    

std::string CapabilityDefinition::encode() {
    std::ostringstream stream;
    stream << id << SUB_FIELD_SEPARATOR << type << SUB_FIELD_SEPARATOR
           << access << SUB_FIELD_SEPARATOR << minReadSecuritylevel << SUB_FIELD_SEPARATOR 
           << minWriteSecuritylevel << SUB_FIELD_SEPARATOR << allowMultipleReadSubscriptions << SUB_FIELD_SEPARATOR  
           << allowMultipleWriteSubscriptions << SUB_FIELD_SEPARATOR << description.c_str() << SUB_FIELD_SEPARATOR;  

    return stream.str();
}

bool CapabilityDefinition::decode(std::string buffer) {

    unsigned int pos = 0;
    if (!getSubField( buffer, pos, id ) )
        return false;

    long aux;
    if (!getSubField( buffer, pos, aux ) )
        return false;
    type = static_cast<CapabilitiesTypes>(aux);
            
    if (!getSubField( buffer, pos, aux ) )
        return false;
    access = static_cast<AccessTypes>(aux);
            
    if (!getSubField( buffer, pos, minReadSecuritylevel ) )
        return false;

    if (!getSubField( buffer, pos, minWriteSecuritylevel ) )
        return false;

    if (!getSubField( buffer, pos, allowMultipleReadSubscriptions ) )
        return false;

    if (!getSubField( buffer, pos, allowMultipleWriteSubscriptions ) )
        return false;

    if (!getSubField( buffer, pos, description ) )
        return false;

    return true;
}
