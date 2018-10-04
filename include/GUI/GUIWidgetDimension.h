/* 
 * File:   GUIWidgetDimension.h
 * Author: cleo
 *
 * Created on October 16, 2014, 4:17 PM
 */

#ifndef GUIWIDGETDIMENSION_H
#define	GUIWIDGETDIMENSION_H

#include <QMetaType>

class GUIWidgetDimension {
    
public:
    GUIWidgetDimension()
    {
        GUIWidgetDimension(0, 0, 0, 0);
    }
    
    GUIWidgetDimension(int iRowIndex, int iColumnIndex)
    {
        GUIWidgetDimension(iRowIndex, iColumnIndex, 1, 1);
    };
    
    GUIWidgetDimension(int iRowIndex, int iColumnIndex, int iRowSpan, int iColumnSpan)
    {
        m_iRowIndex = iRowIndex;
        m_iColumnIndex = iColumnIndex;
        m_iRowSpan = iRowSpan;
        m_iColumnSpan = iColumnSpan;        
    };
    
    GUIWidgetDimension(const GUIWidgetDimension& other) {
        m_iRowIndex = other.m_iRowIndex;
        m_iColumnIndex = other.m_iColumnIndex;
        m_iRowSpan = other.m_iRowSpan;
        m_iColumnSpan = other.m_iColumnSpan;
    }
    
    virtual ~GUIWidgetDimension() {};
       
    void setRowIndex(int rowIndex) {
        this->m_iRowIndex = rowIndex;
    }

    int getRowIndex() const {
        return m_iRowIndex;
    }       
 
    void setColumnIndex(int columnIndex) {
        this->m_iColumnIndex = columnIndex;
    }

    int getColumnIndex() const {
        return m_iColumnIndex;
    }      
    
    void setRowSpan(int rowSpan) {
        this->m_iRowSpan = rowSpan;
    }

    int getRowSpan() const {
        return m_iRowSpan;
    }       
 
    void setColumnSpan(int columnSpan) {
        this->m_iColumnSpan = columnSpan;
    }

    int getColumnSpan() const {
        return m_iColumnSpan;
    }        
    
private:
    int     m_iRowIndex;
    int     m_iColumnIndex;
    int     m_iRowSpan;
    int     m_iColumnSpan;
};   

Q_DECLARE_METATYPE(GUIWidgetDimension);

#endif	/* GUIWIDGETDIMENSION_H */

