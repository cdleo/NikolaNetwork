/* 
 * File:   WidgetsContainer.h
 * Author: cleo
 *
 * Created on October 14, 2014, 2:13 PM
 */

#ifndef WIDGETSCONTAINER_H
#define	WIDGETSCONTAINER_H

#include <GUI/IInputCallbacks.h>
#include <GUI/GUICapabilityDefinition.h>

#include <QDrag>
#include <QMutex>
#include <QGroupBox>
#include <QSignalMapper>

class WidgetsContainer : public QGroupBox
{
    Q_OBJECT
    
public:
    WidgetsContainer( QWidget *parent = 0, IInputCallbacks* callbacks = 0 );

    bool addWidget( GUICapabilityDefinition guiDef );
    bool updateWidget( long id, bool value );
    bool updateWidget( long id, long value );
    void clear();
    
    WidgetsList getWidgetsList();
    bool capabilityAlreadyInLayout(long capID);
    
signals:
    void sendWidget(const GUICapabilityDefinition guiDef);
    void update( long id, bool value );
    void update( long id, long value );     
    
public slots:
    void getWigdet(const GUICapabilityDefinition guiDef );
    void removeWidget( long capId );
    void onValueChanged(long buttonId, long value);
    void onValueChanged(long buttonId, bool value);
  
protected:
    void dropEvent(QDropEvent *de);
    void dragMoveEvent(QDragMoveEvent *de);
    void dragEnterEvent(QDragEnterEvent *event);    
    
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    
private:
    QLayout*            layout_;
    IInputCallbacks*    callbacks_;
};

#endif	/* WIDGETSCONTAINER_H */

