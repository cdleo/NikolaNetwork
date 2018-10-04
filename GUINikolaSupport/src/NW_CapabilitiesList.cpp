/* 
 * File:   NW_CapabilitiesList.cpp
 * Author: cleo
 * 
 * Created on October 15, 2014, 3:14 PM
 */

#include <NW_CapabilitiesList.h>

#include <support/CapabilitiesManager.h>

#include <QMimeData>

NW_CapabilitiesList::NW_CapabilitiesList(GUICapabilityDefinition guiCapDef, QWidget* parent)
: NW_BaseList(guiCapDef, parent) {
    setObjectName( tr("%1").arg( getId() ) );
    setColumnCount( 2 );
    setColumnHidden( 1, true);
    setHeaderHidden(true);
    //setAcceptDrops(true);
    setDragEnabled(true);
    setDragDropMode(QAbstractItemView::DragOnly);    
    //display( (int) 0 );
}

NW_CapabilitiesList::~NW_CapabilitiesList(){
}

bool NW_CapabilitiesList::subscribe(QWidget* container){
    
    //Receive
    connect(container, SIGNAL(update(long,long)), this, SLOT(update(long,long)));          
    return true;
}

void NW_CapabilitiesList::mousePressEvent ( QMouseEvent* ev ) {
    
    NW_BaseList::mousePressEvent(ev);
}

void NW_CapabilitiesList::mouseMoveEvent ( QMouseEvent* ev ){
    
    NW_BaseList::mouseMoveEvent(ev);
}

void NW_CapabilitiesList::update( long id, long value ){
    
    if (id != getId())
        return;
    
    CapabilityDefinition capability;
    Ident module;
    if (CAPABILITIES->getForeignCapabilityById( value, capability, module )){
        QTreeWidgetItem* root = getTreeItem( module.getModuleDescription().c_str() );
        if ( root == NULL ) {
            root = addTreeRoot( QString(module.getModuleDescription().c_str()) );
        }
        addTreeChild( root, capability.getId(), QString(capability.getDescription().c_str()) );
    }
    return;
}

QMimeData* NW_CapabilitiesList::mimeData(const QList<QTreeWidgetItem*> items) const {
     QMimeData *mimeData = new QMimeData();
     QByteArray encodedData;

     QDataStream stream(&encodedData, QIODevice::WriteOnly);

     foreach (QTreeWidgetItem* item, items) {
        //QString text = item->text();
        //stream << item->text(0) << " - " << item->text(1);
         stream << item->text(1);
     }

     mimeData->setData("application/capabilities.text.list", encodedData);
     return mimeData;
 }