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

// NOTE: Adapted to KToon from KDE for David Cuadrado.

#include "ktdebug.h"

#include "ktapplication.h"

#include <qmessagebox.h>

#include <qfile.h>
#include <q3intdict.h>
#include <qstring.h>
#include <qdatetime.h>
#include <qpoint.h>
#include <qrect.h>
#include <qregion.h>
#include <qstringlist.h>
#include <qpen.h>
#include <qbrush.h>
#include <qsize.h>
#include <qvariant.h>
//Added by qt3to4:
#include <QByteArray>
#include <Q3MemArray>

#include <stdlib.h>	// abort
#include <unistd.h>	// getpid
#include <stdarg.h>	// vararg stuff
#include <ctype.h>      // isprint

#ifndef Q_WS_WIN
#include <syslog.h>
#endif

#include <errno.h>
#include <cstring>

class KTDebugEntry;

class KTDebugEntry
{
	public:
		KTDebugEntry (int n, const QByteArray& d) {number=n; descr=d;}
		unsigned int number;
		QByteArray descr;
};

enum DebugLevels {
	KTDEBUG_INFO=    0,
	KTDEBUG_WARN=    1,
	KTDEBUG_ERROR=   2,
	KTDEBUG_FATAL=   3
};

#ifndef HAVE_STRLCPY
unsigned long strlcpy(char* d, const char* s, unsigned long bufsize)
{
	unsigned long len, ret = strlen(s);

	if (ret >= bufsize) {
		if (bufsize) {
			len = bufsize - 1;
			memcpy(d, s, len);
			d[len] = '\0';
		}
	} else
		memcpy(d, s, ret + 1);
	
		return ret;
}
#endif

static void kDebugBackend( unsigned short nLevel, int nOutput, const char *data)
{
	int nPriority = 0;
	QString aCaption;

	/* Determine output */

	QString key;
	switch( nLevel )
	{
		case KTDEBUG_INFO:
			key = "InfoOutput";
			aCaption = "Info";
			nPriority = LOG_INFO;
			break;
		case KTDEBUG_WARN:
			key = "WarnOutput";
			aCaption = "Warning";
			nPriority = LOG_WARNING;
			break;
		case KTDEBUG_FATAL:
			key = "FatalOutput";
			aCaption = "Fatal Error";
			nPriority = LOG_CRIT;
			break;
		case KTDEBUG_ERROR:
		default:
			/* Programmer error, use "Error" as default */
			key = "ErrorOutput";
			aCaption = "Error";
			nPriority = LOG_ERR;
			break;
	}

	/* Determine output */

	switch( nLevel )
	{
		case KTDEBUG_INFO:
			aCaption = "Info";
			nPriority = LOG_INFO;
			break;
		case KTDEBUG_WARN:
			aCaption = "Warning";
			nPriority = LOG_WARNING;
			break;
		case KTDEBUG_FATAL:
			aCaption = "Fatal Error";
			nPriority = LOG_CRIT;
			break;
		case KTDEBUG_ERROR:
		default:
			/* Programmer error, use "Error" as default */
			aCaption = "Error";
			nPriority = LOG_ERR;
			break;
	}

  // If the application doesn't have a QApplication object it can't use
  // a messagebox.
	if (!qApp && (nOutput == 1))
		nOutput = 2;
	else if ( nOutput == 4 && nLevel != KTDEBUG_FATAL )
		return;
	
	///////////

	const int BUFSIZE = 4096;
	char buf[BUFSIZE];
	int nSize = strlcpy( buf, data, BUFSIZE );

  // Output
	switch( nOutput ) // TODO: FIXME
	{
		case 0: // File
		{
			const char* aKey;
			switch( nLevel )
			{
				case KTDEBUG_INFO:
					aKey = "InfoFilename";
					break;
				case KTDEBUG_WARN:
					aKey = "WarnFilename";
					break;
				case KTDEBUG_FATAL:
					aKey = "FatalFilename";
					break;
				case KTDEBUG_ERROR:
				default:
					aKey = "ErrorFilename";
					break;
			}
			QFile aOutputFile( "ktoon.dbg" );
			aOutputFile.open( QIODevice::WriteOnly | QIODevice::Append/* | IO_Raw */);
			if ( ( nSize == -1 ) || ( nSize >= BUFSIZE ) )
				aOutputFile.writeBlock( buf, BUFSIZE-1 );
			else
				aOutputFile.writeBlock( buf, nSize );
			aOutputFile.close();
			break;
		}
		case 1: // Message Box
		{
      // Since we are in kdecore here, we cannot use KMsgBox and use
      // QMessageBox instead
			QMessageBox::warning( 0L, aCaption, data, QObject::tr("&OK") );
			break;
		}
		case 2: // Shell
		{
			write( 2, buf, nSize ); //fputs( buf, stderr );
			break;
		}
		case 3: // syslog
		{
#ifndef Q_WS_WIN
			syslog( nPriority, "%s", buf);
#else
			write( 2, buf, nSize );
#endif
			break;
		}
	}
}


