/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
 /* This file is part of the KDE libraries
    Copyright (C) 1997 Matthias Kalle Dalheimer (kalle@kde.org)
                  2000-2002 Stephan Kulow (coolo@kde.org)
                  2002 Holger Freyther (freyther@kde.org)

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Library General Public
    License as published by the Free Software Foundation; either
    version 2 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Library General Public License for more details.

    You should have received a copy of the GNU Library General Public License
    along with this library; see the file COPYING.LIB.  If not, write to
    the Free Software Foundation, Inc., 59 Temple Place - Suite 330,
    Boston, MA 02111-1307, USA.
 */

#ifndef KTDEBUG_H
#define KTDEBUG_H

#include <qstring.h>
//Added by qt3to4:
#include <QByteArray>
#include <QList>

class QWidget;
class QDateTime;
class QDate;
class QTime;
class QPoint;
class QSize;
class QRect;
class QRegion;
class QStringList;
class QColor;
class QPen;
class QBrush;
class QVariant;
template <class T> class QList;

class kdbgstream;
class kndbgstream;

/**
 * \addtogroup kdebug Debug message generators
 *  @{
 * KDE debug message streams let you and the user control just how many debug
 * messages you see.
 */

typedef kdbgstream & (*KDBGFUNC)(kdbgstream &); // manipulator function
typedef kndbgstream & (*KNDBGFUNC)(kndbgstream &); // manipulator function

#ifdef __GNUC__
#define k_funcinfo "[" << __PRETTY_FUNCTION__ << "] "
#else
#define k_funcinfo "[" << __FILE__ << ":" << __LINE__ << "] "
#endif

#define k_lineinfo "[" << __FILE__ << ":" << __LINE__ << "] "

class kdbgstreamprivate;
/**
 * kdbgstream is a text stream that allows you to print debug messages.
 * Using the overloaded "<<" operator you can send messages. Usually
 * you do not create the kdbgstream yourself, but use ktDebug()
 * ktWarning(), ktError() or ktFatal to obtain one.
 *
 * Example:
 * \code
 *    int i = 5;
 *    ktDebug() << "The value of i is " << i << endl;
 * \endcode
 * @see kndbgstream
 */
class kdbgstream {
 public:
  /**
   * @internal
   */
    kdbgstream(unsigned int _nOutput, unsigned int _level, bool _print = true) :
      nOutput(_nOutput), level(_level),  print(_print) { }
    kdbgstream(const char * initialString, unsigned int _nOutput, unsigned int _level, bool _print = true) :
      output(QString::fromLatin1(initialString)), nOutput(_nOutput), level(_level),  print(_print) { }
    /// Copy constructor
    kdbgstream(kdbgstream &str);
    kdbgstream(const kdbgstream &str) :
      output(str.output), nOutput(str.nOutput), level(str.level), print(str.print) {}
    ~kdbgstream();
    /**
     * Prints the given value.
     * @param i the boolean to print (as "true" or "false")
     * @return this stream
     */
    kdbgstream &operator<<(bool i)  {
	if (!print) return *this;
	output += QString::fromLatin1(i ? "true" : "false");
	return *this;
    }
    /**
     * Prints the given value.
     * @param i the short to print
     * @return this stream
     */
    kdbgstream &operator<<(short i)  {
	if (!print) return *this;
	QString tmp; tmp.setNum(i); output += tmp;
	return *this;
    }
    /**
     * Prints the given value.
     * @param i the unsigned short to print
     * @return this stream
     */
    kdbgstream &operator<<(unsigned short i) {
        if (!print) return *this;
        QString tmp; tmp.setNum(i); output += tmp;
        return *this;
    }
    /**
     * Prints the given value.
     * @param ch the char to print
     * @return this stream
     */
    kdbgstream &operator<<(char ch);
    /**
     * Prints the given value.
     * @param ch the unsigned char to print
     * @return this stream
     */
    kdbgstream &operator<<(unsigned char ch) {
        return operator<<( static_cast<char>( ch ) );
    }
    /**
     * Prints the given value.
     * @param i the int to print
     * @return this stream
     */
    kdbgstream &operator<<(int i)  {
	if (!print) return *this;
	QString tmp; tmp.setNum(i); output += tmp;
	return *this;
    }
    /**
     * Prints the given value.
     * @param i the unsigned int to print
     * @return this stream
     */
    kdbgstream &operator<<(unsigned int i) {
        if (!print) return *this;
        QString tmp; tmp.setNum(i); output += tmp;
        return *this;
    }
    /**
     * Prints the given value.
     * @param i the long to print
     * @return this stream
     */
    kdbgstream &operator<<(long i) {
        if (!print) return *this;
        QString tmp; tmp.setNum(i); output += tmp;
        return *this;
    }
    /**
     * Prints the given value.
     * @param i the unsigned long to print
     * @return this stream
     */
    kdbgstream &operator<<(unsigned long i) {
        if (!print) return *this;
        QString tmp; tmp.setNum(i); output += tmp;
        return *this;
    }
    /**
     * Prints the given value.
     * @param i the long long to print
     * @return this stream
     */
    kdbgstream &operator<<(qlonglong i) {
        if (!print) return *this;
        QString tmp; tmp.setNum(i); output += tmp;
        return *this;
    }
    /**
     * Prints the given value.
     * @param i the unsigned long long to print
     * @return this stream
     */
    kdbgstream &operator<<(qulonglong i) {
        if (!print) return *this;
        QString tmp; tmp.setNum(i); output += tmp;
        return *this;
    }

