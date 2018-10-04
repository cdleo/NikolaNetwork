/****************************************************************************
** Meta object code from reading C++ file 'GUIContainer.h'
**
** Created by: The Qt Meta Object Compiler version 67 (Qt 5.3.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../../../include/GUI/GUIContainer.h"
#include <QtCore/qbytearray.h>
#include <QtCore/qmetatype.h>
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'GUIContainer.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 67
#error "This file was generated using the moc from 5.3.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
struct qt_meta_stringdata_GUIContainer_t {
    QByteArrayData data[11];
    char stringdata[106];
};
#define QT_MOC_LITERAL(idx, ofs, len) \
    Q_STATIC_BYTE_ARRAY_DATA_HEADER_INITIALIZER_WITH_OFFSET(len, \
    qptrdiff(offsetof(qt_meta_stringdata_GUIContainer_t, stringdata) + ofs \
        - idx * sizeof(QByteArrayData)) \
    )
static const qt_meta_stringdata_GUIContainer_t qt_meta_stringdata_GUIContainer = {
    {
QT_MOC_LITERAL(0, 0, 12),
QT_MOC_LITERAL(1, 13, 10),
QT_MOC_LITERAL(2, 24, 0),
QT_MOC_LITERAL(3, 25, 23),
QT_MOC_LITERAL(4, 49, 6),
QT_MOC_LITERAL(5, 56, 6),
QT_MOC_LITERAL(6, 63, 2),
QT_MOC_LITERAL(7, 66, 5),
QT_MOC_LITERAL(8, 72, 9),
QT_MOC_LITERAL(9, 82, 14),
QT_MOC_LITERAL(10, 97, 8)
    },
    "GUIContainer\0sendWidget\0\0"
    "GUICapabilityDefinition\0guiDef\0update\0"
    "id\0value\0getWigdet\0onValueChanged\0"
    "buttonId"
};
#undef QT_MOC_LITERAL

static const uint qt_meta_data_GUIContainer[] = {

 // content:
       7,       // revision
       0,       // classname
       0,    0, // classinfo
       6,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       3,       // signalCount

 // signals: name, argc, parameters, tag, flags
       1,    1,   44,    2, 0x06 /* Public */,
       5,    2,   47,    2, 0x06 /* Public */,
       5,    2,   52,    2, 0x06 /* Public */,

 // slots: name, argc, parameters, tag, flags
       8,    1,   57,    2, 0x0a /* Public */,
       9,    2,   60,    2, 0x0a /* Public */,
       9,    2,   65,    2, 0x0a /* Public */,

 // signals: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Long, QMetaType::Bool,    6,    7,
    QMetaType::Void, QMetaType::Long, QMetaType::Long,    6,    7,

 // slots: parameters
    QMetaType::Void, 0x80000000 | 3,    4,
    QMetaType::Void, QMetaType::Long, QMetaType::Long,   10,    7,
    QMetaType::Void, QMetaType::Long, QMetaType::Bool,   10,    7,

       0        // eod
};

void GUIContainer::qt_static_metacall(QObject *_o, QMetaObject::Call _c, int _id, void **_a)
{
    if (_c == QMetaObject::InvokeMetaMethod) {
        GUIContainer *_t = static_cast<GUIContainer *>(_o);
        switch (_id) {
        case 0: _t->sendWidget((*reinterpret_cast< const GUICapabilityDefinition(*)>(_a[1]))); break;
        case 1: _t->update((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        case 2: _t->update((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2]))); break;
        case 3: _t->getWigdet((*reinterpret_cast< const GUICapabilityDefinition(*)>(_a[1]))); break;
        case 4: _t->onValueChanged((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< long(*)>(_a[2]))); break;
        case 5: _t->onValueChanged((*reinterpret_cast< long(*)>(_a[1])),(*reinterpret_cast< bool(*)>(_a[2]))); break;
        default: ;
        }
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        switch (_id) {
        default: *reinterpret_cast<int*>(_a[0]) = -1; break;
        case 0:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GUICapabilityDefinition >(); break;
            }
            break;
        case 3:
            switch (*reinterpret_cast<int*>(_a[1])) {
            default: *reinterpret_cast<int*>(_a[0]) = -1; break;
            case 0:
                *reinterpret_cast<int*>(_a[0]) = qRegisterMetaType< GUICapabilityDefinition >(); break;
            }
            break;
        }
    } else if (_c == QMetaObject::IndexOfMethod) {
        int *result = reinterpret_cast<int *>(_a[0]);
        void **func = reinterpret_cast<void **>(_a[1]);
        {
            typedef void (GUIContainer::*_t)(const GUICapabilityDefinition & );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GUIContainer::sendWidget)) {
                *result = 0;
            }
        }
        {
            typedef void (GUIContainer::*_t)(long , bool );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GUIContainer::update)) {
                *result = 1;
            }
        }
        {
            typedef void (GUIContainer::*_t)(long , long );
            if (*reinterpret_cast<_t *>(func) == static_cast<_t>(&GUIContainer::update)) {
                *result = 2;
            }
        }
    }
}

const QMetaObject GUIContainer::staticMetaObject = {
    { &QGroupBox::staticMetaObject, qt_meta_stringdata_GUIContainer.data,
      qt_meta_data_GUIContainer,  qt_static_metacall, 0, 0}
};


const QMetaObject *GUIContainer::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->dynamicMetaObject() : &staticMetaObject;
}

void *GUIContainer::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_GUIContainer.stringdata))
        return static_cast<void*>(const_cast< GUIContainer*>(this));
    return QGroupBox::qt_metacast(_clname);
}

int GUIContainer::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QGroupBox::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    } else if (_c == QMetaObject::RegisterMethodArgumentMetaType) {
        if (_id < 6)
            qt_static_metacall(this, _c, _id, _a);
        _id -= 6;
    }
    return _id;
}

// SIGNAL 0
void GUIContainer::sendWidget(const GUICapabilityDefinition & _t1)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)) };
    QMetaObject::activate(this, &staticMetaObject, 0, _a);
}

// SIGNAL 1
void GUIContainer::update(long _t1, bool _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 1, _a);
}

// SIGNAL 2
void GUIContainer::update(long _t1, long _t2)
{
    void *_a[] = { 0, const_cast<void*>(reinterpret_cast<const void*>(&_t1)), const_cast<void*>(reinterpret_cast<const void*>(&_t2)) };
    QMetaObject::activate(this, &staticMetaObject, 2, _a);
}
QT_END_MOC_NAMESPACE
