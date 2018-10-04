/* 
 * File:   ControlsContainer.h
 * Author: cleo
 *
 * Created on October 14, 2014, 2:13 PM
 */

#ifndef CONTROLSCONTAINER_H
#define	CONTROLSCONTAINER_H

#include <GUI/IInputCallbacks.h>
#include <GUI/GUIWidgetDimension.h>
#include <GUI/GUICapabilityDefinition.h>

#include <QDrag>
#include <QMutex>
#include <QGroupBox>
#include <QGridLayout>
#include <QSignalMapper>

class ControlsContainer : public QGroupBox
{
    Q_OBJECT
    
public:
    ControlsContainer( QWidget *parent = 0, IInputCallbacks* callbacks = 0 );

    bool addWidget( GUICapabilityDefinition guiDef, GUIWidgetDimension guiDim );
    bool updateWidget( long id, bool value );
    bool updateWidget( long id, long value );
    //bool updateWidget( long id, long value );
  
signals:
    void sendWidget(const GUICapabilityDefinition guiDef, GUIWidgetDimension guiDim );
    void update( long id, bool value );
    void update( long id, long value );     
    
public slots:
    void getWigdet(const GUICapabilityDefinition guiDef, GUIWidgetDimension guiDim );
    void onValueChanged(long buttonId, long value);
    void onValueChanged(long buttonId, bool value);
  
protected:
    void dropEvent(QDropEvent *de);
    void dragMoveEvent(QDragMoveEvent *de);
    void dragEnterEvent(QDragEnterEvent *event);    
    void mousePressEvent(QMouseEvent *event);
    
private:
    QGridLayout*        layout_;
    IInputCallbacks*    callbacks_;
};

#endif	/* CONTROLSCONTAINER_H */

