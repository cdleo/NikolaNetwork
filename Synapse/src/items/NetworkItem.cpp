
#include <synapse/items/NetworkItem.h>

NetworkItem::NetworkItem() 
: ip_(""), type_(NetworkItemStatus_Undefined) {
}

NetworkItem::NetworkItem(const char* ip, NetworkItem_Status st) {
    ip_.assign(ip);
    type_ = st;
}

NetworkItem::NetworkItem(const NetworkItem& other){
    ip_.assign(other.ip_);
    type_ = other.type_;
}           

std::string NetworkItem::encode(){
    std::ostringstream stream;
    stream << ip_ << SUB_FIELD_SEPARATOR << type_ << SUB_FIELD_SEPARATOR;  
    return stream.str();  
}

bool NetworkItem::decode(std::string buffer){
    unsigned int pos = 0;
    if (!getSubField( buffer, pos, ip_ ) )
        return false;

    long aux;
    if (!getSubField( buffer, pos, aux ) )
        return false;
    type_ = static_cast<NetworkItem_Status>(aux);
    
    return true;
}

bool NetworkItem::equals(const NetworkItem& other){
    return ( (type_ == other.type_) && (ip_.compare(other.ip_) == 0) );    
}
