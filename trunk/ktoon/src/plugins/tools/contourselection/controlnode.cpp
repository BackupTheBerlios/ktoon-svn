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

ControlNode::ControlNode(int index, NodeGroup *nodeGroup, const QPointF & pos, QGraphicsItem * parent,  KTScene * scene   ) : QGraphicsItem(0, scene), m_index(index), m_parent(0),  m_left(0),  m_right(0), m_nodeParent(0), m_notChange(true) ,m_nodeGroup(nodeGroup), m_scene(scene)
{
	QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor ));
// 	setFlags(ItemIsMovable);
	setFlag(ItemIsSelectable, true);
	setFlag(ItemIsMovable, true);
	
	setPos(pos);
	setZValue(1000);
	
	setParentI(parent);
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
	painter->setPen(QPen(QColor(0x8080FF)));
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
			if( qgraphicsitem_cast<KTPathItem*>(m_parent) )
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
				QPainterPath path = qgraphicsitem_cast<QGraphicsPathItem *>(m_parent)->path();
				path.setElementPositionAt(m_index,
					scenePos.x(),
					scenePos.y() );
				QPainterPath::Element e = path.elementAt(0);
				QPointF pos = path. controlPointRect().topLeft();
				QMatrix m;
				m.translate(-pos.x(), -pos.y());
				path = m.map(path);
				m_parent->setPos(m_parent->mapToScene(pos));
				qgraphicsitem_cast<QGraphicsPathItem *>( m_parent)->setPath(path);
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
		setSelected(true);
		m_nodeParent->setSelected( true);
		if(m_nodeParent->left())
		{
			if(m_nodeParent->left() != this)
			{
				m_nodeParent->left()->setSelected(false);
			}
		}
		if(m_nodeParent->right())
		{
			if(m_nodeParent->right() != this)
			{
				m_nodeParent->right()->setSelected(false);
			}
		}
	}
	else
	{
		setSeletedChilds(false);
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
		if(qgraphicsitem_cast<ControlNode*>(item))
		{
			if(m_nodeParent)
			{
			}
			else
			{
				if(item != this)
				{
					item->moveBy(event->pos().x(), event->pos().y());
				}
			}
		}
	}
	setPos(event->scenePos());
// 	if(m_parent)
// 	{
// 		m_parent->setSelected( true);
// 	}
// 	else
// 	{
// 		dDebug() << "Not parent";
// 	}
	
// 	event->ignore();
	update();
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
	m_left->setZValue(zValue()+1);
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
	m_right->setZValue(zValue()+2);
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

void ControlNode::setSeletedChilds(bool select)
{
	if(m_left)
	{
		m_left->setSelected(select);
	}
	if(m_right)
	{
		m_right->setSelected(select);
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
