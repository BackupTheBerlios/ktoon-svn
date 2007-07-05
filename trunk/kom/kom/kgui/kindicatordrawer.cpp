/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
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

#include "kindicatordrawer.h"
#include "kcore/kdebug.h"

#include <QRadialGradient>

#include <cmath> //abs


struct DIndicatorDrawer::Private
{
	int face;
};

DIndicatorDrawer::DIndicatorDrawer(): d(new Private)
{
	d->face = 1;
}


DIndicatorDrawer::~DIndicatorDrawer()
{
	delete d;
}



void DIndicatorDrawer::paint(QPainter *painter, const QRectF & rect, const QColor &color, const QColor &fg)
{
	painter->save();
	
	painter->scale(rect.width()/54, rect.height()/54);
	painter->setPen(Qt::NoPen);
	
	int r = color.red();
	int g = color.green();
	int b = color.blue();
	
	for(int i = 0; i < 8; i++)
	{
		painter->save();
		
		int alpha = 31*(8-(std::abs(8+d->face-i)%8));
		
		QColor bg(r, g, b, alpha);
		
		QRectF rect(12,0,15,15);
		
		QRadialGradient grad(rect.center(), 15.0/2.0, rect.topLeft());
		
		QColor w = fg;
		w.setAlpha(alpha);
		
		grad.setColorAt(0.0, w);
		grad.setColorAt(1.0, bg);
		
		
		painter->setBrush(grad);
		
// 		painter->setBrush(QBrush(bg, Qt::DiagCrossPattern));
		
		
		painter->translate(27,27);
		painter->rotate(45*i);
		
		painter->drawEllipse(rect);
		
		painter->restore();
	}
	
	painter->restore();
}

void DIndicatorDrawer::advance()
{
	d->face = (d->face+1) % 8;
}


