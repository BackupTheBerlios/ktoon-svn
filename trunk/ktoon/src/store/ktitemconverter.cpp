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

#include "ktitemconverter.h"

#include "ktrectitem.h"
#include "ktpathitem.h"
#include "ktellipseitem.h"

#include <QBrush>
#include <QPen>

KTItemConverter::KTItemConverter()
{
}


KTItemConverter::~KTItemConverter()
{
}

void KTItemConverter::copyProperties(QGraphicsItem *src, QGraphicsItem *dest)
{
	dest->setMatrix(src->matrix());
	dest->setPos(src->scenePos());
	dest->setFlags(src->flags() );
	
	dest->setSelected(src->isSelected());
	
	
	// Shapes
	QAbstractGraphicsShapeItem *shape =  dynamic_cast<QAbstractGraphicsShapeItem*>(src);
	QAbstractGraphicsShapeItem *shapeDst = qgraphicsitem_cast<QAbstractGraphicsShapeItem*>(dest);
	
	if ( shape && dest )
	{
		QBrush shapeBrush = shape->brush();
		
		if ( shapeBrush.color().isValid() || shapeBrush.gradient() || 
		!shapeBrush.texture().isNull() )
			shapeDst->setBrush( shape->brush() );
		
		shapeDst->setPen(shape->pen() );
	}
}

KTPathItem *KTItemConverter::convertToPath(QGraphicsItem *item)
{
	KTPathItem *path = new KTPathItem( item->parentItem(), 0);
	
	QPainterPath ppath;
	
	switch(item->type() )
	{
		case 2:
		{
			ppath = qgraphicsitem_cast<QGraphicsPathItem *>(item)->path();
		}
		break;
		case 3: // Rect
		{
			ppath.addRect( qgraphicsitem_cast<QGraphicsRectItem *>(item)->rect());
		}
		break;
		case 4: // Ellipse
		{
			ppath.addEllipse( qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect());
		}
		break;
		default:
		{
			ppath = item->shape(); // TODO
		}
		break;
	}
	
	path->setPath(ppath);
	
	KTItemConverter::copyProperties( item, path);
	
	return path;
	
}

KTEllipseItem *KTItemConverter::convertToEllipse(QGraphicsItem *item)
{
	KTEllipseItem *ellipse = new KTEllipseItem(item->parentItem());
	
	switch(item->type() )
	{
		case 2:
		{
			ellipse->setRect(qgraphicsitem_cast<QGraphicsPathItem *>(item)->path().boundingRect());
		}
		break;
		case 4:
		{
			ellipse->setRect(qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect());
		}
		break;
		// TODO
	}
	
	KTItemConverter::copyProperties( item, ellipse);
	
	return ellipse;
}

KTRectItem *KTItemConverter::convertToRect(QGraphicsItem *item)
{
	KTRectItem *rect = new KTRectItem(item->parentItem());
	
	switch(item->type() )
	{
		case 2:
		{
			rect->setRect(qgraphicsitem_cast<QGraphicsPathItem *>(item)->path().boundingRect());
		}
		break;
		case 4:
		{
			rect->setRect(qgraphicsitem_cast<QGraphicsEllipseItem *>(item)->rect());
		}
		break;
		// TODO
	}
	
	KTItemConverter::copyProperties( item, rect);
	
	return rect;
}


