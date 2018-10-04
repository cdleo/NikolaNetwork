/* 
 * File:   ActionsManager.h
 * Author: cleo
 *
 * Created on August 8, 2014, 9:54 AM
 */

#ifndef ACTIONSMANAGER_H
#define	ACTIONSMANAGER_H

#include <support/GlobalDefinitions.h>
#include <SingletonBase.h>

#define ACTIONS ActionsManager::getInstance()

class ActionsManager : public SingletonBase<ActionsManager> {
public:
    friend class SingletonBase<ActionsManager>;
    
    void load();
    
    void addAction(ActionDefinition action) {
        actions_.push_back( action );
    }    
    
    ActionsList& getActions() {
        return actions_;
    }
     
    void onChange(long capabilityID, bool value);
    void onChange(long capabilityID, long value);
     
private:
    ActionsManager();
    virtual ~ActionsManager() throw();
    
    ActionsList            actions_;
};

#endif	/* ACTIONSMANAGER_H */

