/* 
 * File:   IdentResp.h
 * Author: cleo
 *
 * Created on July 3, 2014, 10:25 AM
 */

#ifndef IDENTRESP_H
#define	IDENTRESP_H

#include <synapse/items/Ident.h>
#include <synapse/Response.h>

class IdentResp : public Response {	
public:    
    IdentResp(const Ident& ident);
    IdentResp(Header& header);
    virtual ~IdentResp(){};
    
    Ident getIdent() { return ident_; } 

    virtual std::string encode();
    virtual bool decode(const void* buffer, size_t len);

private:
    Ident         ident_;
};

#endif	/* IDENTRESP_H */

