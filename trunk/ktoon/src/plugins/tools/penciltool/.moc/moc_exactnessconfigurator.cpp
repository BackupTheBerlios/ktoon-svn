/****************************************************************************
** Meta object code from reading C++ file 'exactnessconfigurator.h'
**
** Created: Mon Aug 9 03:19:01 2010
**      by: The Qt Meta Object Compiler version 62 (Qt 4.6.2)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#include "../exactnessconfigurator.h"
#if !defined(Q_MOC_OUTPUT_REVISION)
#error "The header file 'exactnessconfigurator.h' doesn't include <QObject>."
#elif Q_MOC_OUTPUT_REVISION != 62
#error "This file was generated using the moc from 4.6.2. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

QT_BEGIN_MOC_NAMESPACE
static const uint qt_meta_data_ExactnessConfigurator[] = {

 // content:
       4,       // revision
       0,       // classname
       0,    0, // classinfo
       3,   14, // methods
       0,    0, // properties
       0,    0, // enums/sets
       0,    0, // constructors
       0,       // flags
       0,       // signalCount

 // slots: signature, parameters, type, tag, flags
      23,   22,   22,   22, 0x08,
      41,   22,   22,   22, 0x08,
      67,   62,   22,   22, 0x08,

       0        // eod
};

static const char qt_meta_stringdata_ExactnessConfigurator[] = {
    "ExactnessConfigurator\0\0addCurrentValue()\0"
    "removeCurrentValue()\0item\0"
    "updateValueFromItem(QTableWidgetItem*)\0"
};

const QMetaObject ExactnessConfigurator::staticMetaObject = {
    { &QWidget::staticMetaObject, qt_meta_stringdata_ExactnessConfigurator,
      qt_meta_data_ExactnessConfigurator, 0 }
};

#ifdef Q_NO_DATA_RELOCATION
const QMetaObject &ExactnessConfigurator::getStaticMetaObject() { return staticMetaObject; }
#endif //Q_NO_DATA_RELOCATION

const QMetaObject *ExactnessConfigurator::metaObject() const
{
    return QObject::d_ptr->metaObject ? QObject::d_ptr->metaObject : &staticMetaObject;
}

void *ExactnessConfigurator::qt_metacast(const char *_clname)
{
    if (!_clname) return 0;
    if (!strcmp(_clname, qt_meta_stringdata_ExactnessConfigurator))
        return static_cast<void*>(const_cast< ExactnessConfigurator*>(this));
    return QWidget::qt_metacast(_clname);
}

int ExactnessConfigurator::qt_metacall(QMetaObject::Call _c, int _id, void **_a)
{
    _id = QWidget::qt_metacall(_c, _id, _a);
    if (_id < 0)
        return _id;
    if (_c == QMetaObject::InvokeMetaMethod) {
        switch (_id) {
        case 0: addCurrentValue(); break;
        case 1: removeCurrentValue(); break;
        case 2: updateValueFromItem((*reinterpret_cast< QTableWidgetItem*(*)>(_a[1]))); break;
        default: ;
        }
        _id -= 3;
    }
    return _id;
}
QT_END_MOC_NAMESPACE