kdbgstream &perror( kdbgstream &s) { return s <<  QString::fromLocal8Bit(strerror(errno)); }
kdbgstream ktDebug(int nOutput) { return kdbgstream(nOutput, KTDEBUG_INFO); }
kdbgstream ktDebug(bool cond, int nOutput) { if (cond) return kdbgstream(nOutput, KTDEBUG_INFO); else return kdbgstream(0, 0, false); }

kdbgstream ktError(int nOutput) { return kdbgstream("ERROR: ", nOutput, KTDEBUG_ERROR); }
kdbgstream ktError(bool cond, int nOutput) { if (cond) return kdbgstream("ERROR: ", nOutput, KTDEBUG_ERROR); else return kdbgstream(0,0,false); }
kdbgstream ktWarning(int nOutput) { return kdbgstream("WARNING: ", nOutput, KTDEBUG_WARN); }
kdbgstream ktWarning(bool cond, int nOutput) { if (cond) return kdbgstream("WARNING: ", nOutput, KTDEBUG_WARN); else return kdbgstream(0,0,false); }
kdbgstream ktFatal(int nOutput) { return kdbgstream("FATAL: ", nOutput, KTDEBUG_FATAL); }
kdbgstream ktFatal(bool cond, int nOutput) { if (cond) return kdbgstream("FATAL: ", nOutput, KTDEBUG_FATAL); else return kdbgstream(0,0,false); }

kdbgstream::kdbgstream(kdbgstream &str) : output(str.output), nOutput(str.nOutput), level(str.level), print(str.print) 
{
	str.output.truncate(0); 
}

void kdbgstream::flush() {

	if (output.isEmpty() || !print)
		return;

	kDebugBackend( level, nOutput, output.local8Bit().data());
// 	qDebug(output);	
	output = QString::null;
}

kdbgstream &kdbgstream::form(const char *format, ...)
{
	char buf[4096];
	va_list arguments;
	va_start( arguments, format );
	vsnprintf( buf, sizeof(buf), format, arguments );
	va_end(arguments);
	*this << buf;
	return *this;
}

kdbgstream::~kdbgstream() {
	if (!output.isEmpty()) {
		fprintf(stderr, "ASSERT: debug output not ended with \\n\n");
		fprintf(stderr, "%s", kdBacktrace().latin1());
		*this << "\n";
	}
}

kdbgstream& kdbgstream::operator << (char ch)
{
	if (!print) return *this;
	if (!isprint(ch))
		output += "\\x" + QString::number( static_cast<uint>( ch ), 16 ).rightJustify(2, '0');
	else {
		output += ch;
		if (ch == '\n') flush();
	}
	return *this;
}

kdbgstream& kdbgstream::operator << (QChar ch)
{
	if (!print) return *this;
	if (!ch.isPrint())
		output += "\\x" + QString::number( ch.unicode(), 16 ).rightJustify(2, '0');
	else {
		output += ch;
		if (ch == '\n') flush();
	}
	return *this;
}

kdbgstream& kdbgstream::operator << (QWidget* widget)
{
	return *this << const_cast< const QWidget* >( widget );
}

