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

#include "ktpathitem.h"
#include <QGraphicsSceneDragDropEvent>
#include <QMimeData>
#include <QBrush>
#include <ddebug.h>
#include <QPainter>
#include <QPainterPath>
#include "ktsvg2qt.h"
#include "ktgraphicalgorithm.h"
#include "ktserializer.h"

KTPathItem::KTPathItem( QGraphicsItem * parent, QGraphicsScene * scene) : QGraphicsPathItem(parent, scene), m_dragOver(false)
{
	setAcceptDrops(true);
// 	setFlags (QGraphicsItem::ItemIsSelectable | QGraphicsItem::ItemIsMovable );
}


KTPathItem::~KTPathItem()
{
}


void KTPathItem::fromXml(const QString &xml)
{
}


QDomElement KTPathItem::toXml(QDomDocument &doc)
{
	QDomElement root = doc.createElement("path");
	
	QString strPath = "";
	QChar t;
	
	for(int i = 0; i <  path().elementCount () ; i++)
	{
		QPainterPath::Element e = path().elementAt (i);
		switch(e.type )
		{
			case QPainterPath::MoveToElement:
			{
				if(t != 'M')
				{
					t = 'M';
					strPath += "M " + QString::number(e.x) + " " + QString::number(e.y) + " ";
				}
				else
				{
					strPath += QString::number(e.x) + " " + QString::number(e.y) + " ";
				}
				
			}
			break;
			case QPainterPath::LineToElement:
			{
				if(t != 'L')
				{
					t = 'L';
					strPath += " L " + QString::number(e.x) + " " + QString::number(e.y) + " ";
				}
				else
				{
					strPath += QString::number(e.x) + " " + QString::number(e.y) + " ";
				}
			}
			break;
			case QPainterPath::CurveToElement:
			{
				
				if(t != 'C')
				{
					t = 'C';
					strPath += " C " + QString::number(e.x) + " " + QString::number(e.y) + " ";
				}
				else
				{
					strPath += "  "+ QString::number(e.x) + " " + QString::number(e.y) + " ";
				}
			}
			break;
			case QPainterPath::CurveToDataElement:
			{
				if ( t == 'C' )
					strPath +=  " " +QString::number(e.x) + "  " + QString::number(e.y) + " ";
			}
			break;
		}
	}
	
	root.setAttribute( "d", strPath);
	
	root.appendChild(KTSerializer::properties( this, doc));
	
	QBrush brush = this->brush();
	root.appendChild(KTSerializer::brush(&brush, doc));
	
	QPen pen = this->pen();
	root.appendChild(KTSerializer::pen(&pen, doc));
	
	
	return root;
}

void KTPathItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	QGraphicsPathItem::paint(painter, option,widget );
	
}




bool KTPathItem::contains ( const QPointF & point ) const
{
#if 1
		int thickness = 2;
	QRectF rectS(point-QPointF(thickness/2,thickness/2) , QSizeF(thickness,thickness));
	
	QPolygonF pol = shape().toFillPolygon ();
	foreach(QPointF point, pol)
{
		if(rectS.contains( point))
{
			return true;
}
}
	QPolygonF::iterator it1 = pol.begin() ;
	QPolygonF::iterator it2 = pol.begin()+1;
	
	while(it2 != pol.end())
{
		if(KTGraphicalAlgorithm::intersectLine( (*it1), (*it2), rectS  ))
{
			return true;
}
		++it1;
		++it2;
}
	
	return false;
#else
// 	return QGraphicsPathItem::contains (point );
	return false;
#endif
	
}

void KTPathItem::dragEnterEvent(QGraphicsSceneDragDropEvent *event)
{
	if (event->mimeData()->hasColor() )
	{
		event->setAccepted(true);
		m_dragOver = true;
		update();
	} 
	else
	{
		event->setAccepted(false);
	}
}


void KTPathItem::dragLeaveEvent(QGraphicsSceneDragDropEvent *event)
{
	Q_UNUSED(event);
	m_dragOver = false;
	update();
}


void KTPathItem::dropEvent(QGraphicsSceneDragDropEvent *event)
{
	m_dragOver = false;
	if (event->mimeData()->hasColor())
	{
		setBrush(QBrush(qVariantValue<QColor>(event->mimeData()->colorData())));
	}
	else if (event->mimeData()->hasImage())
	{
		setBrush(QBrush(qVariantValue<QPixmap>(event->mimeData()->imageData())));
	}
	update();
}



