
#include <synapse/items/SynapseItem.h>

bool SynapseItem::getSubField(std::string aux, unsigned int& pos, void* field ){
    unsigned int newPos = aux.find(SUB_FIELD_SEPARATOR, pos);
    if (newPos != pos) {
        std::string rawField = aux.substr( pos, newPos - pos );
        memcpy(field, (const void*)rawField.c_str(), rawField.length());
        pos = newPos + 1;
        return true;
    }
    return false;
}

bool SynapseItem::getSubField(std::string aux, unsigned int& pos, std::string& field ){
    unsigned int newPos = aux.find(SUB_FIELD_SEPARATOR, pos);
    if (newPos != pos) {
        field.assign ( aux.substr( pos, newPos - pos ) );
        pos = newPos + 1;
        return true;
    }
    return false;
}

bool SynapseItem::getSubField(std::string aux, unsigned int& pos, unsigned int& field ){
    std::string value;
    bool res = getSubField(aux, pos, value);
    if (res) field = UTILITIES->lexical_cast<unsigned int>(value);
    return res;
}    

bool SynapseItem::getSubField(std::string aux, unsigned int& pos, long& field ){
    std::string value;
    bool res = getSubField(aux, pos, value);
    if (res) field = UTILITIES->lexical_cast<long>(value);
    return res;
}  

bool SynapseItem::getSubField(std::string aux, unsigned int& pos, bool& field ){
    std::string value;
    bool res = getSubField(aux, pos, value);
    if (res) field = UTILITIES->lexical_cast<bool>(value);
    return res;
}      