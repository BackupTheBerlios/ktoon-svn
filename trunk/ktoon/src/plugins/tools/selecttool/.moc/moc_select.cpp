/****************************************************************************
** Meta object code from reading C++ file 'select.h'
**
** Created: Thu Jul 29 19:56:46 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../select.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'select.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_Select[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       2,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
       8,    7,    7,    7, 0x08,
      20,    7,    7,    7, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_Select[] = {
    "Select\0\0syncNodes()\0updateItems(KTGraphicsScene*)\0"
};

const QMetaObject Select::staticMetaObject = {
    { &KTToolPlugin::staticMetaObject, qt_meta_stringdata_Select,
      qt_meta_data_Select, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &Select::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *Select::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *Select::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_Select))
        return static_cast<void*>(const_cast< Select*>(this));
    return KTToolPlugin::qt_metacast(_clname);
}

int Select::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KTToolPlugin::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: syncNodes(); break;
        case 1: updateItems((*reinterpret_cast< KTGraphicsScene*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 2;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
