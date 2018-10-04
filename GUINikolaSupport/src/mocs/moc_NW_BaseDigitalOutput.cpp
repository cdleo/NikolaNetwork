/****************************************************************************
** Meta object code from reading C++ file 'NW_BaseDigitalOutput.h'
**
** Created by: The Qt Meta Object Compiler version 63 (Qt 4.8.6)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../include/NW_BaseDigitalOutput.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'NW_BaseDigitalOutput.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 63
#error "This file was generated using the moc from 4.8.6. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_NW_BaseDigitalOutput[] = {

 // content:
       6,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      31,   22,   21,   21, 0x0a,

       0        // eod
};

static const char qt_meta_stringdata_NW_BaseDigitalOutput[] = {
    "NW_BaseDigitalOutput\0\0id,value\0"
    "update(long,bool)\0"
};

void NW_BaseDigitalOutput::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        Q_ASSERT(staticMetaObject.cast(_o));
        NW_BaseDigitalOutput *_t = static_cast<NW_BaseDigitalOutput *>(_o);
        switch (_id) {
        case 0: _t->update((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    }
}

const QMetaObjectExtraData NW_BaseDigitalOutput::staticMetaObjectExtraData = {
    0,  qt_static_metacall 
};

const QMetaObject NW_BaseDigitalOutput::staticMetaObject = {
    { &QLabel::staticMetaObject, qt_meta_stringdata_NW_BaseDigitalOutput,
      qt_meta_data_NW_BaseDigitalOutput, &staticMetaObjectExtraData }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &NW_BaseDigitalOutput::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *NW_BaseDigitalOutput::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *NW_BaseDigitalOutput::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_NW_BaseDigitalOutput))
        return static_cast<void*>(const_cast< NW_BaseDigitalOutput*>(this));
    if (!strcmp(_clname, "INikolaWidget"))
        return static_cast< INikolaWidget*>(const_cast< NW_BaseDigitalOutput*>(this));
    if (!strcmp(_clname, "IMouseSupport"))
        return static_cast< IMouseSupport*>(const_cast< NW_BaseDigitalOutput*>(this));
    return QLabel::qt_metacast(_clname);
}

int NW_BaseDigitalOutput::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QLabel::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 1)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 1;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
