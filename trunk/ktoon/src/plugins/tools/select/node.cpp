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

#include <QCursor>
#include <ddebug.h>
#include "nodemanager.h"

#include <cmath> //atan

Node::Node(TypeNode node, ActionNode action, const QPointF & pos, NodeManager *manager, QGraphicsItem * parent,  QGraphicsScene * scene   ) : QGraphicsItem(0, scene), m_typeNode(node), m_action(action), m_notChange(true), m_parent(parent), m_manager(manager)
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
	
	if(m_action == Rotate)
	{
		c.setGreen(200);
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
	if(change ==  ItemSelectedChange)
	{
		D_FUNCINFO;
		SHOW_VAR(value.toBool());
		setVisible(true);
		
		if(value.toBool())
		{
			m_parent->setSelected(true);
		}
		m_manager->show();
	}
	return QGraphicsItem::itemChange(change, value);
}

void Node::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	D_FUNCINFO;
	update();
	m_manager->setPress( true);
	QGraphicsItem::mousePressEvent(event);
}

void Node::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	QGraphicsItem::mouseReleaseEvent(event);
	m_parent->setSelected(true);
	m_manager->setPress(false);
}

void Node::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	QPointF newPos(mapToItem(m_parent,event->pos()));
// 	QMatrix m = m_parent->sceneMatrix();
	
// 	double arc = asin(m.m21())*(180/3.141592653589793116);
// 	SHOW_VAR(arc);
// 	QMatrix m1(m.m11()-cos(arc),0,0,m.m22()-cos(arc),m.dx(),m.dy() );
	
// 	newPos = m1.map( newPos );
	
	
	
	if( m_notChange)
	{
		m_notChange = false;
	}
	else
	{
		if(m_action == Scale)
		{
			QRectF rect = m_parent->boundingRect();
			QRectF br = m_parent->boundingRect();
			switch(m_typeNode)
			{
				case TopLeft:
				{
					m_manager->setAnchor(br.bottomRight());
					rect.setTopLeft( newPos );
					
					break;
				}
				case TopRight:
				{
					m_manager->setAnchor( br.bottomLeft());
					rect.setTopRight(newPos);
					break;
				}
				case BottomRight:
				{
					m_manager->setAnchor( br.topLeft());
					rect.setBottomRight(newPos);
					break;
				}
				case BottomLeft:
				{
					m_manager->setAnchor( br.topRight());
					rect.setBottomLeft(newPos);
					break;
				}
				case Center:
				{
					break;
				}
			};
			float sx = 1, sy = 1;
			sx = static_cast<float>(rect.width()) / static_cast<float>(br.width());
			sy = static_cast<float>(rect.height()) / static_cast<float>(br.height());
		
			if(sx > 0 && sy > 0)
			{
				m_manager->scale( sx,sy);
			}
			else 
			{
				if(sx > 0)
				{
					m_manager->scale(sx, 1);
				}
				if(sy > 0)
				{
					m_manager->scale(1, sy);
				}
			}
		}
		else
		{
			QPointF p1 = newPos;
			QPointF p2 = m_parent->boundingRect().center();
			m_manager->setAnchor( p2 );
			QPointF d = p1 - p2;
			if(d.x() != 0)
			{
				double a =  atan(d.y() / d.x())*(180/3.141592653589793116);
				if(d.x() < 0)
				{
					a += 180;
				}
				m_manager->rotate( a );
			}
		}
		
	}
	
	if(m_typeNode == Center)
	{
		m_parent->moveBy(event->pos().x(), event->pos().y());
	}
	update();
	m_parent->setSelected(true);
}


void Node::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * )
{
	update();
	m_manager->toggleAction();
}

int Node::typeNode() const
{
	return m_typeNode;
}

void Node::setAction(ActionNode action)
{
	m_action = action;
}

int Node::actionNode()
{
	return m_action;
}
