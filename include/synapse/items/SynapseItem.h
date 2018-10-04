/* 
 * File:   SynapseItem.h
 * Author: cleo
 *
 * Created on August 25, 2014, 3:02 PM
 */

#ifndef SYNAPSEITEM_H
#define	SYNAPSEITEM_H

#include <synapse/GlobalDefinitions.h>
#include <BaseUtilities.h>

class SynapseItem {
public:
    SynapseItem(){};
    virtual ~SynapseItem(){};
    
    virtual std::string encode() = 0;
    virtual bool decode(std::string buffer) = 0;
    
protected:
    bool getSubField(std::string aux, unsigned int& pos, void* field );
    bool getSubField(std::string aux, unsigned int& pos, std::string& field );
    bool getSubField(std::string aux, unsigned int& pos, unsigned int& field );
    bool getSubField(std::string aux, unsigned int& pos, long& field );
    bool getSubField(std::string aux, unsigned int& pos, bool& field );
};
#endif	/* SYNAPSEITEM_H */

