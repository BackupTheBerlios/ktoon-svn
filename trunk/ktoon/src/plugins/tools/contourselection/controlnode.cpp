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

#include "controlnode.h"
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QStyleOptionButton>
#include <QApplication>

#include <ddebug.h>
#include <QCursor>


ControlNode::ControlNode(int index, const QPointF & pos, QGraphicsItem * parent,  QGraphicsScene * scene   ) : QGraphicsItem(0, scene), m_index(index), m_notChange(true), m_parent(parent),  m_left(0),  m_right(0), m_nodeParent(0)
{
	QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor ));
// 	setFlag(ItemIsMovable);
	setFlag(ItemIsSelectable, false);
	setPos(pos);
// 	translate(pos.x(), pos.y());
	setZValue(1000);
}


ControlNode::~ControlNode()
{
}


void ControlNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
	Q_UNUSED(w);
	if (option->state & QStyle::State_Sunken )
	{
		painter->save();
		painter->setPen(QPen( Qt::gray));

		paintLinesToChilds(painter);
		painter->restore();

		QColor c("green");
		c.setAlpha(150);
		painter->setPen(QPen(c, 5));
	}
	else
	{
		QColor c("navy");
		c.setAlpha(150);
		painter->setPen(QPen(c, 5));
	}

	if(m_repaint)
	{
		paintLinesToChilds(painter);
		m_repaint = false;
	}
	painter->drawPoint(QPoint(0,0));
}

void ControlNode::paintLinesToChilds(QPainter * painter)
{
	QMatrix inverted = sceneMatrix().inverted();
	painter->save();
	painter->setPen(QPen(Qt::gray));
	if(m_right)
	{
		m_right->setVisible(true);
		painter->drawLine(inverted.map(pos()), inverted.map(m_right->pos()));
		m_repaint = false;
	}
	if(m_left)
	{

		m_left->setVisible(true);
		painter->drawLine(inverted.map(pos()), inverted.map(m_left->pos()));
		m_repaint = false;
	}
	if(m_nodeParent)
	{
		emit requestUpdateParent();
	}
	painter->restore();
}

QRectF ControlNode::boundingRect() const
{
	QSizeF size( 10  , 10 );
	QRectF r(QPointF( -size.width()/2, -size.height()/2), size);
	return r;
}


QVariant ControlNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
	return QGraphicsItem::itemChange(change, value);
}

void ControlNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	m_brParent = m_parent->sceneBoundingRect();
	
	QGraphicsItem::mousePressEvent(event);
	m_parent->setSelected( true);
}

void ControlNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	QGraphicsItem::mouseReleaseEvent(event);
	m_parent->setSelected( true);
}

void ControlNode::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	if(dynamic_cast<QGraphicsPathItem *>(m_parent) )
	{
		QPainterPath path = static_cast<QGraphicsPathItem *>(m_parent)->path();
		path.setElementPositionAt(m_index, 
			event->scenePos().x() - m_parent->pos().x(),
			event->scenePos().y()-m_parent->pos().y() );
		
		QPainterPath::Element e = path.elementAt(0);
		QPointF pos = path. controlPointRect().topLeft();
		
// 		SHOW_VAR(pos);
		
		static_cast<QGraphicsPathItem *>( m_parent)->setPath(path);
		static_cast<QGraphicsPathItem *>( m_parent)->setPos(m_parent->sceneBoundingRect().topLeft());
		QMatrix m;
		m.translate(-pos.x(), -pos.y());
		path = m.map(path);
		static_cast<QGraphicsPathItem *>( m_parent)->setPath(path);
	}
	setPos(event->scenePos());
	m_parent->setSelected( true);
}
void ControlNode::setLeft( ControlNode *left)
{
	if(m_left)
	{
		delete m_left;
	}

	m_left = left;
	m_left->setVisible(false);
	m_left->setNodeParent(this);
	connect( m_left, SIGNAL(requestUpdateParent()), this, SLOT(repaint()));
}


void ControlNode::setRight( ControlNode *right)
{
	if(right)
	{
		delete m_right;
	}

	m_right = right;
	m_right->setVisible(false);
	m_right->setNodeParent(this);
	connect( m_right, SIGNAL(requestUpdateParent()), this, SLOT(repaint()));
}

void ControlNode::setNodeParent( ControlNode *nodeParent)
{
	m_nodeParent = nodeParent;
}


ControlNode *ControlNode::left()
{
	return m_left;
}


ControlNode *ControlNode::right()
{
	return m_right;
}

ControlNode *ControlNode::nodeParent()
{
	return m_nodeParent;
}

int ControlNode::index() const
{
	return m_index;
}

void  ControlNode::repaint()
{
	m_repaint = true;
	update();
}

