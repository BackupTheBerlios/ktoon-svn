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

#if !defined(D_NODEBUG)
#include "ddebug.h"

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

static void DDebutOutput(DebugType t, const char *data)
{
	char *output = "%s\n";
	switch(t)
	{
		case DDebugMsg:
		{
// 			output = "%s\n";
		}
		break;
		case DWarningMsg:
		{
			output = SHOW_WARNING;
		}
		break;
		case DErrorMsg:
		{
			output = SHOW_ERROR;
		}
		break;
		case DFatalMsg:
		{
			output = SHOW_FATAL;
		}
		break;
	}
	
	fprintf(stderr, output, data);
}

DDebug::DDebug(DebugType t) : m_type(t)
{
	streamer = new Streamer();
};

DDebug::DDebug(const DDebug & d ) : streamer(d.streamer), m_type(d.m_type)
{
}

DDebug::~DDebug()
{
	::DDebutOutput( m_type, streamer->buffer.toLocal8Bit().data() );
	delete streamer;
}

DDebug& DDebug::operator<<( const QPixmap& p ) 
{
	*this << "(" << p.width() << ", " << p.height() << ")";
	return *this;
}

DDebug& DDebug::operator<<( const QIcon& p )
{
	*this << "(" << p.pixmap(QSize() ).width() << ", " << p.pixmap(QSize()).height() << ")";
	return *this;
}

DDebug& DDebug::operator<<( const QImage& p ) 
{
	*this << "(" << p.width() << ", " << p.height() << ")";
	return *this;
}

DDebug& DDebug::operator<<( const QDateTime& time) 
{
	*this << time.toString();
	return *this;
}

DDebug& DDebug::operator<<( const QDate& date) 
{
	*this << date.toString();

	return *this;
}

DDebug& DDebug::operator<<( const QTime& time) 
{
	*this << time.toString();
	return *this;
}

DDebug& DDebug::operator<<( const QPoint & p)  
{
	*this << "(" << p.x() << ", " << p.y() << ")";
	return *this;
}

DDebug& DDebug::operator<<( const QPointF & p)  
{
	*this << "(" << p.x() << ", " << p.y() << ")";
	return *this;
}

DDebug& DDebug::operator<<( const QSize & s)  
{
	*this << "[" << s.width() << "x" << s.height() << "]";
	return *this;
}

DDebug& DDebug::operator<<( const QRect & r)  
{
	*this << "[" << r.x() << "," << r.y() << " - " << r.width() << "x" << r.height() << "]";
	return *this;
}

DDebug& DDebug::operator<<( const QRegion & reg) 
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

DDebug& DDebug::operator<<( const QStringList & l) 
{
	*this << "(";
	*this << l.join(",");
	*this << ")";

	return *this;
}

DDebug& DDebug::operator<<( const QColor & c) 
{
	if ( c.isValid() )
		*this << c.name();
	else
		*this << "(invalid/default)";
	return *this;
}

DDebug& DDebug::operator<<( const QPen & p) 
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

DDebug& DDebug::operator<<( const QBrush & b)
{
	if ( b.gradient())
	{
		*this << b.gradient();
	}
	else
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
	}
	return *this;
}

DDebug& DDebug::operator<<( const QVariant & v) 
{
	*this << "[variant: ";
	*this << v.typeName();
	*this << " toString=";
	*this << v.toString();
	*this << "]";
	return *this;
}
// 5580379
DDebug& DDebug::operator << (const QWidget* t) 
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

DDebug& DDebug::operator << (const QEvent* e)
{
	*this << "[Event " << e->type() << "]";
	
	return *this;
}


DDebug& DDebug::operator << (const QLinearGradient &g)
{
	*this << "QLinearGradient start=" << g.start() << " stop="<<g.finalStop();
	return *this;
}

DDebug& DDebug::operator << (const QRadialGradient &g)
{
	*this << "QRadialGradient center=" << g.center() << " focal="<<g.focalPoint() << " radius=" << g.radius();
	return *this;
}

DDebug& DDebug::operator << (const QConicalGradient &g)
{
	*this << "QConicalGradient center=" << g.center() << " angle="<<g.angle();
	return *this;
}

DDebug& DDebug::operator << (const QGradient *g)
{
	switch(g->type())
	{
		case QGradient::LinearGradient:
		{
			*this << static_cast<const QLinearGradient &>(*g);
		}
		break;
		case QGradient::RadialGradient:
		{
			*this << static_cast<const QRadialGradient &>(*g);
		}
		break;
		case QGradient::ConicalGradient:
		{
			*this << static_cast<const QConicalGradient &>(*g);
		}
		break;
		
	}
}

void DDebug::resaltWidget(QWidget *w, const QColor &color)
{
	QPalette pal = w->palette();
	pal.setColor(QPalette::Background, color);
	w->setPalette(pal);
}

#endif // D_NODEBUG

