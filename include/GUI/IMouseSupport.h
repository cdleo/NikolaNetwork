/* 
 * File:   IMouseSupport.h
 * Author: cleo
 *
 * Created on October 15, 2014, 10:39 AM
 */

#ifndef IMOUSESUPPORT_H
#define	IMOUSESUPPORT_H

#include <QMouseEvent>

class IMouseSupport {
public:
    IMouseSupport() : widgetMoved_(false) {};
    virtual ~IMouseSupport(){};
   
    virtual void mousePressEvent(QMouseEvent *ev) = 0;
    virtual void mouseReleaseEvent(QMouseEvent *ev) = 0;
    virtual void mouseMoveEvent(QMouseEvent *ev) = 0;     
    
protected:  
    QPoint                      dragStartOffset;

    bool                        widgetMoved_;  
};

#endif	/* IMOUSESUPPORT_H */