kdbgstream& kdbgstream::operator << (const QWidget* widget)
{
	QString string, temp;
  // -----
	if(widget==0)
	{
		string=(QString)"[Null pointer]";
	} else {
		temp.setNum((ulong)widget, 16);
		string=(QString)"["+widget->className()+" pointer "
				+ "(0x" + temp + ")";
		if(widget->name(0)==0)
		{
			string += " to unnamed widget, ";
		} else {
			string += (QString)" to widget " + widget->name() + ", ";
		}
		string += "geometry="
				+ QString().setNum(widget->width())
				+ "x"+QString().setNum(widget->height())
				+ "+"+QString().setNum(widget->x())
				+ "+"+QString().setNum(widget->y())
				+ "]";
	}
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
/*
	* either use 'output' directly and do the flush if needed
	* or use the QString operator which calls the char* operator
	*
 */
kdbgstream& kdbgstream::operator<<( const QDateTime& time) {
	*this << time.toString();
	return *this;
}
kdbgstream& kdbgstream::operator<<( const QDate& date) {
	*this << date.toString();

	return *this;
}
kdbgstream& kdbgstream::operator<<( const QTime& time ) {
	*this << time.toString();
	return *this;
}
kdbgstream& kdbgstream::operator<<( const QPoint& p ) {
	*this << "(" << p.x() << ", " << p.y() << ")";
	return *this;
}
kdbgstream& kdbgstream::operator<<( const QSize& s ) {
	*this << "[" << s.width() << "x" << s.height() << "]";
	return *this;
}
kdbgstream& kdbgstream::operator<<( const QRect& r ) {
	*this << "[" << r.x() << "," << r.y() << " - " << r.width() << "x" << r.height() << "]";
	return *this;
}
kdbgstream& kdbgstream::operator<<( const QRegion& reg ) {
	*this<< "[ ";

	Q3MemArray<QRect>rs=reg.rects();
	for (uint i=0;i<rs.size();++i)
		*this << QString("[%1,%2 - %3x%4] ").arg(rs[i].x()).arg(rs[i].y()).arg(rs[i].width()).arg(rs[i].height() ) ;

	*this <<"]";
	return *this;
}
kdbgstream& kdbgstream::operator<<( const QStringList& l ) {
	*this << "(";
	*this << l.join(",");
	*this << ")";

	return *this;
}
kdbgstream& kdbgstream::operator<<( const QColor& c ) {
	if ( c.isValid() )
		*this << c.name();
	else
		*this << "(invalid/default)";
	return *this;
}
kdbgstream& kdbgstream::operator<<( const QPen& p ) {
	static const char* const s_penStyles[] = {
		"NoPen", "SolidLine", "DashLine", "DotLine", "DashDotLine",
		"DashDotDotLine" };
		static const char* const s_capStyles[] = {
			"FlatCap", "SquareCap", "RoundCap" };
			*this << "[ style:";
			*this << s_penStyles[ p.style() ];
			*this << " width:";
			*this << p.width();
			*this << " color:";
			if ( p.color().isValid() )
				*this << p.color().name();
			else
				*this <<"(invalid/default)";
			if ( p.width() > 0 ) // cap style doesn't matter, otherwise
			{
				*this << " capstyle:";
				*this << s_capStyles[ p.capStyle() >> 4 ];
// join style omitted
			}
			*this <<" ]";
			return *this;
}
kdbgstream& kdbgstream::operator<<( const QBrush& b) {
	static const char* const s_brushStyles[] = {
		"NoBrush", "SolidPattern", "Dense1Pattern", "Dense2Pattern", "Dense3Pattern",
		"Dense4Pattern", "Dense5Pattern", "Dense6Pattern", "Dense7Pattern",
		"HorPattern", "VerPattern", "CrossPattern", "BDiagPattern", "FDiagPattern",
		"DiagCrossPattern" };

		*this <<"[ style: ";
		*this <<s_brushStyles[ b.style() ];
		*this <<" color: ";
    // can't use operator<<(str, b.color()) because that terminates a kdbgstream (flushes)
		if ( b.color().isValid() )
			*this <<b.color().name() ;
		else
			*this <<"(invalid/default)";
		if ( b.pixmap() )
			*this <<" has a pixmap";
		*this <<" ]";
		return *this;
}

kdbgstream& kdbgstream::operator<<( const QVariant& v) {
	*this << "[variant: ";
	*this << v.typeName();
    // For now we just attempt a conversion to string.
    // Feel free to switch(v.type()) and improve the output.
	*this << " toString=";
	*this << v.toString();
	*this << "]";
	return *this;
}

// kdbgstream& kdbgstream::operator<<( const QByteArray& data) {
// 	if (!print) return *this;
// 	output += '[';
// 	unsigned int i = 0;
// 	unsigned int sz = QMIN( data.size(), 64 );
// 	for ( ; i < sz ; ++i ) {
// 		output += QString::number( data[i], 16 ).rightJustify(2, '0');
// 		if ( i < sz )
// 			output += ' ';
// 	}
// 	if ( sz < data.size() )
// 		output += "...";
// 	output += ']';
// 	return *this;
// }

QString kdBacktrace(int levels)
{
	QString s;

	return s;
}

QString kdBacktrace()
{
	return kdBacktrace(-1 /*all*/);
}

void kdClearDebugConfig()
{
}




