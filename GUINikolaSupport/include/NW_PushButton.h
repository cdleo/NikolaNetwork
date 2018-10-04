/* 
 * File:   NW_PushButton.h
 * Author: cleo
 *
 * Created on November 18, 2014, 3:09 PM
 */

#ifndef NW_PUSHBUTTON_H
#define	NW_PUSHBUTTON_H

#include <NW_BaseDigitalInput.h>

class NW_PushButton : public NW_BaseDigitalInput
{
    Q_OBJECT
    
public:
    NW_PushButton(GUICapabilityDefinition guiCapDef, QWidget* parent = 0 );
    virtual ~NW_PushButton();
    
    virtual bool subscribe(QWidget* container);
    
signals:
    void onValueChanged(long, bool);  

protected:       
    virtual void mousePressEvent ( QMouseEvent* ev );
    virtual void mouseReleaseEvent ( QMouseEvent* ev );

};

#endif	/* NW_PUSHBUTTON_H */

