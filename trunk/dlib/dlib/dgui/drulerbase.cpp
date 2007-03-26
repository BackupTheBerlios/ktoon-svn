/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#include "drulerbase.h"
#include "ddebug.h"

#include <QMouseEvent>
#include <QFrame>
#include <QResizeEvent>
#include <QPaintEvent> 
#include <QPainter>

DRulerBase::DRulerBase(Qt::Orientation m_orientation, QWidget *parent, const char *name) : QFrame(parent), m_position(0), m_orientation(m_orientation), m_drawPointer(false), m_separation(100), m_zero(0,0), m_pArrow(3)
{
	setObjectName(name);
	
	if(m_orientation == Qt::Horizontal)
	{
		setMaximumHeight(10);
		setMinimumHeight(20);
		
		m_width = width();
		m_height = height();
		
		m_pArrow << QPointF(0.0, 0.0);
		m_pArrow << QPointF(5.0,  5.0);
		m_pArrow << QPointF(10.0, 0.0);
		
		m_pArrow.translate(0,10);
	}
	else
	{
		setMaximumWidth(20);
		setMinimumWidth(20);
		
		
		m_width = height();
		m_height =  width();
		
		m_pArrow << QPointF(0.0, 0.0);
		m_pArrow << QPointF(5.0, 5.0);
		m_pArrow << QPointF(0.0, 10.0);
		
		m_pArrow.translate(10,0);
	}
	
	
	setMouseTracking ( true );
	
	connect(this, SIGNAL(displayMenu(DRulerBase *, QPoint)), this, SLOT(showMenu(DRulerBase *, QPoint)));
	
	m_menu = new QMenu(this);
	
	QAction *to5 = m_menu->addAction( tr("Change scale to 5..."));
	QAction *to10 = m_menu->addAction( tr("Change scale to 10..."));
	
	connect(to5, SIGNAL(triggered()), this, SLOT(changeScaleTo5pts()));
	connect(to10, SIGNAL(triggered()), this, SLOT(changeScaleTo10pts()));
}


DRulerBase::~DRulerBase()
{
// 	delete m_pArrow;
// 	delete m_pScale;
}

void DRulerBase::paintEvent ( QPaintEvent * )
{
	QPainter p(this);

//  	p.setRenderHint( QPainter::TextAntialiasing/*QPainter::Antialiasing*/);
// 	m_pScale

	
	drawScale(&p);
// 	p.drawImage(QPoint(0, 0), m_pScale);
	p.setBrush(palette ().color(QPalette::Foreground));
	p.drawConvexPolygon(m_pArrow);
	p.end();
	
}

void DRulerBase::drawScale(QPainter *painter)
{
	painter->save();
	QFontMetrics fm(font());
	
// 	m_pScale = QImage(width(), height(), QImage::Format_RGB32);
	QPalette m_palette = palette();
	QColor aColor = m_palette.color( QPalette::Background);
	
// 	m_pScale.fill(aColor.rgb ());
	int fact = 1;
	int init;
	if(m_orientation == Qt::Horizontal)
	{
		painter->translate(m_zero.x(), 0);
		init = m_zero.x();
		
		painter->drawLine(0,height()-1, width(),height()-1 );
	}
	else
	{
		
		painter->drawLine(width()-1, 0, width()-1, height());
		fact = -1;
		painter->translate(0, m_zero.y());
		painter->rotate(90);
		init = m_zero.y();
		painter->drawLine(0,height()-1, width(),height()-1 );
		
	}
	
	for(int i = 0; i < m_width; i +=10)
	{
		QSize sizeFont = fm.size (Qt::TextSingleLine, QString::number(i));
		if( i % 100  == 0 )
		{
			painter->drawLine ( i, m_height*fact, i, 0 );
			if(m_orientation == Qt::Vertical)
			{
				painter->drawText( QPoint(i, m_height/2 -sizeFont.height()), QString::number(i));
			}
			else
			{
				painter->drawText( QPoint(i, m_height/2), QString::number(i));
			}
		}
		else
		{
			painter->drawLine ( i, m_height*fact, i, m_height*fact - m_height/4*fact );
		}
	}
	
	
	for(int i = init; i > 0 ; i -=10)
	{
		QSize sizeFont = fm.size (Qt::TextSingleLine, QString::number(i));
		if( i % 100  == 0 )
		{
			painter->drawLine ( -i, m_height*fact, -i, 0 );
			if(m_orientation == Qt::Vertical)
			{
				painter->drawText( QPoint(-i, m_height/2 -sizeFont.height()), QString::number(-i));
			}
			else
			{
				
				painter->drawText( -i, m_height/2,  QString::number(-i));
			}
		}
		else
		{
			painter->drawLine ( -i, m_height*fact, -i, m_height*fact - m_height/4*fact );
		}
	}
	
// 	QPainter p(&m_pScale);
// 	p.setRenderHint(QPainter::Antialiasing, true);
// 	p.setPen(palette().color(QPalette::Foreground));
	
// 	m_path = QPainterPath();
	painter->restore();
}

