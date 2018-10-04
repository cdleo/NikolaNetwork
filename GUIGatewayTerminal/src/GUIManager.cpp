/* 
 * File:   GUIManager.cpp
 * Author: cleo
 * 
 * Created on October 13, 2014, 11:42 PM
 */

#include <GUIManager.h>
#include <Parameters.h>
#include <GlobalDefinitions.h>
#include <ConnectionsHandlers.h>

#include <GUI/INikolaWidget.h>
#include <GUI/GUIWidgetDimension.h>

#include <support/CapabilitiesManager.h>
#include <support/ServicesManager.h>
#include <support/DataStorage.h>

#include <synapse/PredefinedServicesAndCaps.h>
#include <synapse/Messages.h>

#include <QGroupBox>
#include <QBoxLayout>
#include <QtGui>
#include <QtCore/qnamespace.h>
#include <QtCore/qstring.h>
        
#include <QFile>
#include <QTextStream>


using namespace Synapse;

GUIManager::GUIManager() 
: mainLayout_(NULL), mainWindow_(NULL), 
  widgetsContainer_(NULL), controlsContainer_(NULL), resourcesPath_("") {

}

GUIManager::~GUIManager() throw() {
}

bool GUIManager::initialize(QMainWindow* mainWindow){
    
    mainWindow_ = mainWindow;
    mainWindow_->resize(800, 600);

    PARAMETERS->getValue("resourcesPath", resourcesPath_);
    
    QWidget* centralWidget = new QWidget(mainWindow_);
    centralWidget->setObjectName(QString("centralWidget"));    
    centralWidget->setObjectName("centralWidget");
    centralWidget->setStyleSheet("#centralWidget {background-color:blue;}");
    
    mainLayout_ = new QBoxLayout( QBoxLayout::LeftToRight, centralWidget);

    widgetsContainer_ = new WidgetsContainer(centralWidget, this);
    mainLayout_->addWidget( widgetsContainer_ ); 

    controlsContainer_ = new ControlsContainer(centralWidget, this);
    mainLayout_->addWidget( controlsContainer_ );    

    mainWindow_->setCentralWidget(centralWidget);  
    
    registerGUITypes();
    
    return addDefaultWidgets();
}

bool GUIManager::registerGUITypes() {
    qRegisterMetaType<GUICapabilityDefinition>("GUICapabilityDefinition");
    qRegisterMetaType<GUIWidgetDimension>("GUIWidgetDimension");
    return true;
}

bool GUIManager::addDefaultWidgets() {
    
    GUICapabilityDefinition guiDef;
    bool result = true;
    
    ServicesList services = SERVICES->getLocalServices();
    
    for (ServicesList::iterator it = services.begin(); it != services.end(); it++) {
        
        GUIWidgetDimension guiDim;
        ServiceDefinition def = *it;
        guiDef.setId( def.getId() );
        guiDef.setAccess( AccessType_RW );
        guiDef.setResourcesPath( resourcesPath_ );
        guiDef.setImgAccionatorPrefix( "button" );
        guiDef.setAccionatorColor( NW_Black );
        guiDef.setCaptionColor( Qt::white );
            
        if (def.getId() == ID_CAPABILITIES_LIST_SERVICE) {
            guiDef.setWidgetType( WidgetType_CapabilitiesList );
            guiDim.setRowIndex(0);
            guiDim.setColumnIndex(0);
            guiDim.setRowSpan(1);
            guiDim.setColumnSpan(2);
            result = result && controlsContainer_->addWidget( guiDef, guiDim );              
            
        } else if (def.getId() == ID_LOAD_CONTROLS_SERVICE) {
            guiDef.setCaption(QObject::tr("Load") );
            guiDef.setWidgetType( WidgetType_PushButton );
            guiDim.setRowIndex(1);
            guiDim.setColumnIndex(0);
            result = result && controlsContainer_->addWidget( guiDef, guiDim );        
            
        } else if (def.getId() == ID_SAVE_CONTROLS_SERVICE) {
            guiDef.setCaption( QObject::tr("Save") );
            guiDef.setWidgetType( WidgetType_PushButton );
            guiDim.setRowIndex(1);
            guiDim.setColumnIndex(1);            
            result = result && controlsContainer_->addWidget( guiDef, guiDim );  
            
        }
    }
    return result;
}


