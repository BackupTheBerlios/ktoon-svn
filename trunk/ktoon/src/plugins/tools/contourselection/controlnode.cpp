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

#include  "ktpathitem.h"
#include "nodegroup.h"

ControlNode::ControlNode(int index, NodeGroup *nodeGroup, const QPointF & pos, QGraphicsItem * parent,  QGraphicsScene * scene   ) : QGraphicsItem(0, scene), m_index(index), m_parent(parent),  m_left(0),  m_right(0), m_nodeParent(0), m_notChange(true) ,m_nodeGroup(nodeGroup)
{
	QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor ));
// 	setFlags(ItemIsMovable);
	setFlag(ItemIsSelectable, true);
	setPos(pos);
	setZValue(1000);
}


ControlNode::~ControlNode()
{
}


void ControlNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
	Q_UNUSED(w);
	QColor c;
	
	if (option->state & QStyle::State_Sunken || option->state & QStyle::State_Selected )
	{
		painter->save();
		painter->setPen(QPen( Qt::gray));

		paintLinesToChilds(painter);
		painter->restore();

		c = QColor("green");
		c.setAlpha(150);
	}
	else
	{
		c = QColor("navy");
		c.setAlpha(150);
	}
	
	
	if(m_nodeParent)
	{
		c.setRed(100);
	}
	painter->setBrush( c );
	paintLinesToChilds(painter);
	
	painter->drawRect(boundingRect());
}

void ControlNode::paintLinesToChilds(QPainter * painter)
{
	QMatrix inverted = sceneMatrix().inverted();
	painter->save();
	painter->setPen(QPen(Qt::gray));
	if(m_right)
	{
		if(m_right->isVisible())
		painter->drawLine(inverted.map(pos()), inverted.map(m_right->pos()));
	}
	if(m_left)
	{
		if(m_left->isVisible())
		painter->drawLine(inverted.map(pos()), inverted.map(m_left->pos()));
	}
	if(m_nodeParent)
	{
		emit requestUpdateParent();
	}
	painter->restore();
}

QRectF ControlNode::boundingRect() const
{
	QSizeF size( 8 , 8 );
	QRectF r(QPointF( -size.width()/2, -size.height()/2), size);
	if(m_right)
	{
		if(m_right->isVisible())
		r.unite(m_right->boundingRect());
	}
	if(m_left)
	{
		if(m_left->isVisible())
		r.unite(m_left->boundingRect());
	}
	return r;
}


QVariant ControlNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
	
	if(change == ItemPositionChange )
	{
		if(!m_notChange)
		{
// 			dDebug() << "0";
// 			dDebug() << m_parent->type();
// // 			dDebug() << QGraphicsPathItem().type();
// 			if(!m_parent)
// 			{
// 				dDebug() << m_parent->type();
				
// 			}
// 			dDebug() << m_parent;
// 			if( !dynamic_cast<KTPathItem*>(m_parent) )
// 			{
// 				dDebug() << "1";
// 				scene()->removeItem(m_parent);
// 				
// 				KTPathItem *tmp = new KTPathItem( m_parent->parentItem(), scene());
// // 				dDebug() << "2";
// 				tmp->setPath(m_parent->shape());
// // 				tmp->setMatrix(m_parent->matrix());
// 				tmp->setPos(m_parent->scenePos());
// 				tmp->setFlags(m_parent->flags() );
// 				if(dynamic_cast<QAbstractGraphicsShapeItem*>(m_parent))
// 				{
// 					tmp->setBrush( dynamic_cast<QAbstractGraphicsShapeItem*>(m_parent)->brush() );
// 					tmp->setPen( dynamic_cast<QAbstractGraphicsShapeItem*>(m_parent)->pen() );
// 				}
// // 				tmp->setSelected(m_parent->isSelected());
// 				tmp->setSelected(true);
// // 				dDebug() << "3";
// // 				delete m_parent;
// 				m_parent = tmp;
// 				m_nodeGroup->setParentItem( tmp );
// 				dDebug() << "4";
// 				setParent();
// 			}
// 			dDebug() << "5";
// 			else
			{
				QPointF diff = value.toPointF() - pos() ;
				if(m_left)
				{
					m_left->moveBy(diff.x(), diff.y());
				}
				if(m_right)
				{
					m_right->moveBy(diff.x(), diff.y());
				}
				QPointF scenePos = m_parent->mapFromScene ( value.toPointF());
				QPainterPath path = static_cast<QGraphicsPathItem *>(m_parent)->path();
				path.setElementPositionAt(m_index,
					scenePos.x(),
					scenePos.y() );
				QPainterPath::Element e = path.elementAt(0);
				QPointF pos = path. controlPointRect().topLeft();
				QMatrix m;
				m.translate(-pos.x(), -pos.y());
				path = m.map(path);
				m_parent->setPos(m_parent->mapToScene(pos));
				static_cast<QGraphicsPathItem *>( m_parent)->setPath(path);
			}
		}
		else
		{
			m_notChange = false;
		}
	}
	else if(change ==  ItemSelectedChange)
	{
		if(value.toBool()  )
		{
			m_parent->setSelected(true);
		}
		else
		{
			if(m_left && m_right)
			{
				if(m_left->isSelected() || m_right->isSelected())
				{
					
					setVisibleChilds(true);
				}
				else
				{
					setVisibleChilds(false);
				}
			}
			else
			{
				if(m_left && !m_left->isSelected())
				{
					m_left->setVisible(false);
				}
				if(m_right && !m_right->isSelected())
				{
					m_right->setVisible(false);
				}
			}
		}
	}
	
	return QGraphicsItem::itemChange(change, value);
}

void ControlNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{

	update();
	
	if(m_nodeParent)
	{
		m_nodeParent->setSelected( true);
		setSelected(true);
	}
	
	QGraphicsItem::mousePressEvent(event);
	
	m_parent->setSelected( true);
	setVisibleChilds(true);
}

void ControlNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	update();
	QGraphicsItem::mouseReleaseEvent(event);
	m_parent->setSelected( true);
}

void ControlNode::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	foreach(QGraphicsItem *item, scene()->selectedItems())
	{
		if(dynamic_cast<ControlNode*>(item))
		{
			if(item != this)
			{
				item->moveBy(event->pos().x(), event->pos().y());
			}
		}
	}
	setPos(event->scenePos());
	if(m_parent)
	{
		m_parent->setSelected( true);
	}
	else
	{
		dDebug() << "HEEREE";
	}
	event->ignore();
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


void ControlNode::setVisibleChilds(bool visible)
{
	if(m_left)
	{
		m_left->setVisible(visible);
	}
	if(m_right)
	{
		m_right->setVisible(visible);
	}
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
	update();
}

void  ControlNode::setParentI(QGraphicsItem *newParent)
{
	m_parent = newParent;
}


QGraphicsItem * ControlNode::parentI()
{
	return m_parent;
}
