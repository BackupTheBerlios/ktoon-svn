/****************************************************************************
** Meta object code from reading C++ file 'ktnetsocket.h'
**
** Created: Sat Dec 30 11:42:11 2006
**      by: The Qt Meta Object Compiler version 59 (Qt 4.2.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "ktnetsocket.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'ktnetsocket.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 59
#error "This file was generated using the moc from 4.2.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

static const uint qt_meta_data_KTNetSocket[] = {

 // content:
       1,       // revision
       0,       // classname
       0,    0, // classinfo
       1,   10, // methods
       0,    0, // properties
       0,    0, // enums/sets

 // slots: signature, parameters, type, tag, flags
      13,   12,   12,   12, 0x09,

       0        // eod
};

static const char qt_meta_stringdata_KTNetSocket[] = {
    "KTNetSocket\0\0readFromServer()\0"
};

const QMetaObject KTNetSocket::staticMetaObject = {
    { &QTcpSocket::staticMetaObject, qt_meta_stringdata_KTNetSocket,
      qt_meta_data_KTNetSocket, 0 }
};

const QMetaObject *KTNetSocket::metaObject() const
{
    return &staticMetaObject;
}

void *KTNetSocket::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_KTNetSocket))
	return static_cast<void*>(const_cast<KTNetSocket*>(this));
    return QTcpSocket::qt_metacast(_clname);
}

int KTNetSocket::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QTcpSocket::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: readFromServer(); break;
        }
        _id -= 1;
    }
    return _id;
}