    /**
     * Flushes the output.
     */
    void flush(); //AB: maybe this should be virtual! would save some trouble for some 3rd party projects

    /**
     * Prints the given value.
     * @param ch the char to print
     * @return this stream
     * @since 3.3
     */
    kdbgstream &operator<<(QChar ch);
    /**
     * Prints the given value.
     * @param string the string to print
     * @return this stream
     */
    kdbgstream &operator<<(const QString& string) {
	if (!print)
	{
		return *this;
	}
	
	output += string;

	if (output.at(output.length() -1 ) == '\n')
	{
	    flush();
	}

	return *this;
    }
    /**
     * Prints the given value.
     * @param string the string to print
     * @return this stream
     */
    kdbgstream &operator<<(const char *string) {
	if (!print) return *this;
	output += QString::fromUtf8(string);
	if (output.at(output.length() - 1) == '\n')
	    flush();
	return *this;
    }
    /**
     * Prints the given value.
     * @param string the string to print
     * @return this stream
     */
//     kdbgstream &operator<<(const QByteArray& string) {
//         *this << string.data();
//         return *this;
//     }
    /**
     * Prints the given value.
     * @param p a pointer to print (in number form)
     * @return this stream
     */
    kdbgstream& operator<<(const void * p) {
        form("%p", p);
        return *this;
    }
    /**
     * Invokes the given function.
     * @param f the function to invoke
     * @return the return value of @p f
     */
    kdbgstream& operator<<(KDBGFUNC f) {
	if (!print) return *this;
	return (*f)(*this);
    }
    /**
     * Prints the given value.
     * @param d the double to print
     * @return this stream
     */
    kdbgstream& operator<<(double d) {
      QString tmp; tmp.setNum(d); output += tmp;
      return *this;
    }
    /**
     * Prints the string @p format which can contain
     * printf-style formatted values.
     * @param format the printf-style format
     * @return this stream
     */
    kdbgstream &form(const char *format, ...)
#ifdef __GNUC__
      __attribute__ ( ( format ( printf, 2, 3 ) ) )
#endif
     ;

    /** Operator to print out basic information about a QWidget.
     *  Output of class names only works if the class is moc'ified.
     * @param widget the widget to print
     * @return this stream
     */
    kdbgstream& operator << (const QWidget* widget);
    kdbgstream& operator << (QWidget* widget); // KDE4 merge

    /**
     * Prints the given value.
     * @param dateTime the datetime to print
     * @return this stream
     */
    kdbgstream& operator << ( const QDateTime& dateTime );

    /**
     * Prints the given value.
     * @param date the date to print
     * @return this stream
     */
    kdbgstream& operator << ( const QDate& date );

