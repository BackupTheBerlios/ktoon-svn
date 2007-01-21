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

Node::Node(TypeNode node, ActionNode action, const QPointF & pos, NodeManager *manager, QGraphicsItem * parent,  QGraphicsScene * scene   ) : QGraphicsItem(parent, scene), m_typeNode(node), m_action(action), m_notChange(true), m_parent(parent), m_manager(manager), m_newRotation(0), m_newScale(1)
{
	QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor ));
// 	setParentItem(m_parent);
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
	D_FUNCINFO;
	update();
	QGraphicsItem::mouseReleaseEvent(event);
	m_parent->setSelected(true);
	m_manager->setPress(false);
	
	SHOW_VAR(m_newScale);
	m_parent->setData( KTGraphicObject::Rotate, m_newRotation);
	m_parent->setData( KTGraphicObject::ScaleX, m_newScale );
}

void Node::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	QPointF buttonDownPos = m_parent->mapFromScene(event->buttonDownPos(Qt::LeftButton));
	QPointF newPos = m_parent->mapFromScene(event->scenePos());
	
// 	scene()->addRect(QRectF(buttonDownPos - QPointF(2,2), QSizeF(4,4)),QPen( Qt::red));
// 	scene()->update(QRectF(buttonDownPos - QPointF(2,2), QSizeF(4,4)));
// 	scene()->addRect(QRectF(newPos - QPointF(2,2), QSizeF(4,4)), QPen(Qt::green));
// 	scene()->update(QRectF(newPos - QPointF(2,2), QSizeF(4,4)));
	
	QPointF m_anchor = m_parent->boundingRect().center();
	m_newScale = m_parent->data(KTGraphicObject::ScaleX).toDouble()  * KTGraphicalAlgorithm::distanceToPoint(newPos - m_anchor ) / KTGraphicalAlgorithm::distanceToPoint(buttonDownPos - m_anchor);
	QMatrix m = QMatrix();
	qreal oldAngle = (180 * KTGraphicalAlgorithm::angleForPos(buttonDownPos, m_anchor )) / M_PI;
	qreal newAngle = (180 * KTGraphicalAlgorithm::angleForPos(newPos,m_anchor)) / M_PI;
	m_newRotation = m_parent->data(KTGraphicObject::Rotate).toDouble() + oldAngle - newAngle;
	
	SHOW_VAR(m_parent->data(KTGraphicObject::ScaleX).toDouble());
	
	m.translate(m_anchor.x(), m_anchor.y());
	m.rotate(m_newRotation);
	m.scale(m_newScale, m_newScale);
	m.translate(-m_anchor.x(), -m_anchor.y());
	m_parent->setMatrix(m);

}


void Node::mouseDoubleClickEvent ( QGraphicsSceneMouseEvent * e)
{
	D_FUNCINFO;
	update();
	m_manager->toggleAction();
	e->setAccepted (false);
	QGraphicsItem::mouseDoubleClickEvent(e);
	
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
