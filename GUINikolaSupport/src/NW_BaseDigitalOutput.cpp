/* 
 * File:   NW_BaseDigitalOutput.cpp
 * Author: cleo
 * 
 * Created on October 22, 2014, 12:36 PM
 */

#include <NW_BaseDigitalOutput.h>
#include <QPixmap>
#include <QPalette>

NW_BaseDigitalOutput::NW_BaseDigitalOutput(GUICapabilityDefinition guiCapDef, QWidget* parent)
: QLabel(parent), INikolaWidget(guiCapDef), caption_(NULL) {
     initialize();
}

NW_BaseDigitalOutput::~NW_BaseDigitalOutput(){
    if (caption_) {
        delete caption_;
        caption_ = NULL;
    }   
}

void NW_BaseDigitalOutput::initialize( ){
    
    setObjectName( QString("NW%1").arg( getId() ) ); 
    setAlignment(Qt::AlignTop);
    
    drawStatus( DigitalCapabilityStatus_Unknown  );
    
    setFixedSize( pixmap()->width(), pixmap()->height() );
    
    if (!guiCapDef_.getCaption().isEmpty()) {
        caption_ = new QLabel(guiCapDef_.getCaption(), this);
        QPalette palette = caption_->palette();
        palette.setColor(caption_->foregroundRole(), guiCapDef_.getCaptionColor());
        caption_->setPalette(palette);  
        caption_->setWordWrap(true);
        caption_->setAlignment(Qt::AlignCenter);
        caption_->setFixedSize( pixmap()->width(), fontMetrics().height() );
        caption_->move( 0, pixmap()->height() ); 
        caption_->show();
        caption_->raise();
        
        setFixedSize( pixmap()->width(), pixmap()->height() + caption_->height() );
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

bool NW_BaseDigitalOutput::subscribe(QWidget* container){
    
    //Receive
    connect(container, SIGNAL(update(long, bool)), this, SLOT(update(long, bool)));          
    return true;    
}

void NW_BaseDigitalOutput::mousePressEvent ( QMouseEvent* ev ) {
    if  (ev->button() == Qt::LeftButton) {
        dragStartOffset = ev->pos();
    }
}

void NW_BaseDigitalOutput::mouseMoveEvent ( QMouseEvent* ev ){
    if (ev->buttons() & Qt::LeftButton) {
        this->move(mapToParent(ev->pos() - dragStartOffset));
    }
}

void NW_BaseDigitalOutput::update( long id, bool value ){
    
    if (id != getId())
        return;

    if (value)
        drawStatus( DigitalCapabilityStatus_On );
    else 
        drawStatus( DigitalCapabilityStatus_Off );
}

void NW_BaseDigitalOutput::drawStatus( DigitalCapabilityStatus newStatus ) {
 
    QPixmap* img = (QPixmap*) pixmap();
    bool newImg = (img == NULL);
    
    if (newImg) {
        img = new QPixmap( );
    } else {
        if (newStatus == status_) return;
    }
    
    status_ = newStatus;
    
    img->load( getImgFileNameByStatus(status_) );
    
    if (newImg)
        setPixmap(*img);
    else
        repaint();    
}

QString NW_BaseDigitalOutput::getImgFileNameByStatus( DigitalCapabilityStatus status )
{
    if (status == DigitalCapabilityStatus_On) {
        return QString("%1/%2_%3_on.png").arg( guiCapDef_.getResourcesPath().c_str() ).arg( guiCapDef_.getImgIndicatorPrefix().c_str() ).arg( getColorName( guiCapDef_.getIndicatorColor() ) );
    } else if (status == DigitalCapabilityStatus_Off) {
        return QString("%1/%2_%3_off.png").arg( guiCapDef_.getResourcesPath().c_str() ).arg( guiCapDef_.getImgIndicatorPrefix().c_str() ).arg( getColorName( guiCapDef_.getIndicatorColor() ) );
    } else {
        return QString("%1/%2_unknown.png").arg( guiCapDef_.getResourcesPath().c_str() ).arg( guiCapDef_.getImgIndicatorPrefix().c_str() );
    }           
}


