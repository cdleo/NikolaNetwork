/* 
 * File:   NW_BaseDigitalIO.cpp
 * Author: cleo
 * 
 * Created on October 23, 2014, 12:36 PM
 */

#include <NW_BaseDigitalIO.h>
#include <QPixmap>
#include <QPalette>
//#include <qt5/QtCore/qitemselectionmodel.h>

NW_BaseDigitalIO::NW_BaseDigitalIO(GUICapabilityDefinition guiCapDef, QWidget* parent)
: QLabel(parent), INikolaWidget(guiCapDef), caption_(NULL), statusIndicator_(NULL) {
     initialize( );
}

NW_BaseDigitalIO::~NW_BaseDigitalIO(){
    if (caption_) {
        delete caption_;
        caption_ = NULL;
    }   
    
    if (statusIndicator_) {
        delete statusIndicator_;
        statusIndicator_ = NULL;
    }      
}

void NW_BaseDigitalIO::initialize( ){
    
    setObjectName( QString("NW%1").arg( getId() ) ); 
    setAlignment(Qt::AlignTop);
    
    drawStatus( DigitalCapabilityStatus_Off  );
    
    setFixedSize( pixmap()->width(), pixmap()->height() );
    
    statusIndicator_ = new QLabel(this);
    drawIndicator( DigitalCapabilityStatus_Unknown  );
    statusIndicator_->setFixedSize( pixmap()->height() - 2, pixmap()->height() - 2 );
    statusIndicator_->move(1,1);
    statusIndicator_->show();
    statusIndicator_->raise();
        
    if (!guiCapDef_.getCaption().isEmpty()) {        
        caption_ = new QLabel(guiCapDef_.getCaption(), this);
        QPalette palette = caption_->palette();
        palette.setColor(caption_->foregroundRole(), guiCapDef_.getCaptionColor());
        caption_->setPalette(palette);  
        caption_->setWordWrap(false);
        caption_->setAlignment(Qt::AlignCenter);
        caption_->setFixedSize( pixmap()->width() - statusIndicator_->width() - 2, pixmap()->height() );
        caption_->move(statusIndicator_->width(),-1);
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

bool NW_BaseDigitalIO::subscribe(QWidget* container){
    
    //Send    
    connect( this, SIGNAL(onValueChanged(long, bool)), container, SLOT(onValueChanged(long, bool)));
    
    //Receive
    connect(container, SIGNAL(update(long, bool)), this, SLOT(update(long, bool)));          
    return true;    
}

void NW_BaseDigitalIO::mousePressEvent ( QMouseEvent* ev ) {
    
    if  ( ev->button() == Qt::LeftButton ) {
        
        dragStartOffset = ev->pos();
        
        drawStatus( DigitalCapabilityStatus_On );

        /*int retries = 0;
        while (actionPerformed_ && (retries < 20)) { //Significa que aún no recibí respuesta del gateway 
            QThread::msleep( 50 );                   //o campabilityModule. Esperamos (como máximo) 1s
            retries++;
        }            

        emit onValueChanged( getId(), true );

        actionPerformed_ = true;*/

    }
}

void NW_BaseDigitalIO::mouseReleaseEvent ( QMouseEvent* ev ) {
    
    if  ( ev->button() == Qt::LeftButton ) {
        
        drawStatus( DigitalCapabilityStatus_Off );

        if (widgetMoved_) {
            widgetMoved_ = false;
            return;
        }
        
        int retries = 0;
        while (actionPerformed_ && (retries < 20)) { //Significa que aún no recibí respuesta del gateway 
            Sleeper::msleep( 50 );                   //o capabilityModule. Esperamos (como máximo) 1s
            retries++;
        }

        emit onValueChanged( getId(), (status_ != DigitalCapabilityStatus_On) );   
        
        actionPerformed_ = true;
    }    
}

void NW_BaseDigitalIO::mouseMoveEvent ( QMouseEvent* ev ){
    if (ev->buttons() & Qt::LeftButton) {
        this->move(mapToParent(ev->pos() - dragStartOffset));
        widgetMoved_ = true;
    }
}

void NW_BaseDigitalIO::update( long id, bool value ){
    
    if (id != getId())
        return;

    if (actionPerformed_){
        actionPerformed_ = false;
    }    

    if (value)
        drawIndicator( DigitalCapabilityStatus_On );
    else 
        drawIndicator( DigitalCapabilityStatus_Off );
}

void NW_BaseDigitalIO::drawStatus( DigitalCapabilityStatus newStatus ) {
 
    QPixmap* img = (QPixmap*) pixmap();
    bool newImg = (img == NULL);
    
    if (newImg)
        img = new QPixmap( );

    img->load( getImgFileNameByStatus(newStatus) );  
    
    if (newImg)
        setPixmap(*img);
    else
        repaint();    
}

void NW_BaseDigitalIO::drawIndicator( DigitalCapabilityStatus newStatus ) {
 
    QPixmap* img = (QPixmap*) statusIndicator_->pixmap();
    bool newImg = (img == NULL);
    
    if (newImg) {
        img = new QPixmap( );
    } else {
        if (newStatus == status_) return;
    }
    
    status_ = newStatus;
    
 
    
    statusIndicator_->setPixmap(img->scaled(height()-2,height()-2,Qt::KeepAspectRatio));
    
    /*if (newImg)
        statusIndicator_->setPixmap(img->scaled(height()-2,height()-2,Qt::KeepAspectRatio));
    else
        statusIndicator_->repaint();    */
}

QString NW_BaseDigitalIO::getImgFileNameByStatus( DigitalCapabilityStatus status )
{
    if (status == DigitalCapabilityStatus_On) {
        return QString("%1/%2_%3_on.png").arg( guiCapDef_.getResourcesPath().c_str() ).arg( guiCapDef_.getImgIndicatorPrefix().c_str() ).arg( getColorName( guiCapDef_.getIndicatorColor() ) );
    } else if (status == DigitalCapabilityStatus_Off) {
        return QString("%1/%2_%3_off.png").arg( guiCapDef_.getResourcesPath().c_str() ).arg( guiCapDef_.getImgIndicatorPrefix().c_str() ).arg( getColorName( guiCapDef_.getIndicatorColor() ) );
    } else {
        return QString("%1/%2_unknown.png").arg( guiCapDef_.getResourcesPath().c_str() ).arg( guiCapDef_.getImgIndicatorPrefix().c_str() );
    }           
}