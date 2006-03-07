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

DRulerBase::DRulerBase(Qt::Orientation m_orientation, QWidget *parent, const char *name) : QFrame(parent), m_position(0), m_orientation(m_orientation), m_drawPointer(false), m_separation(100), m_zero(0), m_pArrow(3)
{
	setObjectName(name);
	
	if(m_orientation == Qt::Horizontal)
	{
		m_pArrow.setPoint ( 0, 0, 0);
		m_pArrow.setPoint ( 1, 5, 10);
		m_pArrow.setPoint ( 2, 10, 0);
		setMaximumHeight ( 25 );
		setMinimumHeight ( 25 );
		
		m_width = width();
		m_height = height();
	}
	else if(m_orientation == Qt::Vertical)
	{
		m_pArrow.setPoint ( 0, 0, 0);
		m_pArrow.setPoint ( 1, 10, 5);
		m_pArrow.setPoint ( 2, 0, 10);
		setMaximumWidth(25);
		setMinimumWidth(25);
		
// 		setMinimumHeight ( parent->height()+1000 );
		
		m_width = height();
		m_height =  width();
	}
	
	m_pScale = QImage(m_width, m_height, QImage::Format_RGB32);
	m_pScale.fill(qRgb(255, 255, 255));
	
	//draw scale
	drawScale();
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

	p.drawImage(QPoint(0, 0), m_pScale);
	p.setBrush(palette ().color(QPalette::Foreground));
	p.drawConvexPolygon(m_pArrow);
	p.end();
}

void DRulerBase::drawScale()
{
	QFontMetrics fm(font());
	
	m_pScale = QImage(width(), height(), QImage::Format_RGB32);
	QPalette m_palette = palette ();
	QColor aColor = m_palette.color(  QPalette::Background);
	
	m_pScale.fill(aColor.rgb ());
	
	int fact = 1;
	if( m_orientation == Qt::Vertical )
	{
		fact = -1;
	}
	drawLine( 0-m_zero, 0, m_width, 0);
	drawLine( 0-m_zero, m_height*fact, m_width, m_height*fact);
	
	for(int i = 0; i < m_width; i +=10)
	{
		QSize sizeFont = fm.size (Qt::TextSingleLine, QString::number(i));
		if( i % 100  == 0 )
		{
			drawLine ( i, m_height*fact, i, 0 );
			if(m_orientation == Qt::Vertical)
			{
				m_path.addText( i, m_height/2 -sizeFont.height()  , font(), QString::number(i));
			}
			else
			{
				m_path.addText( i, m_height/2, font(), QString::number(i));
			}
		}
		else
		{
			drawLine ( i, m_height*fact, i, m_height*fact - m_height/4*fact );
		}
	}
	
	for(int i = m_zero; i > 0 ; i -=10)
	{
		QSize sizeFont = fm.size (Qt::TextSingleLine, QString::number(i));
		if( i % 100  == 0 )
		{
			drawLine ( -i, m_height*fact, -i, 0 );
			if(m_orientation == Qt::Vertical)
			{
				m_path.addText( -i, m_height/2 -sizeFont.height()  , font(), QString::number(-i));
			}
			else
			{
				m_path.addText( -i, m_height/2, font(), QString::number(-i));
			}
		}
		else
		{
			drawLine ( -i, m_height*fact, -i, m_height*fact - m_height/4*fact );
		}
	}
	
	QPainter p(&m_pScale);
	p.setRenderHint(QPainter::Antialiasing, true);
	p.setPen(palette().color(QPalette::Foreground));
	
	if(m_orientation == Qt::Horizontal)
	{
		p.translate(m_zero,0);
		p.drawPath(m_path);
	}
	else
	{
		
		p.save();
		p.translate(0,m_zero);
		p.rotate(90);
		p.drawPath(m_path);
		p.restore();
	}
	
	m_path = QPainterPath();
}

void DRulerBase::resizeEvent ( QResizeEvent * )
{
	if(m_orientation == Qt::Horizontal)
	{
		m_width = width();
		m_height = height();
// 		m_pScale = QImage(m_width, m_height, QImage::Format_RGB32);
	}
	else if(m_orientation == Qt::Vertical)
	{
		m_width = height();
		m_height =  width();
// 		m_pScale = QImage(m_height, m_width , QImage::Format_RGB32);
	}

	drawScale();
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
		drawScale();
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



void DRulerBase::drawLine(int x1 , int y1, int x2, int y2)
{
	m_path.moveTo(x1, y1);
	m_path.lineTo(x2, y2);
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

void DRulerBase::setZeroAt(int pos)
{
	m_zero = pos;
	drawScale();
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



