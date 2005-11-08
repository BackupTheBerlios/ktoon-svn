/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#ifndef KTDEBUG_H
#define KTDEBUG_H

#include <QTextStream>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

#ifdef __GNUC__
#define KT_FUNCINFO ktDebug() << "[" << __PRETTY_FUNCTION__ << "] ";
#define KTINIT ktDebug() << "[Initializing " << __FUNCTION__ << "]";
#define KTEND ktDebug() << "[Destroying " << __FUNCTION__ << "]";
#else
#define KT_FUNCINFO
#define KTINIT ktDebug() << "[Initializing " << __FILE__ << ":" << __LINE__ << "] ";
#define KTEND ktDebug() << "[Destroying " << __FILE__ << ":" << __LINE__ << "] ";
#endif

#define SHOW_VAR(arg) ktDebug() << #arg << " = " << arg;


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
class QPixmap;
class QIcon;
class QImage;
template <class T> class QList;

enum DebugType
{
	KTDebugMsg = 0,
	KTWarningMsg,
	KTErrorMsg,
	KTFatalMsg 
};

#if !defined(KT_NODEBUG)
class KTDebug
{
	public:
		class Streamer
		{
			public:
				Streamer() : ts(&buffer, QIODevice::WriteOnly), space(true) {}
				~Streamer() {};
				QTextStream ts;
				QString buffer;
				bool space;
		} *streamer;
		
		
		KTDebug(DebugType t);
		~KTDebug();
		
		
		inline KTDebug &operator<<(QTextStreamManipulator m)
		{ 
			streamer->ts << m; 
			return *this; 
		}
		
		inline KTDebug &operator<<(QTextStreamFunction f) {
			streamer->ts << f;
			return *this;
		}
		
		inline KTDebug& operator << (const QString &str)
		{
			streamer->ts << "\"" << str << "\"";
			return *this;
		};
		
		inline KTDebug& operator << (char c)
		{
			streamer->ts << "\'" << c << "\'";
			return *this;
		};
		
		inline KTDebug &operator<<(bool t) 
		{ 
			streamer->ts << (t ? "true" : "false"); 
			return *this; 
		}

		inline KTDebug &operator<<(signed short t) 
		{ 
			streamer->ts << t; 
			return *this; 
		}
		inline KTDebug &operator<<(unsigned short t) 
		{ 
			streamer->ts << t; 
			return *this; 
		}
		inline KTDebug &operator<<(signed int t) 
		{ 
			streamer->ts << t; 
			return *this; 
		}
		inline KTDebug &operator<<(unsigned int t) 
		{ 
			streamer->ts << t; 
			return *this; 
		}
		inline KTDebug &operator<<(signed long t) 
		{ 
			streamer->ts << t; 
			return *this; 
		}
		inline KTDebug &operator<<(unsigned long t) 
		{ 
			streamer->ts << t; 
			return *this; 
		}
		inline KTDebug &operator<<(float t) 
		{ 
			streamer->ts << t; 
			return *this; 
		}
		inline KTDebug &operator<<(double t) 
		{ 
			streamer->ts << t; 
			return *this; 
		}
		inline KTDebug &operator<<(const char* t) 
		{ 
			streamer->ts  << t; 
			return *this; 
		}
		KTDebug& operator<<( const QPixmap& p );
		KTDebug& operator<<( const QIcon& p );
		KTDebug& operator<<( const QImage& p ) ;
		KTDebug& operator<<( const QDateTime& );
		KTDebug& operator<<( const QDate&     );
		KTDebug& operator<<( const QTime&     );
		KTDebug& operator<<( const QPoint & ) ;
		KTDebug& operator<<( const QSize & ) ;
		KTDebug& operator<<( const QRect & ) ;
		KTDebug& operator<<( const QRegion & );
		KTDebug& operator<<( const QStringList & );
		KTDebug& operator<<( const QColor & );
		KTDebug& operator<<( const QPen & );
		KTDebug& operator<<( const QBrush & );
		KTDebug& operator<<( const QVariant & );
		KTDebug& operator << (const QWidget*);
		
		template <class T> KTDebug& operator << ( const QList<T> &list );
		
	private:
		DebugType m_type;
};

template <class T> KTDebug &KTDebug::operator<<( const QList<T> &list )
{
	*this << "(";
	typename QList<T>::ConstIterator it = list.begin();
	if ( !list.isEmpty() ) 
	{
		*this << *it++;
	}
	for ( ; it != list.end(); ++it ) 
	{
		*this << "," << *it;
	}
	*this << ")";
	return *this;
}

// Global functions
inline KTDebug ktDebug()
{
	return KTDebug(KTDebugMsg);
}

inline KTDebug ktFatal()
{
	return KTDebug(KTFatalMsg);
}

inline KTDebug ktError()
{
	return KTDebug(KTErrorMsg);
}

inline KTDebug ktWarning()
{
	return KTDebug(KTWarningMsg);
}

#else // KT_NODEBUG

class KTNDebug
{
	public:
		inline KTNDebug(){}
		inline KTNDebug(const KTNDebug &){}
		inline ~KTNDebug(){}
		inline KTNDebug &operator<<(QTextStreamFunction) { return *this; }
		inline KTNDebug &operator<<(QTextStreamManipulator) { return *this; }
		inline KTNDebug &space() { return *this; }
		inline KTNDebug &nospace() { return *this; }
		inline KTNDebug &maybeSpace() { return *this; }
		template<typename T> inline KTNDebug &operator<<(const T &) { return *this; }
};

inline KTNDebug ktDebug()
{
	return KTNDebug();
}

#define ktFatal ktDebug
#define ktError ktDebug
#define ktWarning ktDebug

#endif // KT_NODEBUG

#endif
