/* 
 * File:   NW_BaseDigitalInput.cpp
 * Author: cleo
 * 
 * Created on October 22, 2014, 12:36 PM
 */

#include <NW_BaseDigitalInput.h>
#include <QPixmap>
#include <QPalette>
//#include <qt5/QtWidgets/qgraphicsitem.h>

NW_BaseDigitalInput::NW_BaseDigitalInput(GUICapabilityDefinition guiCapDef, QWidget* parent)
: QLabel(parent), INikolaWidget(guiCapDef), caption_(NULL) {
     initialize( );
}

NW_BaseDigitalInput::~NW_BaseDigitalInput(){
    if (caption_) {
        delete caption_;
        caption_ = NULL;
    }   
}

void NW_BaseDigitalInput::initialize( ){
    
    setObjectName( QString("NW%1").arg( getId() ) ); 
    setAlignment(Qt::AlignTop);
    
    drawStatus( DigitalCapabilityStatus_Unknown  );
    
    setFixedSize( pixmap()->width(), pixmap()->height() );
    
    if (!guiCapDef_.getCaption().isEmpty()) {
        caption_ = new QLabel(guiCapDef_.getCaption(), this);
        QPalette palette = caption_->palette();
        palette.setColor(caption_->foregroundRole(), guiCapDef_.getCaptionColor());
        caption_->setPalette(palette);  
        caption_->setWordWrap(false);
        caption_->setAlignment(Qt::AlignCenter);
        caption_->setFixedSize( pixmap()->width(), pixmap()->height() );
        caption_->move(0,-1);
        caption_->show();
        caption_->raise();
    }
    
    QPoint point = guiCapDef_.getPosition();
    if ( point.isNull() ) {
        point = mapFromGlobal(QCursor::pos());
        point.setX( point.x() - (pixmap()->width() / 2) );
        point.setY( point.y() - (pixmap()->height() / 2) );
        guiCapDef_.setPosition( point ); 
    }
    this->move(point); 
}

bool NW_BaseDigitalInput::subscribe(QWidget* container){
    
    //Send    
    connect( this, SIGNAL(onValueChanged(long, bool)), container, SLOT(onValueChanged(long, bool)));
    
    //Receive
    connect(container, SIGNAL(update(long, bool)), this, SLOT(update(long, bool)));          
    return true;    
}

void NW_BaseDigitalInput::mousePressEvent ( QMouseEvent* ev ) {
    
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

        }
    }
}

void NW_BaseDigitalInput::mouseReleaseEvent ( QMouseEvent* ev ) {
    
    if ( (ev->button() == Qt::LeftButton) && 
         (status_ != DigitalCapabilityStatus_Off) ) {
        
        drawStatus( DigitalCapabilityStatus_Off );

        int retries = 0;
        while (actionPerformed_ && (retries < 20)) { //Significa que aún no recibí respuesta del gateway 
            Sleeper::msleep( 50 );                   //o campabilityModule. Esperamos (como máximo) 1s
            retries++;
        }

        emit onValueChanged( getId(), false );   

        actionPerformed_ = true;
    }    
}

void NW_BaseDigitalInput::mouseMoveEvent ( QMouseEvent* ev ){
    if (ev->buttons() & Qt::LeftButton) {
        this->move(mapToParent(ev->pos() - dragStartOffset));
        guiCapDef_.setPosition( pos() );
    }
}

void NW_BaseDigitalInput::update( long id, bool value ){
    
    if (id != getId())
        return;

    if (actionPerformed_){
        actionPerformed_ = false;
        return;
    }

    if (value)
        drawStatus( DigitalCapabilityStatus_On );
    else 
        drawStatus( DigitalCapabilityStatus_Off );
}

void NW_BaseDigitalInput::drawStatus( DigitalCapabilityStatus newStatus ) {
 
    QPixmap* img = (QPixmap*) pixmap();
    bool newImg = (img == NULL);
    
    if (newImg) {
        img = new QPixmap( );
    } else {
        if (newStatus == status_) return;
    }
    
    status_ = newStatus;
    
    img->load( getImgFileNameByStatus( status_ ) );

    if (newImg)
        setPixmap(*img);
    else
        repaint();    
}

QString NW_BaseDigitalInput::getImgFileNameByStatus( DigitalCapabilityStatus status )
{
    if (status == DigitalCapabilityStatus_On) {
        return QString("%1/%2_%3_pressed.png").arg( guiCapDef_.getResourcesPath().c_str() ).arg( guiCapDef_.getImgAccionatorPrefix().c_str() ).arg( getColorName( guiCapDef_.getAccionatorColor() ) );
    } else if (status == DigitalCapabilityStatus_Off) {
        return QString("%1/%2_%3_released.png").arg( guiCapDef_.getResourcesPath().c_str() ).arg( guiCapDef_.getImgAccionatorPrefix().c_str() ).arg( getColorName( guiCapDef_.getAccionatorColor() ) );
    } else {
        return QString("%1/%2_unknown.png").arg( guiCapDef_.getResourcesPath().c_str() ).arg( guiCapDef_.getImgAccionatorPrefix().c_str() );
    }            
}

