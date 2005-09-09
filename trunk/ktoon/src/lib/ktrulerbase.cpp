/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosxx@toonka.com                                                   *
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

#include "ktrulerbase.h"
#include "ktdebug.h"

#include <qpainter.h>
#include <qpixmap.h>

KTRulerBase::KTRulerBase(Orientation orientation, QWidget *parent, const char *name)
	: QFrame(parent, name), m_position(0), m_orientation(orientation),m_pArrow(3), m_drawPointer(false), m_separation(100)
{
	m_pScale.setOptimization( QPixmap::BestOptim );
	m_pScale.resize( width(),height());
	m_pScale.fill(paletteBackgroundColor ());
	
	if(m_orientation == Horizontal)
	{
// 		m_pArrow.setPoints(3, 20, 0, 0, 0+2, 0, 0-2);
// 		m_pArrow.setPoints(3,9,0,0,5,10,10,0);
		m_pArrow.setPoint ( 0, 0, 0);
		m_pArrow.setPoint ( 1, 5, 10);
		m_pArrow.setPoint ( 2, 10, 0);
		setMaximumHeight ( 25 );
		setMinimumHeight (25);
	}
	else if(m_orientation == Vertical)
	{
		m_pArrow.setPoint ( 0, 0, 0);
		m_pArrow.setPoint ( 1, 10, 5);
		m_pArrow.setPoint ( 2, 0, 10);
		setMaximumWidth(25);
		setMinimumWidth(25);
	}
	//draw scale
	drawScale();
	setMouseTracking ( true );
	
	connect(this, SIGNAL(displayMenu(KTRulerBase *, QPoint)), this, SLOT(showMenu(KTRulerBase *, QPoint)));
	
	m_menu = new QPopupMenu(this);
	m_menu->insertItem( tr("Change scale to 5..."), ChangeScaleToFive);
	m_menu->insertItem( tr("Change scale to 10..."), ChangeScaleToTen);
	
	connect(m_menu, SIGNAL(activated(int)), this, SLOT(chooseOption(int)));
}


KTRulerBase::~KTRulerBase()
{
// 	delete m_pArrow;
// 	delete m_pScale;
}

void KTRulerBase::paintEvent ( QPaintEvent * e)
{
	QPainter p(this);
	p.drawPixmap ( 0, 0, m_pScale);
// 	p.drawPolygon (m_pArrow,true,0,3);
	if ( m_drawPointer )
	{
		p.setPen(foregroundColor ());
		p.setBrush(foregroundColor () );
		p.drawConvexPolygon(m_pArrow);//,true,0,3);
	}
// 	p.drawCubicBezier (m_pArrow) ;
	p.end();
}

void KTRulerBase::drawScale()
{
// 	ktDebug() << "Drawing Scale" << endl;
	m_pScale.fill(paletteBackgroundColor ());

	QPainter p(&m_pScale,this);
	QWMatrix m;
	m.rotate( 0.0 );
	p.setWorldMatrix( m );
	
	int range = 10;
	int advance = 1;
		
	if (m_separation > 10)
	{
		range = 1;
		advance = 10;
	}
	
	if(m_orientation == Horizontal)
	{
		p.drawLine( 0, height()-1, width(), height()-1);
		
		for( int i =0; i < width(); i += advance )
		{
			if( i % m_separation == 0 )
			{
// 				qDebug(QString::number(i));
				p.drawLine ( i*range, height(), i*range, 0 );
				p.drawText ( (i*range)+1, height()/2, QString::number(i));
				//, -1, TextDirection dir = Auto );
			}
			else
			{
				p.drawLine ( i*range, height(), i*range, height()-10 );
			}
		}
	}
	else if(m_orientation == Vertical)
	{
		p.drawLine( width()-1, 0, width()-1, height());
		for(int i =0; i < height();i += advance)
		{
			if( i % m_separation == 0 )
			{
// 				qDebug(QString::number(i));
				p.drawLine ( width(), i*range, 0, i*range );
				p.save();
// 				p.rotate(40);
				p.translate( 0,i*range);
				p.rotate(90);
				p.drawText ( 1, 0, QString::number(i));
				p.restore();
			}
			else
			{
				p.drawLine ( width() , i*range, width()-10, i*range );
			}
		}
	}
	p.end();
	
	update();
}

void KTRulerBase::resizeEvent ( QResizeEvent * e)
{
	m_pScale.resize( width(), height());
	drawScale();
}

void KTRulerBase::mouseMoveEvent ( QMouseEvent * e )
{
	if ( m_drawPointer )
	{
		movePointers(e->pos());
	}
}

// const double KTRulerBase::unitGetRatioFromIndex(const int index)
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
// const double KTRulerBase::pts2mm(double pts)
// {
// 	return pts * unitGetRatioFromIndex(SC_MM);
// }
// 
// const double KTRulerBase::mm2pts(double mm)
// {
// 	return mm / unitGetRatioFromIndex(SC_MM);
// }

void KTRulerBase::setDrawPointer(bool yes)
{
	m_drawPointer = yes;
	repaint(false);
}

void KTRulerBase::setSeparation(int sep)
{
	if ( sep > 0 && sep <= 10000 )
	{
		m_separation = sep;
		drawScale();
	}
	else
	{
		ktError() << "I can't assign separation : " << sep << endl;
	}
}

void KTRulerBase::mousePressEvent (QMouseEvent *e)
{
	if ( e->button() == Qt::RightButton )
	{
		emit displayMenu(this, e->globalPos()  );
	}
}

int KTRulerBase::orientation()
{
	return m_orientation;
}

void KTRulerBase::showMenu(KTRulerBase *ruler, QPoint pos)
{
	if(ruler)
	{
		m_menu->popup(pos);
	}
}


void KTRulerBase::chooseOption(int opt)
{
	switch(opt)
	{
		case ChangeScaleToFive:
		{
			setSeparation(5);
		}
		break;
		case ChangeScaleToTen:
		{
			setSeparation(10);
		}
		break;
		default:
		{
		}
		break;
	}
}


