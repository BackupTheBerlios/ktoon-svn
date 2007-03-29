/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
 *   kruadrosx@toonka.com                                                     *
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

#include "ktlineguide.h"

#include <QGraphicsScene>
#include <QPainter>
#include <QCursor>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsView>
#include <QDebug>
#include <QApplication>

struct KTGuideLine::Private
{
	Qt::Orientation orientation;
};

KTGuideLine::KTGuideLine(Qt::Orientation o ,QGraphicsScene *scene): QGraphicsItem(0, scene), d(new Private)
{
	d->orientation = o;
	setAcceptsHoverEvents(true);
}


KTGuideLine::~KTGuideLine()
{
}

QRectF KTGuideLine::boundingRect() const
{
	if(d->orientation == Qt::Vertical)
	{
		return QRectF(QPointF(0,0), QSizeF(5, scene()->height()));
	}
	else
	{
		return QRectF(QPointF(0,0), QSizeF(scene()->width(), 5 ));
	}
}

void KTGuideLine::paint( QPainter * painter, const QStyleOptionGraphicsItem * , QWidget * )
{
	painter->setPen(QPen(Qt::black, 1, Qt::DashLine));
	if(d->orientation == Qt::Vertical)
	{
		painter->drawLine((int)boundingRect().center().x(), 0, (int)boundingRect().center().x(), (int)boundingRect().height());
	}
	else
	{
		painter->drawLine( 0, (int)boundingRect().center().y(), (int)boundingRect().width(), (int)boundingRect().center().y());
	}
	
}


QVariant KTGuideLine::itemChange( GraphicsItemChange change, const QVariant & value )
{
	if(change == ItemPositionChange)
	{
		if(d->orientation == Qt::Vertical)
		{
			return QPointF(value.toPointF().x(), 0);
		}
		else
		{
			return QPointF(0, value.toPointF().y());
		}
	}
	return QGraphicsItem::itemChange(change, value );
}

void KTGuideLine::hoverMoveEvent ( QGraphicsSceneHoverEvent * )
{
	syncCursor();
}

void KTGuideLine::mouseMoveEvent ( QGraphicsSceneMouseEvent * )
{
	syncCursor();
}

void KTGuideLine::syncCursor()
{
	QPointF globalPos;
	if(scene())
	{
		foreach(QGraphicsView *view, scene()->views())
		{
			globalPos = view->viewport()->mapToGlobal(scenePos().toPoint() + view->mapFromScene(QPointF(0,0))) ;
		}
	}
	
	double distance;
	if(d->orientation == Qt::Vertical)
	{
		distance = globalPos.x()+ 2 - QCursor::pos().x();
	}
	else
	{
		distance = globalPos.y() + 2 - QCursor::pos().y();
	}
	
	if( -QApplication::startDragDistance() < distance && distance < QApplication::startDragDistance() )
	{
		if(d->orientation == Qt::Vertical)
		{
			QCursor::setPos((int)globalPos.x()+2, (int)QCursor::pos().y()) ;
		}
		else
		{
			QCursor::setPos((int)QCursor::pos().x(), (int)globalPos.y()+2);
		}
	}
}

