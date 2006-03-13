/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "agraphic.h"
#include "ddebug.h"
#include <QPixmap>

AGraphic::AGraphic()
{
}

AGraphic::AGraphic(const AGraphic &toCopy) : path(toCopy.path), brush(toCopy.brush), pen(toCopy.pen)
{
}

AGraphic::~AGraphic()
{
}

void AGraphic::mapTo(const QMatrix& matrix)
{
	path = matrix.map(path);
	
	brush = DBrushAdjuster::mapBrush( brush, matrix );
	pen.setBrush( DBrushAdjuster::mapBrush( pen.brush(), matrix ));
	
}

void AGraphic::flip(Qt::Orientation o)
{
	QPointF pos = path.boundingRect().center();
	QMatrix  matrix;
	matrix.translate(-pos.x(),-pos.y());
	
	brush = DBrushAdjuster::mapBrush( brush, matrix );
	pen.setBrush( DBrushAdjuster::mapBrush( pen.brush(), matrix ));
	
	QList<QPolygonF> pols = path.toSubpathPolygons(matrix);
	
	QList<QPolygonF>::iterator itPol = pols.begin();
	path = QPainterPath();
	
	QPolygonF result;
	
	while(itPol != pols.end())
	{
		QPolygonF::iterator itPoint = (*itPol).begin();
		while(itPoint != (*itPol).end())
		{
			if ( o == Qt::Horizontal)
			{
				(*itPoint).setY(/*pos.y()*/-(*itPoint).y());
			}
			else
			{
				(*itPoint).setX(-(*itPoint).x());
			}
			result.append(*itPoint);
			++itPoint;
		}
// 		path.addPolygon((*itPol));
// 		path.closeSubpath();
		++itPol;
	}
	
	path.addPolygon(result);
	brush = DBrushAdjuster::flipBrush( brush, o );
	pen.setBrush( DBrushAdjuster::flipBrush( pen.brush(), o ));
	
	matrix.reset();
	matrix.translate(pos.x(), pos.y()/*-path.currentPosition().y()*/);
	brush = DBrushAdjuster::mapBrush( brush, matrix );
	pen.setBrush( DBrushAdjuster::mapBrush( pen.brush(), matrix ));
	path = matrix.map(path);
	
	
	
}