bool GUIManager::addGUIRepresentation(CapabilityDefinition def){

    if (widgetsContainer_->capabilityAlreadyInLayout( def.getId() ))
        return true;
            
    GUICapabilityDefinition guiDef;
    guiDef.setId( def.getId() );
    guiDef.setAccess( def.getAccess() );
    guiDef.setType( def.getType() );
    guiDef.setResourcesPath( resourcesPath_ );
    
    switch( guiDef.getType() ) {
        case Capability_AnalogInput:
        {
            guiDef.setWidgetType( WidgetType_Slider );
            return widgetsContainer_->addWidget( guiDef );
        }

        case Capability_DigitalInput:
        {
            guiDef.setWidgetType( WidgetType_PushButton );
            guiDef.setImgAccionatorPrefix( "button" );
            guiDef.setAccionatorColor( NW_Black );
            guiDef.setCaption( QObject::tr("Cap %1").arg( def.getId() ) );
            guiDef.setCaptionColor( Qt::white );
            return widgetsContainer_->addWidget( guiDef );
        }

        case Capability_AnalogOutput:
            guiDef.setWidgetType( WidgetType_AnalogValue );
            return widgetsContainer_->addWidget( guiDef );

        case Capability_DigitalOutput:
        {
            guiDef.setWidgetType( WidgetType_LedIndicator );
            guiDef.setImgIndicatorPrefix( "ledlight" );
            guiDef.setImgAccionatorPrefix( "button" );
            guiDef.setAccionatorColor( NW_Black );
            if ( def.getId() == 106) {
                guiDef.setCaption( QObject::tr("%1").arg( def.getId() ) );
                guiDef.setIndicatorColor( NW_Green );
            } else if ( def.getId() == 107) {
                guiDef.setIndicatorColor( NW_Red );
            } else {
                guiDef.setIndicatorColor( NW_Blue );
            }
            guiDef.setCaptionColor( Qt::white );
            return widgetsContainer_->addWidget( guiDef );
        }

        default:
            return false;       //No tiene representacion grafica (todavia)
    }
 
}

bool GUIManager::updateGUIRepresentation(long capabilityId, bool value){
    
    bool result = widgetsContainer_->updateWidget( capabilityId, value );
    return result && controlsContainer_->updateWidget( capabilityId, value );
}

bool GUIManager::updateGUIRepresentation(long capabilityId, long value){
    
    bool result = widgetsContainer_->updateWidget( capabilityId, value );
    return result && controlsContainer_->updateWidget( capabilityId, value );
    
}

void GUIManager::onNewCapabilityRequested(long capId){
    Ident module;
    CapabilityDefinition cap;
    if (CAPABILITIES->getForeignCapabilityById(capId, cap, module)){
        Subscribe request(cap, STORAGE->getUserHash());
        CONN_HANDLERS->sendMessageToClient( module, (Packet&) request );
    }
}

void GUIManager::onInputValueChange(long id, bool value){
    Ident module;
    CapabilityDefinition cap;
    if (CAPABILITIES->getForeignCapabilityById(id, cap, module)){
        SetDigitalValue request(cap, STORAGE->getUserHash(), value);
        CONN_HANDLERS->sendMessageToClient( module, (Packet&) request );
        return;    
    }
}

void GUIManager::onInputValueChange(long id, long value){
    Ident module;
    CapabilityDefinition cap;
    if (CAPABILITIES->getForeignCapabilityById(id, cap, module)){
        SetAnalogValue request(cap, STORAGE->getUserHash(), value);
        CONN_HANDLERS->sendMessageToClient( module, (Packet&) request ); 
        return;
    }      
}

void GUIManager::onCapabilityRemoved(long capId){
    Ident module;
    CapabilityDefinition cap;
    if (CAPABILITIES->getForeignCapabilityById(capId, cap, module)){
        Unsubscribe request(cap, STORAGE->getUserHash());
        CONN_HANDLERS->sendMessageToClient( module, (Packet&) request );
    }
}

void GUIManager::onServiceRequested(long serviceId){
    ServiceDefinition service;
    if (SERVICES->getServiceById(serviceId, SourceData_Local, service)){
        switch (serviceId) {
            case ID_LOAD_CONTROLS_SERVICE:
                loadWidgets();
                break;
                
            case ID_SAVE_CONTROLS_SERVICE:
                saveWidgets();
                break;
                
            default:
                break;
        }
        return;
    }
}

void GUIManager::loadWidgets() {
    
    QFile file("./layout.cfg");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    widgetsContainer_->clear();
    
    QTextStream in(&file);
    while (!in.atEnd()) {
        QString line = in.readLine();
        GUICapabilityDefinition def;  
        if (def.decode(line)){
            Ident module;
            CapabilityDefinition cap;
            if (CAPABILITIES->getForeignCapabilityById(def.getId(), cap, module)){
                Subscribe request(cap, STORAGE->getUserHash());
                CONN_HANDLERS->sendMessageToClient( module, (Packet&) request );            
            }                 
        }
        widgetsContainer_->addWidget( def );
    }
    
    file.close();
}

void GUIManager::saveWidgets() {
    
    QFile file("./layout.cfg");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return;

    QTextStream out(&file);
   
    WidgetsList widgets = widgetsContainer_->getWidgetsList();
    for (WidgetsList::iterator it = widgets.begin(); it != widgets.end(); it++){
        GUICapabilityDefinition def = *it;
        out << def.encode() << "\n";   
    }
    
    file.close();
}
