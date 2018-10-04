/* 
 * File:   ActionsManager.cpp
 * Author: cleo
 * 
 * Created on July 16, 2014, 1:40 PM
 */

#include <support/ActionsManager.h>
#include <support/DataStorage.h>
#include <Parameters.h>

#include <BaseUtilities.h>

#include <stdlib.h>
#include <string.h>

ActionsManager::ActionsManager() {
    actions_.clear();
}

ActionsManager::~ActionsManager() throw() {
    actions_.clear();
}

void ActionsManager::load(){
    std::vector<std::string> actions;
    PARAMETERS->getValue("actions", actions);    
    for ( std::vector<std::string>::iterator it = actions.begin(); it != actions.end(); it++ ){
        std::vector<std::string> splitedAction = UTILITIES->split( *it, '|' );
        if (splitedAction.size() != 4) continue;
    
        ActionDefinition def;
        def.fromCapId = atoi(splitedAction.at(0).c_str());
        def.toCapId = atoi(splitedAction.at(1).c_str());
        def.action = (ActionsTypes) atoi(splitedAction.at(2).c_str());
        def.description.assign ( splitedAction.at(3).c_str() );                
        actions_.push_back(def);          
    }
}

void ActionsManager::onChange(long capabilityID, bool value){

    for (ActionsList::iterator it = actions_.begin(); it != actions_.end(); it++){
        if ( it->fromCapId == capabilityID )  {
            switch (it->action) {
                case Action_Set: {
                    if (value)
                        STORAGE->setValue( it->toCapId, true );
                    break;
                }
                    
                case Action_Unset: {
                    if (value)
                        STORAGE->setValue( it->toCapId, false );
                    break;
                }
                    
                case Action_Toggle: {
                    if (value) {
                        bool valorActual;
                        if (STORAGE->getValue( it->toCapId, valorActual ) == GetDataResult_Ok)
                            STORAGE->setValue( it->toCapId, !valorActual );
                        else
                            STORAGE->setValue( it->toCapId, value );
                    }
                    break;
                }
                
                case Action_PushButton: {
                    STORAGE->setValue( it->toCapId, value );
                    break;
                }                
                
                default: 
                    break;
            }
        }
    }
}

void ActionsManager::onChange(long capabilityID, long value){
    for (ActionsList::iterator it = actions_.begin(); it != actions_.end(); it++){
        if ( it->fromCapId == capabilityID )  {
            switch (it->action) {
                case Action_Set: {
                    STORAGE->setValue( it->toCapId, value );
                    break;
                }

                case Action_Increment: {
                    long valorActual;
                    if (STORAGE->getValue( it->toCapId, valorActual ) == GetDataResult_Ok)
                        STORAGE->setValue( it->toCapId, valorActual + value);
                    else
                        STORAGE->setValue( it->toCapId, value);
                    break;
                }

                case Action_Decrement: {
                    long valorActual;
                    if (STORAGE->getValue( it->toCapId, valorActual ) == GetDataResult_Ok)
                        STORAGE->setValue( it->toCapId, valorActual - value);
                    else
                        STORAGE->setValue( it->toCapId, (long) 0);
                    break;
                }
                
                default: 
                    break;
            }
        }
    }  
}

