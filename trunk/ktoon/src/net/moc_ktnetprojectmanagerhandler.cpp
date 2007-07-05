/****************************************************************************
** Meta object code from reading C++ file 'ktnetprojectmanagerhandler.h'
**
** Created: Sat Dec 30 11:42:10 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ktnetprojectmanagerhandler.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ktnetprojectmanagerhandler.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_KTNetProjectManagerHandler[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      28,   27,   27,   27, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_KTNetProjectManagerHandler[] = {
    "KTNetProjectManagerHandler\0\0sendHello()\0"
};

const QMetaObject KTNetProjectManagerHandler::staticMetaObject = {
    { &KTAbstractProjectHandler::staticMetaObject, qt_meta_stringdata_KTNetProjectManagerHandler,
      qt_meta_data_KTNetProjectManagerHandler, 0 }
};

const QMetaObject *KTNetProjectManagerHandler::metaObject() const
{
    return &staticMetaObject;
}

void *KTNetProjectManagerHandler::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KTNetProjectManagerHandler))
	return static_cast<void*>(const_cast<KTNetProjectManagerHandler*>(this));
    return KTAbstractProjectHandler::qt_metacast(_clname);
}

int KTNetProjectManagerHandler::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = KTAbstractProjectHandler::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: sendHello(); break;
        }
        _id -= 1;
    }
    return _id;
}
