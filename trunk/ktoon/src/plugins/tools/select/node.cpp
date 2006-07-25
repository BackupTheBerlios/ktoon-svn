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

#include <QtDebug>

#define SHOW_VAR(s) qDebug() << #s << " = " << s

Node::Node(TypeNode node,const QPointF & pos, QGraphicsItem * parent,  QGraphicsScene * scene  ) : QGraphicsItem(parent, scene), m_typeNode(node)
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
    qreal adjust = 2;
    QMatrix matrix = parentItem()->sceneMatrix();
    QSizeF size( 8  * 1/matrix.m11(), 8  * 1/matrix.m22());
    QRectF r(QPointF( -size.width()/2, -size.height()/2), size);
    
    return r;
}


QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
	switch (change) 
	{
		case ItemPositionChange:
		{
			switch(m_typeNode)
			{
				case TopLeft:
				{
					QRectF rect = parentItem()->sceneBoundingRect();
					QRectF br =parentItem()->sceneBoundingRect();
					rect.setTopLeft(scenePos());
					float sx = 1, sy = 1;
					sx = static_cast<float>(rect.width()) / static_cast<float>(br.width());
					sy = static_cast<float>(rect.height()) / static_cast<float>(br.height());
					
					
					if(sx > 0 && sy > 0)
					{
						
						parentItem()->translate(pos().x(), pos().y());
						parentItem()->scale(sx, sy);
					}
					break;
				}
				case TopRight:
				{
					QRectF rect = parentItem()->sceneBoundingRect();
					QRectF br =parentItem()->sceneBoundingRect();
					br.moveTopLeft(parentItem()->pos());
					rect.setTopRight(scenePos());
					float sx = 1, sy = 1;
					sx = static_cast<float>(rect.width()) / static_cast<float>(br.width());
					sy = static_cast<float>(rect.height()) / static_cast<float>(br.height());
					if(sx > 0 && sy > 0)
					{
						parentItem()->translate(0, pos().y());
						parentItem()->scale(sx, sy);
					}
					break;
				}
				case BottomRight:
				{
					QRectF rect = parentItem()->sceneBoundingRect();
					
					QRectF br =parentItem()->sceneBoundingRect();
					rect.setBottomRight(scenePos());
					rect.translate ( scenePos());
					float sx = 1, sy = 1;
					sx = static_cast<float>(rect.width()) / static_cast<float>(br.width());
					sy = static_cast<float>(rect.height()) / static_cast<float>(br.height());
					if(sx > 0 && sy > 0)
					{
						parentItem()->scale(sx, sy);
					}
					break;
				}
				case BottomLeft:
				{
					QRectF rect = parentItem()->sceneBoundingRect();
					QRectF br =parentItem()->sceneBoundingRect();
					br.moveTopLeft(parentItem()->pos());
					
					rect.setBottomLeft(scenePos());
					float sx = 1, sy = 1;
					sx = static_cast<float>(rect.width()) / static_cast<float>(br.width());
					sy = static_cast<float>(rect.height()) / static_cast<float>(br.height());
					if(sx > 0 && sy > 0)
					{
						parentItem()->translate(pos().x(), 0);
						parentItem()->scale(sx, sy);
					}
					break;
					
					
				}
				case Center:
				{
					break;
				}
			};
			break;
		}
		case QGraphicsItem::ItemMatrixChange:
		{
			break;
		}
		
		default:
			break;
	};
	return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{

	update();
	QGraphicsItem::mouseReleaseEvent(event);
	
}

void Node::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	moveBy(  event->pos().x(), event->pos().y() );
	update();
// 	QGraphicsItem::mouseMoveEvent(event);
}


int Node::typeNode() const
{
	return m_typeNode;
}


