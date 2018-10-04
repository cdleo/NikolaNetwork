/* 
 * File:   NW_BaseDigitalInput.h
 * Author: cleo
 *
 * Created on October 22, 2014, 12:36 PM
 */

#ifndef NW_BASEDIGITALINPUT_H
#define	NW_BASEDIGITALINPUT_H

#include <QLabel>
#include <GUI/INikolaWidget.h>
#include <GUI/IMouseSupport.h>

class NW_BaseDigitalInput : public QLabel, public INikolaWidget, public IMouseSupport
{
    Q_OBJECT
    
public:
    NW_BaseDigitalInput(GUICapabilityDefinition guiCapDef, QWidget* parent = 0 );
    virtual ~NW_BaseDigitalInput();

    virtual bool subscribe(QWidget* container);
    
signals:
    void onValueChanged(long, bool);        
    
public slots:
    void update( long id, bool value );

protected:       
    virtual void initialize( );
    
    virtual void drawStatus( DigitalCapabilityStatus newStatus );
    
    virtual void mousePressEvent ( QMouseEvent* ev );
    virtual void mouseReleaseEvent ( QMouseEvent* ev );
    virtual void mouseMoveEvent ( QMouseEvent* ev );    
    
    QString getImgFileNameByStatus( DigitalCapabilityStatus status );
    
    QLabel*                     caption_;

    DigitalCapabilityStatus     status_;
};

#endif	/* NW_BASEDIGITALINPUT_H */

