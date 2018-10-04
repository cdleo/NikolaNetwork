/* 
 * File:   NW_CapabilitiesList.h
 * Author: cleo
 *
 * Created on November 11, 2014, 11:00 AM
 */

#ifndef NW_CAPABILITIESLIST_H
#define	NW_CAPABILITIESLIST_H

#include <NW_BaseList.h>

class NW_CapabilitiesList : public NW_BaseList
{
    Q_OBJECT
    
public:
    NW_CapabilitiesList(GUICapabilityDefinition guiCapDef, QWidget* parent = 0 );
    virtual ~NW_CapabilitiesList();

    virtual bool subscribe(QWidget* container);   
    
protected:
    virtual void mousePressEvent ( QMouseEvent* ev );
    virtual void mouseMoveEvent ( QMouseEvent* ev );
    virtual void mouseReleaseEvent(QMouseEvent *ev) { NW_BaseList::mouseReleaseEvent(ev); }
    
    virtual QMimeData* mimeData(const QList<QTreeWidgetItem*> items) const;
    
public slots:    
    void update( long id, long value );        
    
private:

};

#endif	/* NW_CAPABILITIESLIST_H */