    /**
     * Prints the given value.
     * @param time the time to print
     * @return this stream
     */
    kdbgstream& operator << ( const QTime& time );

    /**
     * Prints the given value.
     * @param point the point to print
     * @return this stream
     */
    kdbgstream& operator << ( const QPoint& point );

    /**
     * Prints the given value.
     * @param size the QSize to print
     * @return this stream
     */
    kdbgstream& operator << ( const QSize& size );

    /**
     * Prints the given value.
     * @param rect the QRect to print
     * @return this stream
     */
    kdbgstream& operator << ( const QRect& rect);

    /**
     * Prints the given value.
     * @param region the QRegion to print
     * @return this stream
     */
    kdbgstream& operator << ( const QRegion& region);

    /**
     * Prints the given value.
     * @param list the stringlist to print
     * @return this stream
     */
    // ### KDE4: Remove in favor of template operator for QValueList<T> below
    kdbgstream& operator << ( const QStringList& list);

    /**
     * Prints the given value.
     * @param color the color to print
     * @return this stream
     */
    kdbgstream& operator << ( const QColor& color);

    /**
     * Prints the given value.
     * @param pen the pen to print
     * @return this stream
     * @since 3.2
     */
    kdbgstream& operator << ( const QPen& pen );

    /**
     * Prints the given value.
     * @param brush the brush to print
     * @return this stream
     */
    kdbgstream& operator << ( const QBrush& brush );

    /**
     * Prints the given value.
     * @param variant the variant to print
     * @return this stream
     * @since 3.3
     */
    kdbgstream& operator << ( const QVariant& variant );

    /**
     * Prints the given value.
     * @param data the byte array to print
     * @return this stream
     * @since 3.3
     */
//     kdbgstream& operator << ( const QByteArray& data );

    /**
     * Prints the given value
     * @param list the list to print
     * @return this stream
     * @since 3.3
     */
    template <class T>
    kdbgstream& operator << ( const QList<T> &list );

 private:
    QString output;
    unsigned int nOutput, level;
    bool print;
    kdbgstreamprivate* d;
};

template <class T>
kdbgstream &kdbgstream::operator<<( const QList<T> &list )
{
    *this << "(";
    typename QList<T>::ConstIterator it = list.begin();
    if ( !list.isEmpty() ) {
      *this << *it++;
    }
    for ( ; it != list.end(); ++it ) {
      *this << "," << *it;
    }
    *this << ")";
    return *this;
}

/**
 * \relates KGlobal
 * Prints an "\n".
 * @param s the debug stream to write to
 * @return the debug stream (@p s)
 */
inline kdbgstream &endl( kdbgstream &s) { s << "\n"; return s; }

/**
 * \relates KGlobal
 * Flushes the stream.
 * @param s the debug stream to write to
 * @return the debug stream (@p s)
 */
inline kdbgstream &flush( kdbgstream &s) { s.flush(); return s; }

kdbgstream &perror( kdbgstream &s);

/**
 * \relates KGlobal
 * kndbgstream is a dummy variant of kdbgstream. All functions do
 * nothing.
 * @see kndDebug()
 */
