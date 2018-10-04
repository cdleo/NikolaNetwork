/* 
 * File:   NW_BaseAnalogInput.h
 * Author: cleo
 *
 * Created on October 15, 2014, 3:14 PM
 */

#ifndef NW_BASEANALOGINPUT_H
#define	NW_BASEANALOGINPUT_H

#include <QSlider>
#include <GUI/INikolaWidget.h>
#include <GUI/IMouseSupport.h>
#include <GUI/IKeyboardSupport.h>

class NW_BaseAnalogInput : public QSlider, public INikolaWidget, public IKeyboardSupport, public IMouseSupport
{
    Q_OBJECT
    
public:
    NW_BaseAnalogInput(GUICapabilityDefinition guiCapDef, QWidget* parent = 0 );
    NW_BaseAnalogInput(GUICapabilityDefinition guiCapDef, Qt::Orientation orientation, QWidget* parent = 0 );
    virtual ~NW_BaseAnalogInput();

    virtual bool subscribe(QWidget* container);   

protected:
    void mousePressEvent(QMouseEvent *ev);
    void mouseReleaseEvent(QMouseEvent *ev);
    void mouseMoveEvent(QMouseEvent *ev);    
    
    void keyPressEvent ( QKeyEvent * event ) {
        onKeyPress( event );
    }
    void keyReleaseEvent ( QKeyEvent * event ) {
        onKeyRelease( event );
    }
    
signals:
    void onValueChanged(long, long);
    
public slots: 
    void update( long id, long newValue );        
    
/*private slots:
    void onValueChanged(int value );
*/
};

//Q_DECLARE_METATYPE(NW_BaseAnalogInput);

#endif	/* NW_BASEANALOGINPUT_H */

