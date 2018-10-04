/* 
 * File:   ControlsContainer.cpp
 * Author: cleo
 * 
 * Created on October 14, 2014, 2:13 PM
 */

#include <GUI/ControlsContainer.h>
#include <NikolaWidgets.h>

#include <QtGui>
#include <QDrag>
#include <QByteArray>
#include <QDataStream>
#include <QMimeData>
#include <QDebug>
#include <QGroupBox>
#include <QLineEdit>
#include <QPushButton>

#include <sstream>

ControlsContainer::ControlsContainer( QWidget *parent, IInputCallbacks* callbacks  )
: QGroupBox(parent), callbacks_(callbacks) {
  
    layout_ = new QGridLayout();
    setAcceptDrops(true);

    setObjectName("ControlsContainer");
    setStyleSheet("#ControlsContainer {background-color:green;}");        
    
    this->setLayout(layout_);
    
    connect(this, SIGNAL(sendWidget(GUICapabilityDefinition, GUIWidgetDimension)), this, SLOT(getWigdet(GUICapabilityDefinition, GUIWidgetDimension)));
}

bool ControlsContainer::addWidget( GUICapabilityDefinition guiDef, GUIWidgetDimension guiDim ) {
    emit sendWidget( guiDef, guiDim );
    return true;
}

bool ControlsContainer::updateWidget( long id, bool value ) {
    emit update( id, value );
    return true;
}

bool ControlsContainer::updateWidget( long id, long value ) {
    emit update( id, value );
    return true;
}

void ControlsContainer::getWigdet( const GUICapabilityDefinition guiDef, const GUIWidgetDimension guiDim ) {
        
    INikolaWidget* newWidget;
    switch( guiDef.getWidgetType() ) {
        case WidgetType_CapabilitiesList:
        {
            newWidget = new NW_CapabilitiesList( guiDef );
            break;
        }
        
        case WidgetType_GenericList:
        {
            newWidget = new NW_BaseList( guiDef );
            break;
        }
        
        case WidgetType_PushButton:
        {
            newWidget = new NW_PushButton( guiDef );
            break;
        }
        
        case WidgetType_ToggleButton:
        {
            newWidget = new NW_DigitalInputToggle( guiDef );
            break;
        }
        
        case WidgetType_IndicatorButton:
        {
            newWidget = new NW_BaseDigitalIO( guiDef );
            break;
        }        
        
        case WidgetType_LedIndicator:
        {
            newWidget = new NW_BaseDigitalOutput( guiDef );
            break;
        }
        
        case WidgetType_Slider:
        {
            newWidget = new NW_BaseAnalogInput( guiDef );
            break;
        }          
        
        case WidgetType_AnalogValue:
        {
            newWidget = new NW_BaseAnalogOutput( guiDef );
            break;
        }           
        
        default:
            newWidget = NULL;
    }
    
    if (newWidget) {
        layout_->addWidget( dynamic_cast<QWidget*>(newWidget), guiDim.getRowIndex(), guiDim.getColumnIndex(), guiDim.getRowSpan(), guiDim.getColumnSpan() );
        newWidget->subscribe(this);
    }
}

void ControlsContainer::onValueChanged(long buttonId, long value){
    if (callbacks_ && value) {
        callbacks_->onServiceRequested(buttonId);
    }    
}

void ControlsContainer::onValueChanged(long buttonId, bool value){
    if (callbacks_ && value){
        callbacks_->onServiceRequested(buttonId);
    }    
}

void ControlsContainer::dropEvent(QDropEvent *de){
    // Unpack dropped data and handle it the way you want
    qDebug("Contents: %s", de->mimeData()->text().toLatin1().data());
    
    /*QLabel* qchild = findChild<QLabel*>( QString(de->mimeData()->text().toLatin1().data()));
    if (!qchild)
        return;  
    
    qchild->move( de->pos() );*/
    /*if (de->source() == this) {
        de->setDropAction(Qt::MoveAction);
        de->accept();
    } else {
        de->acceptProposedAction();
    }*/

    //this->move(mapToParent(de->pos() - offset));
    
    
}
 
void ControlsContainer::dragMoveEvent(QDragMoveEvent *de){
    // The event needs to be accepted here
    de->accept();
}
 
void ControlsContainer::dragEnterEvent(QDragEnterEvent *event){
    // Set the drop action to be the proposed action.
    event->acceptProposedAction();
}

void ControlsContainer::mousePressEvent(QMouseEvent *event){
    
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
