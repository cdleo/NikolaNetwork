/* 
 * File:   NW_BaseList.h
 * Author: cleo
 *
 * Created on November 11, 2014, 11:00 AM
 */

#ifndef NW_BASELIST_H
#define	NW_BASELIST_H

#include <QList>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <GUI/INikolaWidget.h>
#include <GUI/IMouseSupport.h>
#include <GUI/IKeyboardSupport.h>

class NW_BaseList : public QTreeWidget, public INikolaWidget, public IMouseSupport, public IKeyboardSupport
{
    Q_OBJECT
    
public:
    NW_BaseList(GUICapabilityDefinition guiCapDef, QWidget* parent = 0 );
    virtual ~NW_BaseList();

    virtual bool subscribe(QWidget* container);   
    
protected:
    virtual void mousePressEvent ( QMouseEvent* ev );
    virtual void mouseMoveEvent ( QMouseEvent* ev );
    virtual void mouseReleaseEvent(QMouseEvent *ev) { QTreeWidget::mouseReleaseEvent(ev); }
    
    virtual void keyPressEvent ( QKeyEvent * event ) { onKeyPress(event); }
    virtual void keyReleaseEvent ( QKeyEvent * event ) { onKeyRelease(event); }
    
    virtual QTreeWidgetItem* addTreeRoot(QString name);
    virtual QTreeWidgetItem* addTreeChild(QTreeWidgetItem *parent, long id, QString name);
    virtual QTreeWidgetItem* getTreeItem(QString name);
    
public slots:    
    void update( long id, long value );        
    
private:

};

#endif	/* NW_BASELIST_H */

