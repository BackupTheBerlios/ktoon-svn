/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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


#include "dcore/dbrushadjuster.h"
#include "dcore/ddebug.h"

#include <QPixmap>
DBrushAdjuster::DBrushAdjuster()
{
}


DBrushAdjuster::~DBrushAdjuster()
{
}

QBrush DBrushAdjuster::adjustBrush(const QBrush &brush, const QRect &rect )
{
	QBrush brush1(brush);
	if(brush.gradient())
	{
		QGradient grad = DGradientAdjuster::adjustGradient( brush.gradient(), rect );
		brush1 = QBrush(grad);
	}
	else if(!brush.texture().isNull())
	{
		QPixmap p = (brush.texture()/*.toImage ()*/);
		int offset= 0;
		QRect br = p.rect();
		QMatrix matrix;
		
		float sx = 1, sy = 1;
		if ( rect.width() < br.width() )
		{
			sx = static_cast<float>(rect.width()-offset) / static_cast<float>(br.width());
		}
		if ( rect.height() < br.height() )
		{
			sy = static_cast<float>(rect.height()-offset) / static_cast<float>(br.height());
		}
	
		float factor = qMin(sx, sy);
		matrix.scale(sx, sy);
		p = p.transformed(matrix ,Qt::SmoothTransformation );
	
		matrix.reset();
	
		QPointF pos = br.topLeft();
	
		float tx = offset/2-pos.x(), ty = offset/2-pos.y();
	
		matrix.translate(tx, ty);
		p = p.transformed(matrix ,Qt::SmoothTransformation );
		
// 		brush1 = QBrush(p);
		brush1.setTexture (  p );
	}
	return brush1;
}

QBrush DBrushAdjuster::mapBrush(const QBrush &brush, const QMatrix &matrix  )
{
	QBrush brush1(brush);
	if(brush.gradient())
	{
		QGradient grad = DGradientAdjuster::mapGradient( brush.gradient(), matrix );
		brush1 = QBrush(grad);
	}
	else if(!brush.texture().isNull())
	{
		QPixmap p(brush.texture());
		p = p.transformed(matrix , Qt::SmoothTransformation );
		brush1.setTexture( p );
	}
	
	
	return brush1;
}

QBrush DBrushAdjuster::flipBrush(const QBrush &brush, Qt::Orientation o)
{
	QBrush brush1(brush);
	if(brush.gradient())
	{
		QGradient grad = DGradientAdjuster::flipGradient( brush.gradient(), o );
		brush1 = QBrush(grad);
	}
	else if(!brush.texture().isNull())
	{
	}
	return brush1;
}