class kndbgstream {
 public:
    /// Default constructor.
    kndbgstream() {}
    ~kndbgstream() {}
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream &operator<<(short int )  { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream &operator<<(unsigned short int )  { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream &operator<<(char )  { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream &operator<<(unsigned char )  { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream &operator<<(int )  { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream &operator<<(unsigned int )  { return *this; }
    /**
     * Does nothing.
     */
    void flush() {}
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream &operator<<(QChar)  { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream &operator<<(const QString& ) { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
//     kndbgstream &operator<<(const QByteArray& ) { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream &operator<<(const char *) { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream& operator<<(const void *) { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream& operator<<(void *) { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream& operator<<(double) { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream& operator<<(long) { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream& operator<<(unsigned long) { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream& operator<<(qlonglong) { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream& operator<<(qulonglong) { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream& operator<<(KNDBGFUNC) { return *this; }
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream& operator << (const QWidget*) { return *this; }
    kndbgstream& operator << (QWidget*) { return *this; } // KDE4 merge
    /**
     * Does nothing.
     * @return this stream
     */
    kndbgstream &form(const char *, ...) { return *this; }

    kndbgstream& operator<<( const QDateTime& ) { return *this; }
    kndbgstream& operator<<( const QDate&     ) { return *this; }
    kndbgstream& operator<<( const QTime&     ) { return *this; }
    kndbgstream& operator<<( const QPoint & )  { return *this; }
    kndbgstream& operator<<( const QSize & )  { return *this; }
    kndbgstream& operator<<( const QRect & )  { return *this; }
    kndbgstream& operator<<( const QRegion & ) { return *this; }
    kndbgstream& operator<<( const QStringList & ) { return *this; }
    kndbgstream& operator<<( const QColor & ) { return *this; }
    kndbgstream& operator<<( const QPen & ) { return *this; }
    kndbgstream& operator<<( const QBrush & ) { return *this; }
    kndbgstream& operator<<( const QVariant & ) { return *this; }
//     kndbgstream& operator<<( const QByteArray & ) { return *this; }

    template <class T>
    kndbgstream& operator<<( const QList<T> & ) { return *this; }
};

/**
 * Does nothing.
 * @param s a stream
 * @return the given @p s
 */
inline kndbgstream &endl( kndbgstream & s) { return s; }
/**
 * Does nothing.
 * @param s a stream
 * @return the given @p s
 */
inline kndbgstream &flush( kndbgstream & s) { return s; }
inline kndbgstream &perror( kndbgstream & s) { return s; }

/**
 * \relates KGlobal
 * Returns a debug stream. You can use it to print debug
 * information.
 * @param nOutput an id to identify the output, 0 for default
 * @see kndDebug()
 */
kdbgstream ktDebug(int nOutput = 2);
kdbgstream ktDebug(bool cond, int nOutput = 2);
/**
 * \relates KGlobal
 * Returns a backtrace.
 * @return a backtrace
 */
QString kdBacktrace();
/**
 * \relates KGlobal
 * Returns a backtrace.
 * @param levels the number of levels of the backtrace
 * @return a backtrace
 * @since 3.1
 */
QString kdBacktrace(int levels);
/**
 * Returns a dummy debug stream. The stream does not print anything.
 * @param nOutput an id to identify the output, 0 for default
 * @see ktDebug()
 */
inline kndbgstream kndDebug(int nOutput = 2) { Q_UNUSED(nOutput); return kndbgstream(); }
inline kndbgstream kndDebug(bool , int  = 2) { return kndbgstream(); }
inline QString kndBacktrace() { return QString::null; }
inline QString kndBacktrace(int) { return QString::null; }

/**
 * \relates KGlobal
 * Returns a warning stream. You can use it to print warning
 * information.
 * @param nOutput an id to identify the output, 0 for default
 */
kdbgstream ktWarning(int nOutput = 2);
kdbgstream ktWarning(bool cond, int nOutput = 2);
/**
 * \relates KGlobal
 * Returns an error stream. You can use it to print error
 * information.
 * @param nOutput an id to identify the output, 0 for default
 */
kdbgstream ktError(int nOutput = 2);
kdbgstream ktError(bool cond, int nOutput = 2);
/**
 * \relates KGlobal
 * Returns a fatal error stream. You can use it to print fatal error
 * information.
 * @param nOutput an id to identify the output, 0 for default
 */
kdbgstream ktFatal(int nOutput = 2);
kdbgstream ktFatal(bool cond, int nOutput = 2);

/**
 * \relates KGlobal
 * Deletes the kdebugrc cache and therefore forces KDebug to reread the
 * config file
 */
void kdClearDebugConfig();

/** @} */

// #define KTINIT ktDebug() << "[Initializing "<< metaObject()->className() << "]" << endl;
// #define KTEND ktDebug() << "[Destroying "<< metaObject()->className() <<"]" << endl;

#define KTINIT ktDebug() << "[Initializing "<< k_funcinfo << "]" << endl;
#define KTEND ktDebug() << "[Destroying "<< k_funcinfo <<"]" << endl;

#endif
