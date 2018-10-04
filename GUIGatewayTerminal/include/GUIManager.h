/* 
 * File:   GUIManager.h
 * Author: cleo
 *
 * Created on October 13, 2014, 11:42 PM
 */

#ifndef GUIMANAGER_H
#define	GUIMANAGER_H

#include <GUI/GlobalDefinitions.h>
#include <GUI/IInputCallbacks.h>
#include <GUI/ControlsContainer.h>
#include <GUI/WidgetsContainer.h>

#include <SingletonBase.h>

#include <QMainWindow>
#include <QBoxLayout>
#include <QSignalMapper>

#include <synapse/items/CapabilityDefinition.h>

#define GUI GUIManager::getInstance()

class GUIManager : public SingletonBase<GUIManager>, public IInputCallbacks {
public:
    friend class SingletonBase<GUIManager>;
     
    bool initialize(QMainWindow* mainWindow);
    
    bool addGUIRepresentation(CapabilityDefinition def);

    bool updateGUIRepresentation(long capabilityId, bool value);
    bool updateGUIRepresentation(long capabilityId, long value);
    
    void onNewCapabilityRequested(long capabilityId);
    void onInputValueChange(long id, bool value);
    void onInputValueChange(long id, long value);    
    void onServiceRequested(long serviceId);
    void onCapabilityRemoved(long capId);
    
private:
    GUIManager();
    virtual ~GUIManager() throw();
    
    bool registerGUITypes();
    
    bool addDefaultWidgets();
    
    void loadWidgets();
    void saveWidgets();
    
    QBoxLayout*         mainLayout_;
    QMainWindow*        mainWindow_;
    
    WidgetsContainer*   widgetsContainer_;
    ControlsContainer*  controlsContainer_;     
    
    std::string         resourcesPath_;
};

#endif	/* GUIMANAGER_H */

