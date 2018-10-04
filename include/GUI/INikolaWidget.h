/* 
 * File:   NikolaWidget.h
 * Author: cleo
 *
 * Created on October 15, 2014, 10:39 AM
 */

#ifndef INIKOLAWIDGET_H
#define	INIKOLAWIDGET_H

#include <QWidget>
#include <GUI/Sleeper.h>
#include <GUI/GUICapabilityDefinition.h>

class INikolaWidget {
public:
    INikolaWidget( GUICapabilityDefinition guiCapDef)
    : guiCapDef_(guiCapDef), actionPerformed_(false) {};
    virtual ~INikolaWidget(){};
   
    virtual long getId() const { return guiCapDef_.getId(); }
    
    virtual const GUICapabilityDefinition& getWidgetDefinition() const { return guiCapDef_; }
    
    virtual void updatePosition(QPoint newPos) { 
        guiCapDef_.setPosition( newPos ); 
    };
    
    virtual QString getImgFileNameByStatus( DigitalCapabilityStatus status )
    {
        throw new std::runtime_error("INikolaWidget::getImgFileNameByStatus - Not implemented!");
    };

    virtual const char* getColorName(NWColor color) const {
    
        switch(color){
            case NW_Blue:
                return "blue";

            case NW_Green:
                return "green";                

            case NW_Orange:
                return "orange";   
                
            case NW_Red:
                return "red";                                
                
            case NW_Violet:
                return "violet";                    
                
            case NW_Yellow:
                return "yellow";                
               
            case NW_Black:
            default:
                return "black";
        }
    }
    
    virtual bool subscribe(QWidget* container) = 0; 
    
protected:  
    GUICapabilityDefinition     guiCapDef_;
    
    bool                        actionPerformed_;
};

#endif	/* INIKOLAWIDGET_H */

