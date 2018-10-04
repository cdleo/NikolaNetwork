/* 
 * File:   NW_PushButton.cpp
 * Author: cleo
 * 
 * Created on October 22, 2014, 12:36 PM
 */

#include <NW_PushButton.h>

NW_PushButton::NW_PushButton(GUICapabilityDefinition guiCapDef, QWidget* parent)
: NW_BaseDigitalInput(guiCapDef, parent) {
     drawStatus( DigitalCapabilityStatus_Off );
}

NW_PushButton::~NW_PushButton(){
}

bool NW_PushButton::subscribe(QWidget* container){
    
    //Send    
    connect( this, SIGNAL(onValueChanged(long, bool)), container, SLOT(onValueChanged(long, bool)));      
    return true;    
}

void NW_PushButton::mousePressEvent ( QMouseEvent* ev ) {
    
    if  (ev->button() == Qt::LeftButton) {
        
        dragStartOffset = ev->pos();
        
        drawStatus( DigitalCapabilityStatus_On );

        emit onValueChanged( getId(), true );

    }
}

void NW_PushButton::mouseReleaseEvent ( QMouseEvent* ev ) {
    
    if (ev->button() == Qt::LeftButton) {
        
        drawStatus( DigitalCapabilityStatus_Off );

        emit onValueChanged( getId(), false );
        
    }    
}



