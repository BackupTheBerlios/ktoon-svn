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

struct DControlNode::Private
{
	int index;
	QGraphicsItem * parent;
	DControlNode *left, *right, *nodeParent;
	bool notChange;
	DNodeGroup *nodeGroup;
	QGraphicsScene *scene;
};


DControlNode::DControlNode(int index, DNodeGroup *nodeGroup, const QPointF & pos, QGraphicsItem * parent,  QGraphicsScene * scene   ) : QGraphicsItem(0, scene), d( new Private)
{
	d->index  = index;
	d->parent = 0;
	d->left = 0;
	d->right = 0;
	d->nodeParent = 0;
	d->notChange = true;
	d->nodeGroup = nodeGroup;
	d->scene = scene;
	
	QGraphicsItem::setCursor(QCursor(Qt::PointingHandCursor ));
	setFlag(ItemIsSelectable, true);
	setFlag(ItemIsMovable, true);
	
	setPos(pos);
	setZValue(1000);
	
	setParentI(parent);
}


DControlNode::~DControlNode()
{	
	//DEND;
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
		
// 		paintLinesToChilds(painter);
		painter->restore();
		
		if ( QAbstractGraphicsShapeItem *it = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->parent))
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
		if ( QAbstractGraphicsShapeItem *it = qgraphicsitem_cast<QAbstractGraphicsShapeItem *>(d->parent))
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
	
	
	if(d->nodeParent)
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
	if(d->right)
	{
		if(d->right->isVisible())
		painter->drawLine(inverted.map(pos()), inverted.map(d->right->pos()));
	}
	if(d->left)
	{
		if(d->left->isVisible())
		painter->drawLine(inverted.map(pos()), inverted.map(d->left->pos()));
	}
	
	painter->restore();
}

QRectF DControlNode::boundingRect() const
{
	QSizeF size( 8 , 8 );
	QRectF r(QPointF( -size.width()/2, -size.height()/2), size);
	if(d->right)
	{
		if(d->right->isVisible())
		{
			r.unite(d->right->boundingRect());
		}
	}
	if(d->left)
	{
		if(d->left->isVisible())
		{
			r.unite(d->left->boundingRect());
		}
	}
	return r;
}


QVariant DControlNode::itemChange(GraphicsItemChange change, const QVariant &value)
{
	if(change == ItemPositionChange )
	{
		if(!d->notChange)
		{
			if( qgraphicsitem_cast<QGraphicsPathItem*>(d->parent) )
			{
				QPointF diff = value.toPointF() - pos() ;
				if(d->left)
				{
					d->left->moveBy(diff.x(), diff.y());
				}
				if(d->right)
				{
					d->right->moveBy(diff.x(), diff.y());
				}
				QPointF scenePos = d->parent->mapFromScene ( value.toPointF());
				if(d->nodeGroup)
				{
					d->nodeGroup->moveElementTo(d->index, scenePos );
				}
			}
		}
		else
		{
			d->notChange = false;
		}
	}
	else if(change == ItemSelectedChange)
	{
		if(value.toBool())
		{
			d->parent->setSelected(true);
			setVisibleChilds(true);
		}
		else
		{
			if(d->left)
			{
				if(d->left->isSelected())
				{
					d->left->setVisible(true);
				}
				else
				{
					d->left->setVisible(false);
				}
			}
			if(d->right)
			{
				if(d->right->isSelected())
				{
					d->right->setVisible(true);
				}
				else
				{
					d->right->setVisible(false);
				}
			}
			update();
		}
	
	}
	return QGraphicsItem::itemChange(change, value);
}

void DControlNode::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
	if(d->nodeParent)
	{
		setSelected(true);
		d->nodeParent->setSelected( true);
		if(d->nodeParent->left())
		{
			if(d->nodeParent->left() != this)
			{
				d->nodeParent->left()->setSelected(false);
			}
		}
		if(d->nodeParent->right())
		{
			if(d->nodeParent->right() != this)
			{
				d->nodeParent->right()->setSelected(false);
			}
		}
	}
	else
	{
		setSeletedChilds(false);
	}
	QGraphicsItem::mousePressEvent(event);
	
	d->parent->setSelected( true);
	setVisibleChilds(true);
	
	event->accept();
}



void DControlNode::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
	d->nodeGroup->emitNodeClicked();
	QGraphicsItem::mouseReleaseEvent(event);
}

void DControlNode::mouseMoveEvent( QGraphicsSceneMouseEvent * event )
{
	foreach(QGraphicsItem *item, scene()->selectedItems())
	{
		if(qgraphicsitem_cast<DControlNode*>(item))
		{
			if(d->nodeParent)
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
}


void DControlNode::setLeft( DControlNode *left)
{
	if(d->left)
	{
		delete d->left;
	}
	d->left = left;
	d->left->setVisible(false);
	d->left->setNodeParent(this);
	d->left->setZValue(zValue()+1);
// 	connect( d->left, SIGNAL(requestUpdateParent()), this, SLOT(repaint()));
}

void DControlNode::setRight( DControlNode *right)
{
	if(right)
	{
		delete d->right;
	}
	d->right = right;
	d->right->setVisible(false);
	d->right->setNodeParent(this);
	d->right->setZValue(zValue()+2);
// 	connect( d->right, SIGNAL(requestUpdateParent()), this, SLOT(repaint()));
}

void DControlNode::setNodeParent( DControlNode *nodeParent)
{
	d->nodeParent = nodeParent;
}


void DControlNode::setVisibleChilds(bool visible)
{
	if(d->left)
	{
		d->left->setVisible(visible);
	}
	if(d->right)
	{
		d->right->setVisible(visible);
	}
}

void DControlNode::setSeletedChilds(bool select)
{
	if(d->left)
	{
		d->left->setSelected(select);
	}
	if(d->right)
	{
		d->right->setSelected(select);
	}
}

DControlNode *DControlNode::left()
{
	return d->left;
}


DControlNode *DControlNode::right()
{
	return d->right;
}

DControlNode *DControlNode::nodeParent()
{
	return d->nodeParent;
}

int DControlNode::index() const
{
	return d->index;
}

void DControlNode::setParentI(QGraphicsItem *newParent)
{
	d->parent = newParent;
}

QGraphicsItem * DControlNode::parentI()
{
	return d->parent;
}

void DControlNode::setNotChange(bool notChange)
{
	d->notChange = notChange;
}

