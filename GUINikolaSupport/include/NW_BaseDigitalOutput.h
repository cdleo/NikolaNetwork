/* 
 * File:   NW_BaseDigitalOutput.h
 * Author: cleo
 *
 * Created on October 22, 2014, 1:00 PM
 */

#ifndef NW_BASEDIGITALOUTPUT_H
#define	NW_BASEDIGITALOUTPUT_H

#include <QLabel>
#include <GUI/INikolaWidget.h>
#include <GUI/IMouseSupport.h>

class NW_BaseDigitalOutput : public QLabel, public INikolaWidget, public IMouseSupport
{
    Q_OBJECT
    
public:
    NW_BaseDigitalOutput(GUICapabilityDefinition guiCapDef, QWidget* parent = 0 );
    virtual ~NW_BaseDigitalOutput();

    virtual bool subscribe(QWidget* container);
    
protected:
    virtual void mousePressEvent ( QMouseEvent* ev );
    virtual void mouseMoveEvent ( QMouseEvent* ev );
    virtual void mouseReleaseEvent(QMouseEvent *ev) { QLabel::mouseReleaseEvent(ev); }
    virtual QString getImgFileNameByStatus( DigitalCapabilityStatus status );
    
public slots:
    void update( long id, bool value );
    
private:
    void initialize( );
    
    void drawStatus( DigitalCapabilityStatus newStatus );
    
    QLabel*                     caption_;

    DigitalCapabilityStatus     status_;
};

#endif	/* NW_BASEDIGITALOUTPUT_H */