void DRulerBase::resizeEvent ( QResizeEvent * )
{
	if(m_orientation == Qt::Horizontal)
	{
		m_width = width();
		m_height = height();
	}
	else if(m_orientation == Qt::Vertical)
	{
		m_width = height();
		m_height =  width();
	}

	update();
}

void DRulerBase::mouseMoveEvent ( QMouseEvent * e )
{
	if ( m_drawPointer )
	{
		movePointers(e->pos()/*-QPoint(m_zero, m_zero)*/);
	}
}

// const double DRulerBase::unitGetRatioFromIndex(const int index)
// {
// 	//PT, MM, IN, P, CM, C (Cicero)
// 	//NOTE: Calling functions that divide by this value will crash on divide by 0. They shouldnt be getting
// 	// a zero value if they are accessing here with a correct index.
// 	if (index>UNITCOUNT) 
// 		return 0;
// 	double ratio[] = { 1.0, 25.4/72.0, 1.0/72.0, 1.0/12.0, 2.54/72.0, 25.4/72.0/4.512 };
// 	return ratio[index];
// }
// 
// const double DRulerBase::pts2mm(double pts)
// {
// 	return pts * unitGetRatioFromIndex(SC_MM);
// }
// 
// const double DRulerBase::mm2pts(double mm)
// {
// 	return mm / unitGetRatioFromIndex(SC_MM);
// }

void DRulerBase::setDrawPointer(bool yes)
{
	m_drawPointer = yes;
	update();
}

void DRulerBase::setSeparation(int sep)
{
	if ( sep > 0 && sep <= 10000 )
	{
		m_separation = sep;
// 		drawScale();
		update();
	}
	else
	{
		dError() << "I can't assign separation : " << sep << endl;
	}
}

void DRulerBase::mousePressEvent (QMouseEvent *e)
{
	if ( e->button() == Qt::RightButton )
	{
		emit displayMenu(this, e->globalPos()  );
	}
}

Qt::Orientation DRulerBase::orientation()
{
	return m_orientation;
}

void DRulerBase::showMenu(DRulerBase *ruler, QPoint pos)
{
	if(ruler)
	{
		m_menu->popup(pos);
	}
}




void DRulerBase::slide(int value)
{
	if ( m_orientation == Qt::Horizontal )
	{
		move(-value+m_height, pos().y());
	}
	else
	{
		move(pos().x(), -value+m_height);
	}
}

void DRulerBase::setZeroAt(const QPointF & pos)
{
	m_zero = pos;
	
	update();
}

QSize DRulerBase::sizeHint() const
{
	if ( m_orientation == Qt::Horizontal )
	{
		return QSize(m_width/3, height());
	}
	
	return QSize( width(), m_height/3 );
}

void DRulerBase::changeScaleTo5pts()
{
	setSeparation( 5 );
}

void DRulerBase::changeScaleTo10pts()
{
	setSeparation( 10 );
}



