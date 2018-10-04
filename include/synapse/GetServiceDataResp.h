/* 
 * File:   GetServiceDataResp.h
 * Author: cleo
 *
 * Created on July 16, 2014, 11:55 AM
 */

#ifndef GETDATARESP_H
#define	GETDATARESP_H

#include <synapse/items/ServiceDefinition.h>
#include <synapse/Response.h>

using namespace Synapse;

class GetServiceDataResp : public Response {	
public:    
    GetServiceDataResp(SubCommandID subcommandID, ServiceDefinition service, GetDataResult result);
    GetServiceDataResp(Header& header) : Response(header) {};
    virtual ~GetServiceDataResp();
    
    ServiceDefinition getService() { return service_; }
    
    GetDataResult getResult() { return result_; } 
    
    virtual std::string encode( );
    virtual bool decode(const void* buffer, size_t len);
    
    virtual std::string encode( std::string currentData = std::string("") );
    virtual bool decode(const void* buffer, size_t len, unsigned int& pos);
    

private:
    ServiceDefinition         service_;
    GetDataResult             result_;       
};

#endif	/* GETDATARESP_H */

