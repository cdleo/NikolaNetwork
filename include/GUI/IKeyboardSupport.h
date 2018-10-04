/* 
 * File:   IKeyboardSupport.h
 * Author: cleo
 *
 * Created on October 15, 2014, 10:39 AM
 */

#ifndef IKEYBOARDSUPPORT_H
#define	IKEYBOARDSUPPORT_H

#include <QKeyEvent>

class IKeyboardSupport {
public:
    IKeyboardSupport() : pressedKeys_(0){};
    virtual ~IKeyboardSupport(){};
    
    virtual void keyPressEvent ( QKeyEvent * event ) = 0;
    virtual void keyReleaseEvent ( QKeyEvent * event ) = 0;
    
    void onKeyPress ( QKeyEvent * event ){
        pressedKeys_ |= event->key();
    }

    void onKeyRelease ( QKeyEvent * event ){
        pressedKeys_ &= ~(event->key());
    }    
    
    bool isKeyPressed (enum Qt::Key key) {
        return (pressedKeys_ & key);
    }     
    
protected:  
    long long   pressedKeys_;
};

#endif	/* IKEYBOARDSUPPORT_H */

