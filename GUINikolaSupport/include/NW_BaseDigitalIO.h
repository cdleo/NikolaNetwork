/* 
 * File:   NW_BaseDigitalIO.h
 * Author: cleo
 *
 * Created on October 23, 2014, 9:37 PM
 */

#ifndef NW_BASEDIGITALIO_H
#define	NW_BASEDIGITALIO_H

#include <QLabel>
#include <GUI/INikolaWidget.h>
#include <GUI/IMouseSupport.h>

class NW_BaseDigitalIO : public QLabel, public INikolaWidget, public IMouseSupport
{
    Q_OBJECT
    
public:
    NW_BaseDigitalIO(GUICapabilityDefinition guiCapDef, QWidget* parent = 0 );
    virtual ~NW_BaseDigitalIO();

    virtual bool subscribe(QWidget* container);
    
protected:
    virtual void mousePressEvent ( QMouseEvent* ev );
    virtual void mouseReleaseEvent ( QMouseEvent* ev );
    virtual void mouseMoveEvent ( QMouseEvent* ev );
    virtual QString getImgFileNameByStatus( DigitalCapabilityStatus status );
    
signals:
    void onValueChanged(long, bool);    
    
public slots:
    void update( long id, bool value );
    
private:
    void initialize( );
    
    void drawStatus( DigitalCapabilityStatus newStatus );
    
    void drawIndicator( DigitalCapabilityStatus newStatus );
    
    QLabel*                     caption_;
    
    QLabel*                     statusIndicator_;
    
    DigitalCapabilityStatus     status_;
};

#endif	/* NW_BASEDIGITALIO_H */

