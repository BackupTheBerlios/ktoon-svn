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
#include "ktgraphicalgorithm.h"

#include "ktgraphicobject.h"


#define DEBUG 0

Node::Node(TypeNode node, ActionNode action, const QPointF & pos, NodeManager *manager, QGraphicsItem * parent,  QGraphicsScene * scene   ) : QGraphicsItem(0, scene), m_typeNode(node), m_action(action), m_notChange(true), m_parent(parent), m_manager(manager)
{
	QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor ));
// 	setParent(m_parent);
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
	//DEBUG
#if DEBUG
	painter->setFont( QFont( painter->font().family(), 5 ));
	painter->drawText( br, QString::number(m_typeNode));
#endif
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
// 	dDebug() << "press";
// 	dFatal() << "press";
	D_FUNCINFO;
	update();
	m_manager->setPress( true);
	
	QGraphicsItem::mousePressEvent(event);
	
#if DEBUG
	QRectF r = m_parent->sceneMatrix().inverted().mapRect( m_parent->sceneBoundingRect() );
	scene()->addRect(r, QPen(Qt::magenta), QBrush(QColor(100,100,200,50)));
	scene()->update(r);
#endif
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
	D_FUNCINFO;
	QPointF newPos(/*mapToItem(m_parent,*/ event->scenePos())/*)*/;
	
	if( m_notChange)
	{
		m_notChange = false;
	}
	else
	{
		if(m_action == Scale)
		{
			QRectF rect = m_parent->sceneBoundingRect();
			QRectF br = m_parent->sceneBoundingRect();
			QRectF br1 = m_parent->boundingRect();
			
			//Debug
// 			scene()->addRect(rect, QPen(Qt::red));
// 			scene()->addRect(br, QPen(Qt::green));
			//Debug
			
			
			switch(m_typeNode)
			{
				case TopLeft:
				{
					m_manager->setAnchor(br1.bottomRight());
					rect.setTopLeft( newPos );
					break;
				}
				case TopRight:
				{
					m_manager->setAnchor( br1.bottomLeft());
					rect.setTopRight(newPos);
					break;
				}
				case BottomRight:
				{
					m_manager->setAnchor( br1.topLeft());
					rect.setBottomRight(newPos);
					break;
				}
				case BottomLeft:
				{
					m_manager->setAnchor( br1.topRight());
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
			
			if(m_typeNode != Center)
			{
// 				m_manager->setVisible(false);
				QPointF p1 = newPos;
				QPointF p2 = m_parent->sceneBoundingRect().center();
				m_manager->setAnchor( m_parent->boundingRect().center() );
				
				double a = (180 * KTGraphicalAlgorithm::angleForPos(p1, p2)) / M_PI;
				m_manager->rotate(a-45 );
			}
			else
			{
				
			}
		}
	}
	
	if(m_typeNode == Center)
	{
		m_parent->moveBy(event->pos().x(), event->pos().y() );
	}
	update();
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
	update();
}

int Node::actionNode()
{
	return m_action;
}
