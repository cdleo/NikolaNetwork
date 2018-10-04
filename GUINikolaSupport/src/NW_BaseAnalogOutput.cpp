/* 
 * File:   NW_BaseAnalogOutput.cpp
 * Author: cleo
 * 
 * Created on October 15, 2014, 3:14 PM
 */

#include <NW_BaseAnalogOutput.h>

NW_BaseAnalogOutput::NW_BaseAnalogOutput(GUICapabilityDefinition guiCapDef, QWidget* parent)
: QLCDNumber(parent), INikolaWidget( guiCapDef ) {
    setObjectName( tr("%1").arg( getId() ) );
    display( (int) 0 );
    
    QPoint point = guiCapDef_.getPosition();
    if ( point.isNull() ) {
        point = mapFromGlobal(QCursor::pos());
        point.setX( point.x() - (width() / 2) );
        point.setY( point.y() - (height() / 2) );
        guiCapDef_.setPosition( point ); 
    }
    this->move(point); 
}

NW_BaseAnalogOutput::NW_BaseAnalogOutput(GUICapabilityDefinition guiCapDef, Qt::Orientation orientation, QWidget* parent)
: QLCDNumber(orientation, parent), INikolaWidget( guiCapDef ) {        
    setObjectName( tr("%1").arg( getId() ) );
    display( (int) 0 );
    
    QPoint point = guiCapDef_.getPosition();
    if ( point.isNull() ) {
        point = mapFromGlobal(QCursor::pos());
        point.setX( point.x() - (width() / 2) );
        point.setY( point.y() - (height() / 2) );
        guiCapDef_.setPosition( point ); 
    }
    this->move(point);   
}

NW_BaseAnalogOutput::~NW_BaseAnalogOutput(){
}

bool NW_BaseAnalogOutput::subscribe(QWidget* container){
    
    //Receive
    connect(container, SIGNAL(update(long,long)), this, SLOT(update(long,long)));          
    return true;
}

void NW_BaseAnalogOutput::mousePressEvent ( QMouseEvent* ev ) {
    
    if  ( ev->button() == Qt::LeftButton ) {
        dragStartOffset = ev->pos();
    }
}

void NW_BaseAnalogOutput::mouseMoveEvent ( QMouseEvent* ev ){
    if (ev->buttons() & Qt::LeftButton) {
        this->move(mapToParent(ev->pos() - dragStartOffset));
    }
}

void NW_BaseAnalogOutput::update( long id, long value ){
    if (id != getId())
        return;
    
    display( (int) value );
    return;
}
