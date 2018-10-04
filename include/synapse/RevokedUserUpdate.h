/* 
 * File:   RevokedUserUpdate.h
 * Author: cleo
 *
 * Created on September 12, 2014, 1:28 PM
 */

#ifndef REVOKEDUSERUPDATE_H
#define	REVOKEDUSERUPDATE_H

#include <synapse/Update.h>
#include <vector>
#include <algorithm>

class RevokedUserUpdate : public Update {	
public:    
    RevokedUserUpdate(CapabilityDefinition def, std::string revokedUserHash);
    RevokedUserUpdate(Header& header) : Update(header) {};
    virtual ~RevokedUserUpdate(){};

    std::string getRevokedUserHash() { return revokedUserHash_; }
    
    std::string encode();
    bool decode(const void* buffer, size_t len);

private:
    std::string revokedUserHash_;
};

#endif	/* REVOKEDUSERUPDATE_H */

