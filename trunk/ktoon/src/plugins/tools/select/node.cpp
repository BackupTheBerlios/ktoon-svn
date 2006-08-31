/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
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

#include "node.h"
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QStyleOptionButton>
#include <QApplication>

// #include <QtDebug>
#include <QCursor>
// #define SHOW_VAR(s) qDebug() << #s << " = " << s
#include <ddebug.h>
#include "nodemanager.h"

Node::Node(TypeNode node, const QPointF & pos, NodeManager *manager, QGraphicsItem * parent,  QGraphicsScene * scene   ) : QGraphicsItem(0, scene), m_typeNode(node), m_notChange(true), m_parent(parent), m_manager(manager), gb1(0), gb2(0)
{
	QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor ));
	
	setFlag(ItemIsSelectable, false);
	setFlag(ItemIsMovable, true);
	
	setPos(pos);
	setZValue(1000);
}


Node::~Node()
{
}


void Node::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
	Q_UNUSED(w);
	QColor c;
	
	if (option->state & QStyle::State_Sunken)
	{
		c = QColor("green");
		c.setAlpha(150);
	} 
	else
	{
		c = QColor("navy");
		c.setAlpha(150);
	}
	QRectF br = boundingRect();

	painter->setBrush( c );
	painter->drawRect(br);
	if(m_typeNode == Center)
	{
		painter->save();
		painter->setPen(Qt::gray);
		painter->drawLine(br.topLeft(), br.bottomRight()  );
		painter->drawLine(br.bottomLeft(), br.topRight()  );
		painter->restore();
	}
}

QRectF Node::boundingRect() const
{
	QSizeF size( 8  , 8 );
	QRectF r(QPointF( -size.width()/2, -size.height()/2), size);
	return r;
}


QVariant Node::itemChange(GraphicsItemChange change, const QVariant &value)
{
	
	return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	m_brParent = m_parent->sceneBoundingRect();
	QGraphicsItem::mousePressEvent(event);
	m_parent->setSelected( true);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	QGraphicsItem::mouseReleaseEvent(event);
	m_parent->setSelected( true);
}

void Node::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	QPointF newPos(event->pos()+pos());
	
	if( m_notChange)
	{
		m_notChange = false;
	}
	else
	{
		QRectF rect =  m_parent->sceneBoundingRect();
		rect.moveTopLeft (m_parent->scenePos());
		QRectF br =   m_parent->sceneBoundingRect();
		
		
		switch(m_typeNode)
		{
			case TopLeft:
			{
				rect.setTopLeft( newPos );
				break;
				
			}
			case TopRight:
			{
				rect.setTopRight(newPos);
				break;
			}
			case BottomRight:
			{
				rect.setBottomRight(newPos);
				break;
			}
			case BottomLeft:
			{
				rect.setBottomLeft(newPos);
				break;
			}
			case Center:
			{
				break;
			}
		};

#if 0
		if(gb1)
		{
			scene()->removeItem(gb1);
		}
		
		if(gb2)
		{
			scene()->removeItem(gb2);
		}
		
		gb1 = scene()->addRect(rect, QPen(Qt::magenta));
		gb2 = scene()->addRect(br, QPen(Qt::blue));
#endif	
		
		float sx = 1, sy = 1;
		sx = static_cast<float>(rect.width()) / static_cast<float>(br.width());
		sy = static_cast<float>(rect.height()) / static_cast<float>(br.height());
		
		if(sx > 0 && sy > 0)
		{
			m_parent->scale(sx, sy);
			m_parent->setPos(  rect.topLeft());
		}
		else 
		{
			if(sx > 0)
			{
				
				m_parent->setPos( rect.topLeft().x(),  br.topLeft().y());
				m_parent->scale(sx, 1);
			}
			
			if(sy > 0)
			{
				m_parent->setPos( br.topLeft().x() , rect.topLeft().y() );
				m_parent->scale(1, sy);
			}
		}
	}
	
	if(m_typeNode == Center)
	{
		m_parent->moveBy(event->pos().x(), event->pos().y());
		QGraphicsItem::mouseReleaseEvent(event);
	}
	m_manager->syncNodes( m_parent->sceneBoundingRect() );
	update();
	m_parent->setSelected( true);
}


int Node::typeNode() const
{
	return m_typeNode;
}
