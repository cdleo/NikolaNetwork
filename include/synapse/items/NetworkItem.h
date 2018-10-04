/* 
 * File:   NetworkItem.h
 * Author: cleo
 *
 * Created on August 25, 2014, 4:22 PM
 */

#ifndef NETWORKITEM_H
#define	NETWORKITEM_H

#include <synapse/items/SynapseItem.h>

using namespace Synapse;

class NetworkItem : public SynapseItem {
public:
    NetworkItem();
    NetworkItem(const char* ip, NetworkItem_Status st = NetworkItemStatus_Undefined);
    NetworkItem(const NetworkItem& other);
    virtual ~NetworkItem() {};

    virtual std::string encode();
    virtual bool decode(std::string buffer);

    bool equals(const NetworkItem& other);

    void setType(NetworkItem_Status type) {
        this->type_ = type;
    }

    NetworkItem_Status getType() const {
        return type_;
    }

    void setIp(std::string ip) {
        this->ip_ = ip;
    }

    std::string getIp() const {
        return ip_;
    }
    
private:    
    std::string             ip_;
    NetworkItem_Status      type_;        
};

#endif	/* NETWORKITEM_H */

