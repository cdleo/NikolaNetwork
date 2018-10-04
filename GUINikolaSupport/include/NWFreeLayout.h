/* 
 * File:   NWFreeLayout.h
 * Author: christian
 *
 * Created on April 23, 2016, 3:59 PM
 */

#ifndef NWFREELAYOUT_H
#define	NWFREELAYOUT_H

#include <QLayout>
#include <QtGui>
#include <QList>

class NWFreeLayout : public QLayout
{
public:
    NWFreeLayout(): QLayout() {}
    NWFreeLayout(QWidget *parent): QLayout(parent) {}
    NWFreeLayout(QLayoutPrivate& d, QLayout *layout, QWidget *widget): QLayout(d, layout, widget) {}
    ~NWFreeLayout();

    void addItem(QLayoutItem *item);
    QSize sizeHint() const;
    QSize minimumSize() const;
    int   count() const;
    QLayoutItem *itemAt(int) const;
    QLayoutItem *takeAt(int);
    void setGeometry(const QRect &rect);

private:
    QList<QLayoutItem*> list;
};

#endif	/* FREELAYOUT_H */

