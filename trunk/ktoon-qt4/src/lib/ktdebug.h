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
#include <QColor>

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

#ifdef __GNUC__
#define KT_FUNCINFO ktDebug() << "[" << __PRETTY_FUNCTION__ << "] ";
#define KTINIT ktDebug() << "[Initializing " << __FUNCTION__ << "]";
#define KTEND ktDebug() << "[Destroying " << __FUNCTION__ << "]";
#define FUNC_NOT_IMPLEMENTED ktWarning() << __FILE__ << ":" << __LINE__ << " " << __PRETTY_FUNCTION__ << " not implemented yet";
#else
#define KT_FUNCINFO
#define KTINIT ktDebug() << "[Initializing " << __FILE__ << ":" << __LINE__ << "] ";
#define KTEND ktDebug() << "[Destroying " << __FILE__ << ":" << __LINE__ << "] ";
#define FUNC_NOT_IMPLEMENTED ktWarning() << __FILE__<<":"<<__LINE__ << " not implemented yet";
#endif

#define SHOW_VAR(arg) ktDebug() << #arg << " = " << arg;

class QPalette;

class QWidget;
class QDateTime;
class QDate;
class QTime;
class QPoint;
class QPointF;
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
		class Streamer : public QObject
		{
			public:
				Streamer() : space(true) {}
				~Streamer() {};
				QString buffer;
				bool space;
				Streamer & operator<< ( QChar c )
				{
					buffer += c;
					return *this;
				}
				Streamer & operator<< ( signed short i )
				{
					buffer += QString::number(i);
					return *this;
				}
				Streamer & operator<< ( float f )
				{
					buffer += QString::number(f);
					return *this;
				}
				Streamer & operator<< ( const QString & string )
				{
					buffer += string;
					return *this;
				}
				Streamer & operator<< ( char c )
				{
					buffer += c;
					return *this;
				}
				Streamer & operator<< ( unsigned short i )
				{
					buffer += QString::number(i);
					return *this;
				}
				Streamer & operator<< ( signed int i )
				{
					buffer += QString::number(i);
					return *this;
				}
				Streamer & operator<< ( unsigned int i )
				{
					buffer += QString::number(i);
					return *this;
				}
				Streamer & operator<< ( signed long i )
				{
					buffer += QString::number(i);
					return *this;
				}
				Streamer & operator<< ( unsigned long i )
				{
					buffer += QString::number(i);
					return *this;
				}
				Streamer & operator<< ( qlonglong i )
				{
					buffer += QString::number(i);
					return *this;
				}
				Streamer & operator<< ( qulonglong i )
				{
					buffer += QString::number(i);
					return *this;
				}
				Streamer & operator<< ( double f )
				{
					buffer += QString::number(f);
					return *this;
				}
				Streamer & operator<< ( const QByteArray & array )
				{
					buffer += array.data();
					return *this;
				}
				Streamer & operator<< ( const char * string )
				{
					buffer += string;
					return *this;
				}
				Streamer & operator<< ( const void * ptr )
				{
					
					return *this;
				}
		} *streamer;
		
		
		KTDebug(DebugType t);
		KTDebug(const KTDebug &);
		~KTDebug();
		
		void resaltWidget(QWidget *w, const QColor &color = QColor(Qt::magenta));
		
		inline KTDebug &operator<<(QTextStreamManipulator /*m*/)
		{ 
// 			streamer->ts << m; 
			return *this; 
		}
		
		inline KTDebug &operator<<(QTextStreamFunction /*f*/)
		{
// 			streamer->ts << f;
			return *this;
		}
		
		inline KTDebug& operator << (const QString &str)
		{
			*streamer << "\"" << str << "\"";
			return *this;
		};
		
		inline KTDebug& operator << (char c)
		{
			*streamer << "\'" << c << "\'";
			return *this;
		};
		
		inline KTDebug &operator<<(bool t) 
		{ 
			*streamer << (t ? "true" : "false"); 
			return *this; 
		}

		inline KTDebug &operator<<(signed short t) 
		{ 
			*streamer << t; 
			return *this; 
		}
		inline KTDebug &operator<<(unsigned short t) 
		{ 
			*streamer << t; 
			return *this; 
		}
		inline KTDebug &operator<<(signed int t) 
		{ 
			*streamer << t; 
			return *this; 
		}
		inline KTDebug &operator<<(unsigned int t) 
		{ 
			*streamer << t; 
			return *this; 
		}
		inline KTDebug &operator<<(signed long t) 
		{ 
			*streamer << t; 
			return *this; 
		}
		inline KTDebug &operator<<(unsigned long t) 
		{ 
			*streamer << t; 
			return *this; 
		}
		inline KTDebug &operator<<(float t) 
		{ 
			*streamer << t; 
			return *this; 
		}
		inline KTDebug &operator<<(double t) 
		{ 
			*streamer << t; 
			return *this; 
		}
		inline KTDebug &operator<<(const char* t) 
		{ 
			*streamer << t; 
			return *this; 
		}
		KTDebug& operator<<( const QPixmap& p );
		KTDebug& operator<<( const QIcon& p );
		KTDebug& operator<<( const QImage& p ) ;
		KTDebug& operator<<( const QDateTime& );
		KTDebug& operator<<( const QDate&     );
		KTDebug& operator<<( const QTime&     );
		KTDebug& operator<<( const QPoint & ) ;
		KTDebug& operator<<( const QPointF & ) ;
		KTDebug& operator<<( const QSize & ) ;
		KTDebug& operator<<( const QRect & ) ;
		KTDebug& operator<<( const QRegion & );
		KTDebug& operator<<( const QStringList & );
		KTDebug& operator<<( const QColor & );
		KTDebug& operator<<( const QPen & );
		KTDebug& operator<<( const QBrush & );
		KTDebug& operator<<( const QVariant & );
		KTDebug& operator << (const QWidget*);
		KTDebug& operator << (const QEvent*);
		
		template <class T> KTDebug& operator << ( const QList<T> &list );
		
	private slots:
		void write();
		
	private:
		DebugType m_type;
		QString m_toWrite;
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
		void resaltWidget(QWidget */*w*/, const QColor &/*color*/ = QColor(Qt::magenta))
		{
		}
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
