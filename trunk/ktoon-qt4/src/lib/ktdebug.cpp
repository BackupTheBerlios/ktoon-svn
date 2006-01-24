/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#if !defined(KT_NODEBUG)
#include "ktdebug.h"

#include <QFile>
#include <QString>
#include <QDateTime>
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QRegion>
#include <QStringList>
#include <QPen>
#include <QBrush>
#include <QSize>
#include <QVariant>
#include <QImage>
#include <QIcon>
#include <QPixmap>
#include <QWidget>
#include <QEvent>
#include <QTimer>

#if defined(Q_OS_UNIX)
# define SHOW_ERROR "*** \033[0;31m%s\033[0;0m ***\n"
# define SHOW_WARNING "-> \033[10;33m%s\033[0;0m\n"
# define SHOW_FATAL "***** \033[0;35m%s\033[0;0m *****\n"
#else
# define SHOW_ERROR "*** %s ***\n"
# define SHOW_WARNING "-> %s\n"
# define SHOW_FATAL "***** %s *****\n"
#endif

static void KTDebutOutput(DebugType t, const char *data)
{
	char *output = "%s\n";
	switch(t)
	{
		case KTDebugMsg:
		{
// 			output = "%s\n";
		}
		break;
		case KTWarningMsg:
		{
			output = SHOW_WARNING;
		}
		break;
		case KTErrorMsg:
		{
			output = SHOW_ERROR;
		}
		break;
		case KTFatalMsg:
		{
			output = SHOW_FATAL;
		}
		break;
	}
	
	fprintf(stderr, output, data);
}

KTDebug::KTDebug(DebugType t) : m_type(t)
{
	streamer = new Streamer();
};

KTDebug::KTDebug(const KTDebug & d ) : streamer(d.streamer), m_type(d.m_type)
{
}

KTDebug::~KTDebug()
{
	::KTDebutOutput( m_type, streamer->buffer.toLocal8Bit().data() );
	delete streamer;
}

KTDebug& KTDebug::operator<<( const QPixmap& p ) 
{
	*this << "(" << p.width() << ", " << p.height() << ")";
	return *this;
}

KTDebug& KTDebug::operator<<( const QIcon& p )
{
	*this << "(" << p.pixmap(QSize() ).width() << ", " << p.pixmap(QSize()).height() << ")";
	return *this;
}

KTDebug& KTDebug::operator<<( const QImage& p ) 
{
	*this << "(" << p.width() << ", " << p.height() << ")";
	return *this;
}

KTDebug& KTDebug::operator<<( const QDateTime& time) 
{
	*this << time.toString();
	return *this;
}

KTDebug& KTDebug::operator<<( const QDate& date) 
{
	*this << date.toString();

	return *this;
}

KTDebug& KTDebug::operator<<( const QTime& time) 
{
	*this << time.toString();
	return *this;
}

KTDebug& KTDebug::operator<<( const QPoint & p)  
{
	*this << "(" << p.x() << ", " << p.y() << ")";
	return *this;
}

KTDebug& KTDebug::operator<<( const QPointF & p)  
{
	*this << "(" << p.x() << ", " << p.y() << ")";
	return *this;
}

KTDebug& KTDebug::operator<<( const QSize & s)  
{
	*this << "[" << s.width() << "x" << s.height() << "]";
	return *this;
}

KTDebug& KTDebug::operator<<( const QRect & r)  
{
	*this << "[" << r.x() << "," << r.y() << " - " << r.width() << "x" << r.height() << "]";
	return *this;
}

KTDebug& KTDebug::operator<<( const QRegion & reg) 
{
	*this<< "[ ";

	QVector<QRect> rs = reg.rects();
	for (int i=0;i<rs.size();++i)
	{
		*this << QString("[%1,%2 - %3x%4] ").arg(rs[i].x()).arg(rs[i].y()).arg(rs[i].width()).arg(rs[i].height() ) ;
	}

	*this <<"]";
	return *this;
}

KTDebug& KTDebug::operator<<( const QStringList & l) 
{
	*this << "(";
	*this << l.join(",");
	*this << ")";

	return *this;
}

KTDebug& KTDebug::operator<<( const QColor & c) 
{
	if ( c.isValid() )
		*this << c.name();
	else
		*this << "(invalid/default)";
	return *this;
}

KTDebug& KTDebug::operator<<( const QPen & p) 
{
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

KTDebug& KTDebug::operator<<( const QBrush & b)
{
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
		if ( ! b.texture().isNull() )
			*this <<" has a texture";
		*this <<" ]";
		return *this;
}

KTDebug& KTDebug::operator<<( const QVariant & v) 
{
	*this << "[variant: ";
	*this << v.typeName();
	*this << " toString=";
	*this << v.toString();
	*this << "]";
	return *this;
}
// 5580379
KTDebug& KTDebug::operator << (const QWidget* t) 
{
	if ( t )
	{
		*this << "[Widget geometry: " << t->width() << "x"<< t->height() << "]";
	}
	else
	{
		*this << "[Null Widget]";
	}
	return *this; 
}

KTDebug& KTDebug::operator << (const QEvent* e)
{
	*this << "[Event " << e->type() << "]";
	
	return *this;
}

void KTDebug::resaltWidget(QWidget *w, const QColor &color)
{
	QPalette pal = w->palette();
	pal.setColor(QPalette::Background, color);
	w->setPalette(pal);
}

#endif // KT_NODEBUG

