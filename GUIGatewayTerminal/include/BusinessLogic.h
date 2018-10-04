/* 
 * File:   BusinessLogic.h
 * Author: cleo
 *
 * Created on August 20, 2014, 10:01 AM
 */

#ifndef BUSINESSLOGIC_H
#define	BUSINESSLOGIC_H

#include <GUI/GlobalDefinitions.h>
#include <SingletonBase.h>

#define LOGIC BusinessLogic::getInstance()

class BusinessLogic : public SingletonBase<BusinessLogic> {
public:
    friend class SingletonBase<BusinessLogic>;

    void GetUserHash();
    
    void SubscribeAll();
    
    void UnsubscribeAll();
    
    void SetAnalogData();
    
    void SetDigitalData();
    
    void GetAnalogData();
    
    void GetDigitalData();    
    
private:
    BusinessLogic();
    virtual ~BusinessLogic() throw();    
    
};

#endif	/* BUSINESSLOGIC_H */

