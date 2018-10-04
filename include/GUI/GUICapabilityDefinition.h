/* 
 * File:   GUICapabilityDefinition.h
 * Author: cleo
 *
 * Created on October 16, 2014, 4:17 PM
 */

#ifndef GUICAPABILITYDEFINITION_H
#define	GUICAPABILITYDEFINITION_H

#include <QMetaType>
#include <QPoint>
#include <QColor>
#include <QString>
#include <QTextStream>
#include <string>
#include <GUI/GlobalDefinitions.h>
#include <synapse/GlobalDefinitions.h>
#include <sstream>

using namespace Synapse;

class GUICapabilityDefinition {
    
    Q_ENUMS(CapabilitiesTypes)
    Q_ENUMS(WidgetTypes)
    Q_ENUMS(AccessTypes)
    Q_ENUMS(NWColor) 
    
public:
    GUICapabilityDefinition() : id(0), type(Capability_Undefined),
        widgetType(WidgetType_Unknown), access(AccessType_Undefined), 
        position(0,0), captionColor(Qt::transparent), 
        backgroundColor(NW_Transparent), accionatorColor(NW_Transparent), 
        indicatorColor(NW_Transparent), caption(""), resourcesPath(""),
        imgAccionatorPrefix(""), imgIndicatorPrefix("") {
    };
    
    GUICapabilityDefinition(const GUICapabilityDefinition& other) {
        id = other.id;
        type = other.type;
        widgetType = other.widgetType;
        access = other.access;
        position = other.position;
        resourcesPath.assign( other.resourcesPath );
        
        captionColor = other.captionColor;
        backgroundColor = other.backgroundColor;
        accionatorColor = other.accionatorColor;
        indicatorColor = other.indicatorColor;
        
        caption = other.caption;
        imgAccionatorPrefix.assign( other.imgAccionatorPrefix );
        imgIndicatorPrefix.assign( other.imgIndicatorPrefix );   
    }
    
    virtual ~GUICapabilityDefinition() {};
    
    bool decode(QString line) {
        
        QStringList list = line.split("&");
        if (list.count() != 13)
            return false;
        
        id = list.at(0).toLong();
        type = (CapabilitiesTypes) list.at(1).toLong();
        widgetType = (WidgetTypes) list.at(2).toLong();
        access = (AccessTypes) list.at(3).toLong();
        
        QStringList listAux = list.at(4).split("|");
        if (listAux.count() != 2)
            return false;
        position.setX( listAux.at(0).toInt() );
        position.setY( listAux.at(1).toInt() );
        
        resourcesPath.assign( list.at(5).toStdString() );
        
        captionColor = (Qt::GlobalColor) list.at(6).toLong();
        backgroundColor = (NWColor) list.at(7).toLong();
        accionatorColor = (NWColor) list.at(8).toLong();
        indicatorColor = (NWColor) list.at(9).toLong();
        
        caption = list.at(10);
        imgAccionatorPrefix.assign( list.at(11).toStdString() );
        imgIndicatorPrefix.assign( list.at(12).toStdString() );  
        
        return true;
    }

    QString encode() {
        std::ostringstream stream;

        stream << id << "&" << type << "&" << widgetType << "&" << access << "&" << position.x() << "|" << position.y() << "&" 
               << resourcesPath << "&" << captionColor << "&" << backgroundColor << "&"
               << accionatorColor << "&" << indicatorColor << "&" << caption.toStdString() << "&" 
               << imgAccionatorPrefix << "&" << imgIndicatorPrefix;
        
        return QString(stream.str().c_str());
    }
    
    void setResourcesPath(std::string resourcesPath) {
        this->resourcesPath = resourcesPath;
    }

    std::string getResourcesPath() const {
        return resourcesPath;
    }
    
    void setCaption(QString caption) {
        this->caption = caption;
    }

    QString getCaption() const {
        return caption;
    }

    void setImgAccionatorPrefix(std::string imgPrefix) {
        this->imgAccionatorPrefix = imgPrefix;
    }

    std::string getImgAccionatorPrefix() const {
        return imgAccionatorPrefix;
    }    
    
    void setImgIndicatorPrefix(std::string imgPrefix) {
        this->imgIndicatorPrefix = imgPrefix;
    }

    std::string getImgIndicatorPrefix() const {
        return imgIndicatorPrefix;
    }        
    
    void setBackgroundColor(NWColor backgroundColor) {
        this->backgroundColor = backgroundColor;
    }

    NWColor getBackgroundColor() const {
        return backgroundColor;
    }    

    void setAccionatorColor(NWColor accionatorColor) {
        this->accionatorColor = accionatorColor;
    }

    NWColor getAccionatorColor() const {
        return accionatorColor;
    }  

    void setIndicatorColor(NWColor indicatorColor) {
        this->indicatorColor = indicatorColor;
    }

    NWColor getIndicatorColor() const {
        return indicatorColor;
    }      
    
    void setCaptionColor(Qt::GlobalColor captionColor) {
        this->captionColor = captionColor;
    }

    Qt::GlobalColor getCaptionColor() const {
        return captionColor;
    }     
    
    void setAccess(AccessTypes access) {
        this->access = access;
    }

    AccessTypes getAccess() const {
        return access;
    }

    void setType(CapabilitiesTypes type) {
        this->type = type;
    }

    CapabilitiesTypes getType() const {
        return type;
    }

    void setWidgetType(WidgetTypes type) {
        this->widgetType = type;
    }

    WidgetTypes getWidgetType() const {
        return widgetType;
    }    
    
    void setPosition(QPoint value) {
        this->position = value;
    }

    QPoint getPosition() const {
        return position;
    }      
    
    void setId(long id) {
        this->id = id;
    }

    long getId() const {
        return id;
    }       
 
private:
    long                id;
    CapabilitiesTypes   type;
    WidgetTypes         widgetType;
    AccessTypes         access;
    QPoint              position;
    
    Qt::GlobalColor     captionColor;
    NWColor             backgroundColor;
    NWColor             accionatorColor;
    NWColor             indicatorColor;
    
    QString             caption;   
    std::string         resourcesPath;
    std::string         imgAccionatorPrefix;
    std::string         imgIndicatorPrefix;
};   

typedef std::vector<GUICapabilityDefinition> WidgetsList;

Q_DECLARE_METATYPE(CapabilitiesTypes);
Q_DECLARE_METATYPE(WidgetTypes);
Q_DECLARE_METATYPE(AccessTypes);
Q_DECLARE_METATYPE(NWColor);
Q_DECLARE_METATYPE(std::string);
Q_DECLARE_METATYPE(GUICapabilityDefinition);

#endif	/* GUICAPABILITYDEFINITION_H */

