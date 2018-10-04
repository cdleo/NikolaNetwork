/* 
 * File:   NW_BaseList.cpp
 * Author: cleo
 * 
 * Created on October 15, 2014, 3:14 PM
 */

#include <NW_BaseList.h>

NW_BaseList::NW_BaseList(GUICapabilityDefinition guiCapDef, QWidget* parent)
: QTreeWidget(parent), INikolaWidget( guiCapDef ) {
    setObjectName( tr("%1").arg( getId() ) );
    //display( (int) 0 );
}

NW_BaseList::~NW_BaseList(){
}

bool NW_BaseList::subscribe(QWidget* container){
    
    //Receive
    connect(container, SIGNAL(update(long,long)), this, SLOT(update(long,long)));
    return true;
}

void NW_BaseList::mousePressEvent ( QMouseEvent* ev ) {
    
    if ((ev->buttons() & Qt::LeftButton) && isKeyPressed( Qt::Key_Shift ) ) {
        dragStartOffset = ev->pos();
    }
    QTreeWidget::mousePressEvent ( ev );
}

void NW_BaseList::mouseMoveEvent ( QMouseEvent* ev ){
    if ((ev->buttons() & Qt::LeftButton) && isKeyPressed( Qt::Key_Shift ) ) {
        this->move(mapToParent(ev->pos() - dragStartOffset));
    }
    QTreeWidget::mouseMoveEvent ( ev );
}

void NW_BaseList::update( long id, long value ){
    if (id != getId())
        return;
    
    return;
}

QTreeWidgetItem* NW_BaseList::addTreeRoot(QString name) {
    QTreeWidgetItem* treeItem = new QTreeWidgetItem(this);
    treeItem->setText(0, name);
    return treeItem;
}

QTreeWidgetItem* NW_BaseList::addTreeChild(QTreeWidgetItem *parent, long id, QString name) {
    QTreeWidgetItem* treeItem = new QTreeWidgetItem();

    treeItem->setText(0, name);    
    treeItem->setText(1, tr("%1").arg(id));

    parent->addChild(treeItem);
    
    return treeItem;
}

QTreeWidgetItem* NW_BaseList::getTreeItem(QString name) {
    QList<QTreeWidgetItem*> items = findItems (name, Qt::MatchExactly, 0 );
    if (items.size() == 0)
        return NULL;
        
    return items.at(0);
}