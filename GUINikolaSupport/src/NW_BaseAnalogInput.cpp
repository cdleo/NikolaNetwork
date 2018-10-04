/* 
 * File:   NW_BaseAnalogInput.cpp
 * Author: cleo
 * 
 * Created on October 15, 2014, 3:14 PM
 */

#include <NW_BaseAnalogInput.h>

NW_BaseAnalogInput::NW_BaseAnalogInput(GUICapabilityDefinition guiCapDef, QWidget* parent)
: NW_BaseAnalogInput(guiCapDef,Qt::Horizontal, parent) { }

NW_BaseAnalogInput::NW_BaseAnalogInput(GUICapabilityDefinition guiCapDef, Qt::Orientation orientation, QWidget* parent)
: QSlider(orientation, parent), INikolaWidget( guiCapDef ) {        
    setObjectName( tr("%1").arg( getId() ) );
    
    if (orientation == Qt::Horizontal)
        setFixedWidth( width() + 100 );
    else
        setFixedHeight( height() + 100 );
    
    QPoint point = guiCapDef_.getPosition();
    if ( point.isNull() ) {
        point = mapFromGlobal(QCursor::pos());
        point.setX( point.x() - (width() / 2) );
        point.setY( point.y() - (height() / 2) );
        guiCapDef_.setPosition( point ); 
    }
    this->move(point);     
}

NW_BaseAnalogInput::~NW_BaseAnalogInput(){
}

bool NW_BaseAnalogInput::subscribe(QWidget* container){
    
    //Send    
    connect( this, SIGNAL(onValueChanged(long, long)), container, SLOT(onValueChanged(long, long)));
    
    //Receive
    //connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));   
    connect(container, SIGNAL(update(long,long)), this, SLOT(update(long,long)));   
    return true;
}


void NW_BaseAnalogInput::mousePressEvent ( QMouseEvent* ev ) {
    
    if ((ev->buttons() & Qt::LeftButton) && isKeyPressed( Qt::Key_Shift )) {
        
        dragStartOffset = ev->pos();

    } else {
        QSlider::mousePressEvent ( ev );
    }
}

void NW_BaseAnalogInput::mouseReleaseEvent ( QMouseEvent* ev ) {
    
    if ((ev->buttons() & Qt::LeftButton) && isKeyPressed( Qt::Key_Shift ) ) {
        
        if (widgetMoved_) {
            widgetMoved_ = false;
            return;
        }
                
    } else {
        
        int retries = 0;
        while (actionPerformed_ && (retries < 20)) { //Significa que aún no recibí respuesta del gateway 
            Sleeper::msleep( 50 );                   //o campabilityModule. Esperamos (como máximo) 1s
            retries++;
        }

        emit onValueChanged( getId(), (long) value() );
        
        actionPerformed_ = true;        
        
        QSlider::mouseReleaseEvent ( ev );
    }
}

void NW_BaseAnalogInput::mouseMoveEvent ( QMouseEvent* ev ){
    if ((ev->buttons() & Qt::LeftButton) && isKeyPressed( Qt::Key_Shift ) ) {
        this->move(mapToParent(ev->pos() - dragStartOffset));
        widgetMoved_ = true;
    } else {
        QSlider::mouseMoveEvent ( ev );
    }
}

/*void NW_BaseAnalogInput::keyPressEvent ( QKeyEvent * event ){

    if (event->key() & Qt::Key_Shift){
        shiftPressed_ = true;
    }
    
}

void NW_BaseAnalogInput::keyReleaseEvent ( QKeyEvent * event ){
    if (event->key() & Qt::Key_Shift){
        shiftPressed_ = false;
    }
}*/

void NW_BaseAnalogInput::update( long id, long newValue ){
    if (id != getId())
        return;
    
    if (newValue == value())
        return;
    
    setValue( newValue );
    return;
}
