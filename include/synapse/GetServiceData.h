/* 
 * File:   GetServiceData.h
 * Author: cleo
 *
 * Created on July 7, 2014, 9:46 PM
 */

#ifndef GETDATA_H
#define	GETDATA_H

#include <synapse/items/ServiceDefinition.h>
#include <synapse/Request.h>

class GetServiceData : public Request {	
public:    
    GetServiceData(SubCommandID subcommandID, ServiceDefinition service);
    GetServiceData(Header& header) : Request(header) {};
    virtual ~GetServiceData(){};
    
    ServiceDefinition getService() { return service_; }

    std::string encode();
    bool decode(const void* buffer, size_t len);

    virtual std::string encode( std::string currentData = std::string("") );
    virtual bool decode(const void* buffer, size_t len, unsigned int& pos);    
    
protected:
    ServiceDefinition        service_;
};

#endif	/* GETDATA_H */

