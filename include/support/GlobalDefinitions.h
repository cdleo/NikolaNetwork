/* 
 * File:   GlobalDefinitions.h
 * Author: cleo
 *
 * Created on August 20, 2014, 2:29 PM
 */

#ifndef GLOBALDEFINITIONS_SUPPORT_H
#define	GLOBALDEFINITIONS_SUPPORT_H

#include <map>
#include <vector>
#include <string>

typedef std::map<long, long>            MAP_ANALOG_VALUES;
typedef std::map<long, bool>            MAP_DIGITAL_VALUES;
typedef std::map<std::string, long>     MAP_USERS_LEVELS;

typedef enum ActionsTypes {
    Action_Undefined = 0,
    Action_Set = 1,
    Action_Unset = 2,
    Action_Toggle = 3,
    Action_PushButton = 4,
    Action_Increment = 5,
    Action_Decrement = 6
} ActionsTypes;

typedef struct ActionDefinition {
    
    ActionDefinition() : fromCapId(0), toCapId(0), action(Action_Undefined), description("") {};
    
    int                 fromCapId;
    int                 toCapId;
    ActionsTypes        action;
    std::string         description;
} ActionDefinition;

typedef std::vector<ActionDefinition>       ActionsList;


typedef struct SubscriptionData {
    SubscriptionData(std::string peerID, std::string userHash) 
    : peerID_(peerID), userHash_(userHash) {};
    
    bool equals(const SubscriptionData& other){
        return ((peerID_.compare(other.peerID_) == 0) &&
                (userHash_.compare(other.userHash_) == 0) );
    }
    
    std::string         peerID_;
    std::string         userHash_;
} SubscriptionData;

typedef std::map<long /* capabilityID */, std::string /*peerID*/>       SUBSCRIPTIONS_MAP;

typedef std::vector<SubscriptionData>                                   SUBSCRIPTORS_LIST;
typedef std::map<long /* capabilityID */, SUBSCRIPTORS_LIST >           SUBSCRIPTORS_MAP;

#endif	/* GLOBALDEFINITIONS_SUPPORT_H */

