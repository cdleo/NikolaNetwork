/* 
 * File:   NW_DigitalInputToggle.cpp
 * Author: cleo
 * 
 * Created on October 22, 2014, 12:36 PM
 */

#include <NW_DigitalInputToggle.h>
#include <QPixmap>
#include <QPalette>

NW_DigitalInputToggle::NW_DigitalInputToggle(GUICapabilityDefinition guiCapDef, QWidget* parent)
: NW_BaseDigitalInput(guiCapDef, parent), toggle_(false) {
     initialize( );
}

NW_DigitalInputToggle::~NW_DigitalInputToggle(){ 
}

void NW_DigitalInputToggle::mousePressEvent ( QMouseEvent* ev ) {
    
    if  (ev->button() == Qt::LeftButton) {
        
        dragStartOffset = ev->pos();
        
        if (status_ != DigitalCapabilityStatus_On) {
        
            drawStatus( DigitalCapabilityStatus_On );
            
            int retries = 0;
            while (actionPerformed_ && (retries < 20)) { //Significa que aún no recibí respuesta del gateway 
                Sleeper::msleep( 50 );                   //o campabilityModule. Esperamos (como máximo) 1s
                retries++;
            }            
            
            emit onValueChanged( getId(), true );

            actionPerformed_ = true;

        } else {
            toggle_ = true;
        }
    }
}

void NW_DigitalInputToggle::mouseReleaseEvent ( QMouseEvent* ev ) {
    
    if (ev->button() == Qt::LeftButton) {
        
        if ( (status_ != DigitalCapabilityStatus_Off) && toggle_ )  {
                 
            drawStatus( DigitalCapabilityStatus_Off );

            int retries = 0;
            while (actionPerformed_ && (retries < 20)) { //Significa que aún no recibí respuesta del gateway 
                Sleeper::msleep( 50 );                   //o campabilityModule. Esperamos (como máximo) 1s
                retries++;
            }

            emit onValueChanged( getId(), false );   

            actionPerformed_ = true;
            
            toggle_ = false;
        }
    }    
}



