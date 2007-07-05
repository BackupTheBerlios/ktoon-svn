/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#include "kpathhelper.h"
#include "kcore/kdebug.h"

DPathHelper::DPathHelper()
{
}


DPathHelper::~DPathHelper()
{
}

QPainterPath DPathHelper::toRect(const QPainterPath &p, const QRect &rect, float offset)
{
	QPainterPath path;
	
	QRectF br = p.boundingRect();
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
	matrix.scale(factor, factor);
	path = matrix.map(p);
	
	matrix.reset();
	
	QPointF pos = path.boundingRect().topLeft();
	
	float tx = offset/2-pos.x(), ty = offset/2-pos.y();
	
	matrix.translate(tx, ty);
	return matrix.map(path);
}

QList<QPainterPath> DPathHelper::toRect(const QList<QPainterPath> &l, const QRect &rect, float offset)
{
	QList<QPainterPath> returnList;
	QRectF br;
	
	foreach(QPainterPath in, l)
	{
		br = br | in.boundingRect().toRect();
	}

	
	foreach(QPainterPath path, l)
	{
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
		matrix.scale(factor, factor);
		path = matrix.map(path);
		
		matrix.reset();
		
		QPointF pos = path.boundingRect().topLeft();
		
		float tx = offset/2-pos.x(), ty = offset/2-pos.y();
		
		matrix.translate(tx, ty);
		returnList << matrix.map(path);
	}
	
	return returnList;
}

QPainterPath DPathHelper::buildPath(const QStringList &polygonsStr, QChar sep)
{
	// ###: Not work for curves
	QPainterPath path;
	
	foreach (QString polTmp, polygonsStr)
	{
		QStringList points = polTmp.trimmed().split(' ');
		
		QPolygonF polygon;
		
		foreach(QString p, points)
		{
			bool valid = false;
			double x = p.section(sep, 0, 0).toDouble(&valid);
			double y = p.section(sep, 1, 1).toDouble(&valid);
			
			if ( valid )
			{
				polygon << QPointF(x, y);
			}
		}
		
		path.addPolygon(polygon);

	}
	return path;
}

QPainterPath DPathHelper::buildPath(const QString &svgpath)
{
	dWarning() << QObject::tr("Not implemented yet.");
	
	return QPainterPath();
}


QPainterPath DPathHelper::fromElements(const QList<QPainterPath::Element>& elements)
{
	QPainterPath shape;
	QVector<QPointF> curve;
	
	foreach(QPainterPath::Element e, elements)
	{
		switch(e.type)
		{
			case QPainterPath::MoveToElement:
			{
				shape.moveTo(e.x, e.y);
				break;
			}
			case QPainterPath::LineToElement:
			{
				shape.lineTo(e.x, e.y);
				break;
			}
			case QPainterPath::CurveToDataElement:
			{
				curve << e;
				if(curve.count() == 3)
				{
					shape.cubicTo(curve[0], curve[1], curve[2]);
				}
				break;
			}
			case QPainterPath::CurveToElement:
			{
				curve.clear();
				curve << e;
				break;
			}
		}
	}
	return shape;
}

QList<QPainterPath> DPathHelper::toSubpaths(const QPainterPath & path )
{
	QList<QPainterPath> paths;
	QList<QPainterPath::Element> elements;
	
	for(int index = 0; index < path.elementCount(); index++ )
	{
		QPainterPath::Element e = path.elementAt(index);
		
		if(e.type == QPainterPath::MoveToElement && !elements.isEmpty())
		{
			QPainterPath path = DPathHelper::fromElements(elements);
			paths << path;
			elements.clear();
		}
		
		elements << e;
	}
	
	if( ! elements.isEmpty() )
	{
		QPainterPath path = DPathHelper::fromElements(elements);
		paths << path;
		elements.clear();
	}
	
	return paths;
}


