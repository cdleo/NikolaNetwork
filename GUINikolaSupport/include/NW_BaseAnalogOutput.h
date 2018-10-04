/* 
 * File:   NW_BaseAnalogOutput.h
 * Author: cleo
 *
 * Created on November 11, 2014, 11:00 AM
 */

#ifndef NW_BASEANALOGOUTPUT_H
#define	NW_BASEANALOGOUTPUT_H

#include <QLCDNumber>
#include <GUI/INikolaWidget.h>
#include <GUI/IMouseSupport.h>

class NW_BaseAnalogOutput : public QLCDNumber, public INikolaWidget, public IMouseSupport
{
    Q_OBJECT
    
public:
    NW_BaseAnalogOutput(GUICapabilityDefinition guiCapDef, QWidget* parent = 0 );
    NW_BaseAnalogOutput(GUICapabilityDefinition guiCapDef, Qt::Orientation orientation, QWidget* parent = 0 );
    virtual ~NW_BaseAnalogOutput();

    virtual bool subscribe(QWidget* container);   
    
protected:
    void mousePressEvent ( QMouseEvent* ev );
    void mouseMoveEvent ( QMouseEvent* ev );
    void mouseReleaseEvent(QMouseEvent *ev) { QLCDNumber::mouseReleaseEvent(ev); }
    
public slots:    
    void update( long id, long value );        
    
private:

};

#endif	/* NW_BASEANALOGOUTPUT_H */

