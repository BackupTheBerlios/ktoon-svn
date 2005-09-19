/****************************************************************************
** KTCommandHistoryBase meta object code from reading C++ file 'ktcommandhistory.h'
**
** Created: Wed Apr 20 23:26:15 2005
**      by: The Qt MOC ($Id: qt/moc_yacc.cpp   3.3.3   edited Aug 5 16:40 $)
**
** WARNING! All changes made in this file will be lost!
*****************************************************************************/

#undef QT_NO_COMPAT
#include "ktcommandhistory.h"
#include <qmetaobject.h>
#include <qapplication.h>

#include <private/qucomextra_p.h>
#if !defined(Q_MOC_OUTPUT_REVISION) || (Q_MOC_OUTPUT_REVISION != 26)
#error "This file was generated using the moc from 3.3.3. It"
#error "cannot be used with the include files from this version of Qt."
#error "(The moc has changed too much.)"
#endif

const char *KTCommandHistoryBase::className() const
{
    return "KTCommandHistoryBase";
}

QMetaObject *KTCommandHistoryBase::metaObj = 0;
static QMetaObjectCleanUp cleanUp_KTCommandHistoryBase( "KTCommandHistoryBase", &KTCommandHistoryBase::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString KTCommandHistoryBase::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KTCommandHistoryBase", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString KTCommandHistoryBase::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KTCommandHistoryBase", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* KTCommandHistoryBase::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = QObject::staticMetaObject();
    static const QUMethod slot_0 = {"undoInternal", 0, 0 };
    static const QUMethod slot_1 = {"redoInternal", 0, 0 };
    static const QUMethod slot_2 = {"undo", 0, 0 };
    static const QUMethod slot_3 = {"redo", 0, 0 };
    static const QUParameter param_slot_4[] = {
	{ "which", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_4 = {"undoUpToNumber", 1, param_slot_4 };
    static const QUParameter param_slot_5[] = {
	{ "which", &static_QUType_int, 0, QUParameter::In }
    };
    static const QUMethod slot_5 = {"redoUpToNumber", 1, param_slot_5 };
    static const QUMethod slot_6 = {"documentSaved", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "undoInternal()", &slot_0, QMetaData::Protected },
	{ "redoInternal()", &slot_1, QMetaData::Protected },
	{ "undo()", &slot_2, QMetaData::Public },
	{ "redo()", &slot_3, QMetaData::Public },
	{ "undoUpToNumber(int)", &slot_4, QMetaData::Public },
	{ "redoUpToNumber(int)", &slot_5, QMetaData::Public },
	{ "documentSaved()", &slot_6, QMetaData::Public }
    };
    static const QUMethod signal_0 = {"documentRestored", 0, 0 };
    static const QMetaData signal_tbl[] = {
	{ "documentRestored()", &signal_0, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"KTCommandHistoryBase", parentObject,
	slot_tbl, 7,
	signal_tbl, 1,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_KTCommandHistoryBase.setMetaObject( metaObj );
    return metaObj;
}

void* KTCommandHistoryBase::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "KTCommandHistoryBase" ) )
	return this;
    return QObject::qt_cast( clname );
}

// SIGNAL documentRestored
void KTCommandHistoryBase::documentRestored()
{
    activate_signal( staticMetaObject()->signalOffset() + 0 );
}

bool KTCommandHistoryBase::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: undoInternal(); break;
    case 1: redoInternal(); break;
    case 2: undo(); break;
    case 3: redo(); break;
    case 4: undoUpToNumber((int)static_QUType_int.get(_o+1)); break;
    case 5: redoUpToNumber((int)static_QUType_int.get(_o+1)); break;
    case 6: documentSaved(); break;
    default:
	return QObject::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool KTCommandHistoryBase::qt_emit( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->signalOffset() ) {
    case 0: documentRestored(); break;
    default:
	return QObject::qt_emit(_id,_o);
    }
    return TRUE;
}
#ifndef QT_NO_PROPERTIES

bool KTCommandHistoryBase::qt_property( int id, int f, QVariant* v)
{
    return QObject::qt_property( id, f, v);
}

bool KTCommandHistoryBase::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES


const char *KTCommandHistory::className() const
{
    return "KTCommandHistory";
}

QMetaObject *KTCommandHistory::metaObj = 0;
static QMetaObjectCleanUp cleanUp_KTCommandHistory( "KTCommandHistory", &KTCommandHistory::staticMetaObject );

#ifndef QT_NO_TRANSLATION
QString KTCommandHistory::tr( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KTCommandHistory", s, c, QApplication::DefaultCodec );
    else
	return QString::fromLatin1( s );
}
#ifndef QT_NO_TRANSLATION_UTF8
QString KTCommandHistory::trUtf8( const char *s, const char *c )
{
    if ( qApp )
	return qApp->translate( "KTCommandHistory", s, c, QApplication::UnicodeUTF8 );
    else
	return QString::fromUtf8( s );
}
#endif // QT_NO_TRANSLATION_UTF8

#endif // QT_NO_TRANSLATION

QMetaObject* KTCommandHistory::staticMetaObject()
{
    if ( metaObj )
	return metaObj;
    QMetaObject* parentObject = KTCommandHistoryBase::staticMetaObject();
    static const QUMethod slot_0 = {"undo", 0, 0 };
    static const QUMethod slot_1 = {"redo", 0, 0 };
    static const QMetaData slot_tbl[] = {
	{ "undo()", &slot_0, QMetaData::Public },
	{ "redo()", &slot_1, QMetaData::Public }
    };
    metaObj = QMetaObject::new_metaobject(
	"KTCommandHistory", parentObject,
	slot_tbl, 2,
	0, 0,
#ifndef QT_NO_PROPERTIES
	0, 0,
	0, 0,
#endif // QT_NO_PROPERTIES
	0, 0 );
    cleanUp_KTCommandHistory.setMetaObject( metaObj );
    return metaObj;
}

void* KTCommandHistory::qt_cast( const char* clname )
{
    if ( !qstrcmp( clname, "KTCommandHistory" ) )
	return this;
    return KTCommandHistoryBase::qt_cast( clname );
}

bool KTCommandHistory::qt_invoke( int _id, QUObject* _o )
{
    switch ( _id - staticMetaObject()->slotOffset() ) {
    case 0: undo(); break;
    case 1: redo(); break;
    default:
	return KTCommandHistoryBase::qt_invoke( _id, _o );
    }
    return TRUE;
}

bool KTCommandHistory::qt_emit( int _id, QUObject* _o )
{
    return KTCommandHistoryBase::qt_emit(_id,_o);
}
#ifndef QT_NO_PROPERTIES

bool KTCommandHistory::qt_property( int id, int f, QVariant* v)
{
    return KTCommandHistoryBase::qt_property( id, f, v);
}

bool KTCommandHistory::qt_static_property( QObject* , int , int , QVariant* ){ return FALSE; }
#endif // QT_NO_PROPERTIES
