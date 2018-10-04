/* 
 * File:   IInputCallbacks.h
 * Author: cleo
 *
 * Created on October 14, 2014, 3:49 PM
 */

#ifndef IINPUTCALLBACKS_H
#define	IINPUTCALLBACKS_H

class IInputCallbacks {
public:
    IInputCallbacks(){};
    virtual ~IInputCallbacks(){};

    virtual void onNewCapabilityRequested(long capabilityId) = 0;
    
    virtual void onInputValueChange(long id, bool value) = 0;
    virtual void onInputValueChange(long id, long value) = 0;
    
    virtual void onCapabilityRemoved(long id) = 0;
    
    virtual void onServiceRequested(long serviceId) = 0;
    
};


#endif	/* IINPUTCALLBACKS_H */

