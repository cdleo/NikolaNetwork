/* 
 * File:   PredefinedServicesAndCaps.h
 * Author: cleo
 *
 * Created on May 20, 2014, 9:27 AM
 */

#ifndef PREDEFINED_SERVICES_AND_CAPABILITIES_H
#define	PREDEFINED_SERVICES_AND_CAPABILITIES_H

#include <string>    
#include <string.h>

namespace Synapse {
    
//************************* SERVICES *****************************
#define ID_FULLNETWORK_SERVICE                  1
#define ID_USER_VALIDATION_SERVICE              2
#define ID_USER_SECURITY_LEVEL_QUERY_SERVICE    3

#define MAX_PREDEFINED_SERVICE                  ID_USER_SECURITY_LEVEL_QUERY_SERVICE    
    
//*********************** CAPABILITIES ***************************
#define ID_NETWORKUPDATE_CAPABILITY             1
#define ID_REVOKED_USERS_CAPABILITY             2
#define ID_RESOURCESUPDATE_CAPABILITY           3
    
#define MAX_PREDEFINED_CAPABILITY               ID_RESOURCESUPDATE_CAPABILITY        
    
};

#endif	/* PREDEFINED_SERVICES_AND_CAPABILITIES_H */

