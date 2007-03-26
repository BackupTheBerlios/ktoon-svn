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

#include "dcontrolnode.h"
#include <QCursor>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QStyleOption>
#include <QStyleOptionButton>
#include <QApplication>

#include <dcore/ddebug.h>
#include <QCursor>

#include <QGraphicsPathItem>
#include "dnodegroup.h"

DControlNode::DControlNode(int index, DNodeGroup *nodeGroup, const QPointF & pos, QGraphicsItem * parent,  QGraphicsScene * scene   ) : QGraphicsItem(0, scene), m_index(index), m_parent(0),  m_left(0),  m_right(0), m_nodeParent(0), m_notChange(true) ,m_nodeGroup(nodeGroup), m_scene(scene)
{
	QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor ));
	setFlag(ItemIsSelectable, true);
	setFlag(ItemIsMovable, true);
	
	setPos(pos);
	setZValue(1000);
	
	setParentI(parent);
}


DControlNode::~DControlNode()
{
}


void DControlNode::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *w )
{
	Q_UNUSED(w);
	
	bool antialiasing =  painter->renderHints() & QPainter::Antialiasing;
	
	painter->setRenderHint(QPainter::Antialiasing, false);
	
	QColor c;
	
	if (option->state & QStyle::State_Sunken || option->state & QStyle::State_Selected )
	{
		painter->save();
		painter->setPen(QPen(Qt::gray) );
		
		paintLinesToChilds(painter);
		painter->restore();
		
		if ( QAbstractGraphicsShapeItem *it = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(m_parent))
		{
			QColor nc = it->pen().brush().color();
			c = QColor(nc.red()+180 % 255, nc.green()+180 % 255, nc.blue()+180 % 255);
		}
		else
		{
			c = QColor("green");
		}
		
		c.setAlpha(150);
	}
	else
	{
		if ( QAbstractGraphicsShapeItem *it = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(m_parent))
		{
			QColor nc = it->pen().brush().color();
			c = QColor(nc.red()+254 % 255, nc.green()+254 % 255, nc.blue()+254 % 255);
		}
		else
		{
			c = QColor("navy");
		}
		
		c.setAlpha(150);
	}
	
	
	if(m_nodeParent)
	{
		c.setRed(100);
	}
	painter->setBrush( c );
	paintLinesToChilds(painter);
	
	painter->drawRoundRect(boundingRect());
	
	painter->setRenderHint(QPainter::Antialiasing, antialiasing);
}

void DControlNode::paintLinesToChilds(QPainter * painter)
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

QRectF DControlNode::boundingRect() const
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


QVariant DControlNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change == ItemPositionChange )
	{
		if(!m_notChange)
		{
			if( qgraphicsitem_cast<QGraphicsPathItem*>(m_parent) )
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
				if(m_nodeGroup)
				{
					m_nodeGroup->moveElementTo(m_index, scenePos );
				}
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

void DControlNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
// 	update();
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
	
	event->accept();
}



void DControlNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	QGraphicsItem::mouseReleaseEvent(event);
}

void DControlNode::mouseMoveEvent ( QGraphicsSceneMouseEvent * event )
{
	foreach(QGraphicsItem *item, scene()->selectedItems())
	{
		if(qgraphicsitem_cast<DControlNode*>(item))
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
	
	event->accept();
	
// 	update();
}


void DControlNode::setLeft( DControlNode *left)
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

void DControlNode::setRight( DControlNode *right)
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

void DControlNode::setNodeParent( DControlNode *nodeParent)
{
	m_nodeParent = nodeParent;
}


void DControlNode::setVisibleChilds(bool visible)
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

void DControlNode::setSeletedChilds(bool select)
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

DControlNode *DControlNode::left()
{
	return m_left;
}


DControlNode *DControlNode::right()
{
	return m_right;
}

DControlNode *DControlNode::nodeParent()
{
	return m_nodeParent;
}

int DControlNode::index() const
{
	return m_index;
}

void DControlNode::repaint()
{
	update();
}

void  DControlNode::setParentI(QGraphicsItem *newParent)
{
	m_parent = newParent;
}

QGraphicsItem * DControlNode::parentI()
{
	return m_parent;
}

void DControlNode::setNotChange(bool notChange)
{
	m_notChange = notChange;
}
