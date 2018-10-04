/* 
 * File:   NW_DigitalInputToggle.h
 * Author: cleo
 *
 * Created on October 22, 2014, 12:36 PM
 */

#ifndef NW_DIGITALINPUTTOGGLE_H
#define	NW_DIGITALINPUTTOGGLE_H

#include <NW_BaseDigitalInput.h>

class NW_DigitalInputToggle : public NW_BaseDigitalInput
{
    Q_OBJECT
    
public:
    NW_DigitalInputToggle(GUICapabilityDefinition guiCapDef, QWidget* parent = 0 );
    virtual ~NW_DigitalInputToggle();
        
signals:
    void onValueChanged(long, bool);    
    
protected:
    virtual void mousePressEvent ( QMouseEvent* ev );
    virtual void mouseReleaseEvent ( QMouseEvent* ev );

    bool                        toggle_;
};

#endif	/* NW_DIGITALINPUTTOGGLE_H */

