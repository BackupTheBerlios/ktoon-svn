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

struct DRulerBase::Private
{
	int position;
	Qt::Orientation orientation;
	bool drawPointer;
	
	int separation;
	QMenu *menu;
	int width, height;
	
	QPointF zero;
	
	QPolygonF pArrow;
	
	double scaleFactor;
};

DRulerBase::DRulerBase(Qt::Orientation orientation, QWidget *parent) : QFrame(parent), d(new Private)
{
	d->position = 0;
	d->orientation = orientation;
	d->drawPointer = false;
	d->separation = 10;
	
	d->zero = QPointF(0,0);
	d->pArrow = QPolygonF(3);
	
	d->scaleFactor = 1.0;
	
	if(d->orientation == Qt::Horizontal)
	{
		setMaximumHeight(10);
		setMinimumHeight(20);
		
		d->width = width();
		d->height = height();
		
		d->pArrow << QPointF(0.0, 0.0);
		d->pArrow << QPointF(5.0,  5.0);
		d->pArrow << QPointF(10.0, 0.0);
		
		d->pArrow.translate(0, 10);
	}
	else
	{
		setMaximumWidth(20);
		setMinimumWidth(20);
		
		
		d->width = height();
		d->height =  width();
		
		d->pArrow << QPointF(0.0, 0.0);
		d->pArrow << QPointF(5.0, 5.0);
		d->pArrow << QPointF(0.0, 10.0);
		
		d->pArrow.translate(10,0);
	}
	
	
	setMouseTracking ( true );
	
	connect(this, SIGNAL(displayMenu(DRulerBase *, QPoint)), this, SLOT(showMenu(DRulerBase *, QPoint)));
	
	d->menu = new QMenu(this);
	
	QAction *to5 = d->menu->addAction( tr("Change scale to 5..."));
	QAction *to10 = d->menu->addAction( tr("Change scale to 10..."));
	
	connect(to5, SIGNAL(triggered()), this, SLOT(changeScaleTo5pts()));
	connect(to10, SIGNAL(triggered()), this, SLOT(changeScaleTo10pts()));
}


DRulerBase::~DRulerBase()
{
// 	delete d->pArrow;
// 	delete d->pScale;
	
	delete d;
}

void DRulerBase::paintEvent ( QPaintEvent * )
{
	QPainter p(this);
	
	if( d->orientation == Qt::Vertical )
	{
		p.scale(1.0, d->scaleFactor);
	}
	else
	{
		p.scale(d->scaleFactor, 1.0);
	}
	
	drawScale(&p);
	p.setBrush(palette ().color(QPalette::Foreground));
	
	p.save();
	
	p.drawConvexPolygon(d->pArrow);
	p.restore();
	
	p.end();
	
}

void DRulerBase::drawScale(QPainter *painter)
{
	painter->save();
	QFontMetrics fm(font());
	
	int fact = 1;
	int init;
	if(d->orientation == Qt::Horizontal)
	{
		painter->translate(d->zero.x(), 0);
		init = (int)d->zero.x();
		
		painter->drawLine(0,height()-1, width(), height()-1 );
	}
	else
	{
		painter->drawLine(width()-1, 0, width()-1, height());
		fact = -1;
		painter->translate(0, d->zero.y());
		painter->rotate(90);
		init = (int)d->zero.y();
		painter->drawLine(0, height()-1, width(), height()-1 );
	}
	
	for(int i = 0; i < d->width; i += d->separation)
	{
		QSize sizeFont = fm.size (Qt::TextSingleLine, QString::number(i));
		if( i % 100 == 0 ) // FIXME
		{
			painter->drawLine ( i, d->height*fact, i, 0 );
			if(d->orientation == Qt::Vertical)
			{
				painter->drawText( QPoint(i, d->height/2 -sizeFont.height()), QString::number(i));
			}
			else
			{
				painter->drawText( QPoint(i, d->height/2), QString::number(i));
			}
		}
		else
		{
			painter->drawLine ( i, d->height*fact, i, d->height*fact - d->height/4*fact );
		}
	}
	
	
	for(int i = init; i > 0 ; i -= d->separation)
	{
		QSize sizeFont = fm.size (Qt::TextSingleLine, QString::number(i));
		if( i % 100  == 0 ) // FIXME
		{
			painter->drawLine ( -i, d->height*fact, -i, 0 );
			if(d->orientation == Qt::Vertical)
			{
				painter->drawText( QPoint(-i, d->height/2 -sizeFont.height()), QString::number(-i));
			}
			else
			{
				
				painter->drawText( -i, d->height/2,  QString::number(-i));
			}
		}
		else
		{
			painter->drawLine ( -i, d->height*fact, -i, d->height*fact - d->height/4*fact );
		}
	}
	
	painter->restore();
}

void DRulerBase::resizeEvent ( QResizeEvent * )
{
	if(d->orientation == Qt::Horizontal)
	{
		d->width = width();
		d->height = height();
	}
	else if(d->orientation == Qt::Vertical)
	{
		d->width = height();
		d->height =  width();
	}

	update();
}

void DRulerBase::mouseMoveEvent ( QMouseEvent * e )
{
	if ( d->drawPointer )
	{
		movePointers(e->pos()/*-QPoint(d->zero, d->zero)*/);
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
	d->drawPointer = yes;
	update();
}

void DRulerBase::setSeparation(int sep)
{
	if ( sep > 0 && sep <= 10000 )
	{
		d->separation = sep;
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
	return d->orientation;
}

int DRulerBase::separation() const
{
	return d->separation;
}

double DRulerBase::scaleFactor() const
{
	return d->scaleFactor;
}

void DRulerBase::showMenu(DRulerBase *ruler, QPoint pos)
{
	if(ruler)
	{
		d->menu->popup(pos);
	}
}




void DRulerBase::slide(int value)
{
	if ( d->orientation == Qt::Horizontal )
	{
		move(-value+d->height, pos().y());
	}
	else
	{
		move(pos().x(), -value+d->height);
	}
}


QPointF DRulerBase::zero() const
{
	return d->zero;
}

void DRulerBase::translateArrow(double dx, double dy)
{
	d->pArrow.translate(dx, dy);
}

void DRulerBase::setZeroAt(const QPointF & pos)
{
	d->zero = pos;
	update();
}

void DRulerBase::scale(double factor)
{
	d->scaleFactor = factor;
	update();
}

QSize DRulerBase::sizeHint() const
{
	if ( d->orientation == Qt::Horizontal )
	{
		return QSize(d->width/3, height());
	}
	
	return QSize( width(), d->height/3 );
}

void DRulerBase::changeScaleTo5pts()
{
	setSeparation( 5 );
}

void DRulerBase::changeScaleTo10pts()
{
	setSeparation( 10 );
}
