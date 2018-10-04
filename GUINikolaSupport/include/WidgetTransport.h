/* 
 * File:   WidgetTransport.h
 * Author: cleo
 *
 * Created on October 16, 2014, 3:01 PM
 */

#ifndef WIDGETTRANSPORT_H
#define	WIDGETTRANSPORT_H

#include <QMutex>
#include <QThread>
#include <GUI/GUICapabilityDefinition.h>


class WidgetTransport : public QThread
{
    Q_OBJECT

public:
    WidgetTransport(QObject *parent = 0):
    QThread(parent), type_(TransportType_Undefined) {};
    
    ~WidgetTransport() {
        wait();
    };

    void transportWidget(GUICapabilityDefinition& guiDef){
        mutex_.lock();
        guiDef_ = guiDef;
        type_ = TransportType_Widget;
        start();
    }
    
    void transportValue( long id, bool value ){
        mutex_.lock();
        id_ = id;
        digitalValue_ = value;
        type_ = TransportType_DigitalValue;
        start();
    }

    void transportValue( long id, long value ){
        mutex_.lock();
        id_ = id;
        analogValue_ = value;
        type_ = TransportType_AnalogValue;
        start();
    }    

signals:
    void sendWidget(const GUICapabilityDefinition& guiDef);
    void sendValue( long id, bool value );
    void sendValue( long id, long value ); 
    
protected:
    void run(){
        switch(type_){
            case TransportType_Widget:
                emit sendWidget(guiDef_);
                break;
            
            case TransportType_AnalogValue:
                emit sendValue(id_,analogValue_);
                break;
                
            case TransportType_DigitalValue:
                emit sendValue(id_,digitalValue_);
                break;                

            default:
                break;
        }
        
        type_ = TransportType_Undefined;
        mutex_.unlock();
    }

private:
    QMutex                      mutex_;
    
    TransportType               type_;
    
    GUICapabilityDefinition     guiDef_; 
    long                        id_;
    bool                        digitalValue_;
    long                        analogValue_;
};

#endif	/* WIDGETTRANSPORT_H */

