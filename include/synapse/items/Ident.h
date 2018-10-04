/* 
 * File:   Ident.h
 * Author: cleo
 *
 * Created on August 25, 2014, 4:02 PM
 */

#ifndef IDENT_H
#define	IDENT_H

#include <synapse/items/SynapseItem.h>

using namespace Synapse;

class Ident : public SynapseItem {
public:    
    Ident();
    Ident(const char* host, unsigned int port, ModuleType moduletype, const char* moduleDescription);
    Ident(const Ident& other);
    virtual ~Ident() {};
    
    virtual std::string encode();
    virtual bool decode(std::string buffer);

    bool equals(const Ident& other);        

    void setModuleDescription(std::string moduleDescription) {
        this->moduleDescription_ = moduleDescription;
    }

    std::string getModuleDescription() const {
        return moduleDescription_;
    }

    void setModuletype(ModuleType moduletype) {
        this->moduletype_ = moduletype;
    }

    ModuleType getModuletype() const {
        return moduletype_;
    }
    
    void setPort(unsigned int port) {
        this->port_ = port;
    }

    unsigned int getPort() const {
        return port_;
    }

    void setHost(std::string host) {
        this->host_ = host;
    }

    std::string getHost() const {
        return host_;
    }

private:
    std::string     host_;
    unsigned int    port_;
    ModuleType      moduletype_;
    std::string     moduleDescription_;    
};  

#endif	/* IDENT_H */

