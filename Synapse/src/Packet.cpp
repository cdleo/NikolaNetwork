
#include <synapse/Packet.h>
#include <BaseUtilities.h>

Packet::Packet(Header& header) 
: h_(header) 
{
}
    
Packet::Packet(MsgType msgType, CommandID commandID, SubCommandID subCommandID)
: h_(msgType, commandID, subCommandID)
{
}
    
std::string Packet::encode( const std::string currentData ){ 
    h_.setLength( sizeof(Header) + currentData.length() );
    return h_.serialize().append( currentData ); 
} 

bool Packet::decode(const void* buffer, size_t len, unsigned int& pos){
    pos = 0;        //Aqui no hay nada que decodificar. El header se carga desde el constructor!
    return true;    
}   

bool Packet::getField(std::string aux, unsigned int& pos, SynapseItem* field ){
    unsigned int newPos = aux.find(FIELD_SEPARATOR, pos);
    if (newPos != pos) {
        std::string rawField = aux.substr( pos, newPos - pos );
        pos = newPos + 1;
        return field->decode(rawField);
    }
    return false;
}

bool Packet::getField(std::string aux, unsigned int& pos, void* field ){
    unsigned int newPos = aux.find(FIELD_SEPARATOR, pos);
    if (newPos != pos) {
        std::string rawField = aux.substr( pos, newPos - pos );
        memcpy(field, (const void*)rawField.c_str(), rawField.length());
        pos = newPos + 1;
        return true;
    }
    return false;
}

bool Packet::getField(std::string aux, unsigned int& pos, std::string& field ){
    unsigned int newPos = aux.find(FIELD_SEPARATOR, pos);
    if (newPos != pos) {
        field.assign ( aux.substr( pos, newPos - pos ) );
        pos = newPos + 1;
        return true;
    }
    return false;
}

bool Packet::getField(std::string aux, unsigned int& pos, unsigned int& field ){
    std::string value;
    bool res = getField(aux, pos, value);
    if (res) field = UTILITIES->lexical_cast<unsigned int>(value);
    return res;
}    

bool Packet::getField(std::string aux, unsigned int& pos, long& field ){
    std::string value;
    bool res = getField(aux, pos, value);
    if (res) field = UTILITIES->lexical_cast<long>(value);
    return res;
}  

bool Packet::getField(std::string aux, unsigned int& pos, bool& field ){
    std::string value;
    bool res = getField(aux, pos, value);
    if (res) field = UTILITIES->lexical_cast<bool>(value);
    return res;
}
