/* 
 * File:   WidgetsContainer.cpp
 * Author: cleo
 * 
 * Created on October 14, 2014, 2:13 PM
 */

#include <GUI/WidgetsContainer.h>
#include <NikolaWidgets.h>
#include <NWFreeLayout.h>

#include <QtGui>
#include <QDrag>
#include <QByteArray>
#include <QDataStream>
#include <QMimeData>
#include <QDebug>
#include <QString>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

#include <sstream>

WidgetsContainer::WidgetsContainer( QWidget *parent, IInputCallbacks* callbacks  )
: QGroupBox(parent), callbacks_(callbacks) {
  
    layout_ = new NWFreeLayout();
    setAcceptDrops(true);
 
    setObjectName("WidgetsContainer");
    setStyleSheet("#WidgetsContainer {background-color:red;}");    

    this->setLayout(layout_);
    
    connect(this, SIGNAL(sendWidget(GUICapabilityDefinition)), this, SLOT(getWigdet(GUICapabilityDefinition)));
}

bool WidgetsContainer::addWidget( GUICapabilityDefinition guiDef ) {
    emit sendWidget( guiDef );
    return true;
}

bool WidgetsContainer::updateWidget( long id, bool value ) {
    emit update( id, value );
    return true;
}

bool WidgetsContainer::updateWidget( long id, long value ) {
    emit update( id, value );
    return true;
}

void WidgetsContainer::removeWidget( long capId ) {
 
    for (int i = 0; i < layout_->count(); i++) {
        QWidget* widget = layout_->itemAt(i)->widget();
        if (INikolaWidget* myWidget = dynamic_cast <INikolaWidget*>(widget)) {
            if (myWidget->getId() == capId) {
                callbacks_->onCapabilityRemoved( capId );
                layout_->removeWidget(widget);
                return;
            }
        }    
    }
}    

void WidgetsContainer::clear() {
 
    while (layout_->count() > 0) {
        QWidget* widget = layout_->itemAt(0)->widget();
        if (INikolaWidget* myWidget = dynamic_cast <INikolaWidget*>(widget)) {
            callbacks_->onCapabilityRemoved( myWidget->getId() );
            layout_->removeWidget(widget);
            delete widget;
        }      
    }
}    

void WidgetsContainer::getWigdet( const GUICapabilityDefinition guiDef ) {
        
    INikolaWidget* newWidget;
    switch( guiDef.getWidgetType() ) {
        case WidgetType_PushButton:
        {
            newWidget = new NW_BaseDigitalInput( guiDef, this );
            break;
        }
        
        case WidgetType_ToggleButton:
        {
            newWidget = new NW_DigitalInputToggle( guiDef, this );
            break;
        }
        
        case WidgetType_IndicatorButton:
        {
            newWidget = new NW_BaseDigitalIO( guiDef, this );
            break;
        }        
        
        case WidgetType_LedIndicator:
        {
            newWidget = new NW_BaseDigitalOutput( guiDef, this );
            break;
        }
        
        case WidgetType_Slider:
        {
            newWidget = new NW_BaseAnalogInput( guiDef, this );
            break;
        }          
        
        case WidgetType_AnalogValue:
        {
            newWidget = new NW_BaseAnalogOutput( guiDef, this );
            break;
        }           
        
        default:
            newWidget = NULL;
    }    
    
    if (newWidget) {
        layout_->addWidget( dynamic_cast<QWidget*>(newWidget) );
        newWidget->subscribe(this);
    }
}

void WidgetsContainer::onValueChanged(long buttonId, long value){
    if (callbacks_){
        if (buttonId == 100) {
            callbacks_->onCapabilityRemoved(value);
        } else {
            callbacks_->onInputValueChange(buttonId, value);
        }
    }    
}

void WidgetsContainer::onValueChanged(long buttonId, bool value){
    if (callbacks_){
        callbacks_->onInputValueChange(buttonId, value);
    }    
}

