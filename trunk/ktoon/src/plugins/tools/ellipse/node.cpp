/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado   *
 *   kuadrosx@toonka.com   *
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

#include "node.h"
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QStyleOptionButton>
#include <QApplication>
#include <cmath>
#include <QtDebug>

#define SHOW_VAR(s) qDebug() << #s << " = " << s

Node::Node(TypeNode node,const QPointF & pos, QGraphicsItem * parent,  QGraphicsScene * scene  ) : QGraphicsItem(parent, scene), m_typeNode(node), m_notChange(true)
{
	QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor ));
	setFlag(ItemIsMovable);
	setPos(pos);
}


Node::~Node()
{
}


void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
	Q_UNUSED(w);
	if (option->state & QStyle::State_Sunken)
	{
		QColor c("green");
		c.setAlpha(150);
		painter->setBrush( c );
	} 
	else
	{
		QColor c("navy");
		c.setAlpha(150);
		painter->setBrush( c );
	}
	painter->drawRect(boundingRect());
}

QRectF Node::boundingRect() const
{
// 	qreal adjust = 2;
	QMatrix matrix = parentItem()->sceneMatrix();
	QSizeF size( 8  * 1/matrix.m11(), 8  * 1/matrix.m22());
	QRectF r(QPointF( -size.width()/2, -size.height()/2), size);
	//     QRectF r (QPointF(0,0), QSizeF( 5/2, 5/2));
	return r;
}


QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
	
	return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	m_brParent = parentItem()->sceneBoundingRect();
// 	QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update();
// 	QGraphicsItem::mouseReleaseEvent(event);
}

void Node::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	QPointF newPos(mapToParent(event->pos()) - matrix().map(event->buttonDownPos(Qt::LeftButton)));
	if( m_notChange)
	{
		m_notChange = false;
	}
	else
	{
		QRectF rect = m_brParent;
// 		QRectF br = m_brParent;
		
		QGraphicsEllipseItem *item = dynamic_cast< QGraphicsEllipseItem*>( parentItem());
		
		if(item)
		{
			
			QPointF center = rect.center();
			QPointF tal= event->scenePos() - center;
// 			QGraphicsItem * parent = parentItem();
			double a =  atan(tal.y() / tal.x())*(180/3.1416);
			SHOW_VAR(a);
			switch(m_typeNode)
			{
				case StartAngle:
				{
					dynamic_cast< QGraphicsEllipseItem*>( parentItem())->setStartAngle (360+ a* 16 );
					break;
				}
				case SpanAngle:
				{
					dynamic_cast< QGraphicsEllipseItem*>( parentItem())->setStartAngle ( a* 16 );
					break;
				}
						
	// 					TypeNode
			};
		}
	}
	update();
}


int Node::typeNode() const
{
	return m_typeNode;
}