void WidgetsContainer::dropEvent(QDropEvent *de){
    // Unpack dropped data and handle it the way you want

    QMimeData* qMimeData = const_cast<QMimeData*>(de->mimeData());
 
    if (callbacks_) {

        //QByteArray encoded = qMimeData->data("application/x-qabstractitemmodeldatalist");
        QByteArray encoded = qMimeData->data("application/capabilities.text.list");

        QDataStream stream(&encoded, QIODevice::ReadOnly);

        while (!stream.atEnd())
        {
            QString droppedId;
            stream >> droppedId;        

            bool ok;
            long capID = droppedId.toLong(&ok);

            qDebug("Content: %s - %ld", droppedId.toLatin1().data(),capID);
            
            if (ok) {
                callbacks_->onNewCapabilityRequested( capID );
            }
        }
    }
    
    //delete qMimeData;
}
 
void WidgetsContainer::dragMoveEvent(QDragMoveEvent *de){
    // The event needs to be accepted here
    de->accept();
    
}
 
void WidgetsContainer::dragEnterEvent(QDragEnterEvent *event){
    // Set the drop action to be the proposed action.
    event->acceptProposedAction();    
}

WidgetsList WidgetsContainer::getWidgetsList(){
 
    WidgetsList widgets;
            
    for (int i = 0; i < layout_->count(); i++) {
        if (INikolaWidget* myWidget = dynamic_cast <INikolaWidget*>(layout_->itemAt(i)->widget())) {
            widgets.push_back( myWidget->getWidgetDefinition() );
        }    
    }
    
    return widgets;
}                             

bool WidgetsContainer::capabilityAlreadyInLayout(long capID) {
    
    for (int i = 0; i < layout_->count(); i++) {
        if (INikolaWidget* myWidget = dynamic_cast <INikolaWidget*>(layout_->itemAt(i)->widget())) {
            if (myWidget->getId() == capID)
                return true;
        }
    }
    return false;
}

void WidgetsContainer::mousePressEvent(QMouseEvent *event){
    
    INikolaWidget* child = dynamic_cast<INikolaWidget*>(childAt(event->pos()));
    
    QLabel* qchild = static_cast<QLabel*>(childAt(event->pos()));
    if (!child || !qchild)
        return;

    GUICapabilityDefinition guiDef = child->getWidgetDefinition();
    qDebug("Cap ID: %ld", guiDef.getId());
    QPixmap pixmap = *qchild->pixmap();

    QByteArray itemData;
    QDataStream dataStream(&itemData, QIODevice::WriteOnly);
    dataStream << pixmap << QPoint(event->pos() - qchild->pos());

    QMimeData *mimeData = new QMimeData;
    //mimeData->setData("application/x-dnditemdata", itemData);     
    mimeData->setText(qchild->objectName());     
 
    QDrag *drag = new QDrag(this);
    drag->setMimeData(mimeData);
    drag->setPixmap( pixmap );
    drag->setHotSpot(event->pos() - qchild->pos());
    
    QPixmap tempPixmap = pixmap;
    QPainter painter;
    painter.begin(&tempPixmap);
    painter.fillRect(pixmap.rect(), QColor(127, 127, 127, 127));
    painter.end();

    qchild->setPixmap(tempPixmap);

    if (drag->exec(Qt::CopyAction | Qt::MoveAction, Qt::CopyAction) == Qt::MoveAction)
        qchild->close();
    else {
        qchild->show();
        qchild->setPixmap(pixmap);
    }    
}

void WidgetsContainer::mouseReleaseEvent(QMouseEvent *event){
    
    INikolaWidget* child = dynamic_cast<INikolaWidget*>(childAt(event->pos()));
    
    QLabel* qchild = static_cast<QLabel*>(childAt(event->pos()));
    if (!child || !qchild)
        return;

    child->updatePosition(event->pos());
}