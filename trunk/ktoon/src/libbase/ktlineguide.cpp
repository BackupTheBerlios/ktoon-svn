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

struct KTGuideLine::Private
{
	Qt::Orientation orientation;
	
	QPointF position;
	
};

KTGuideLine::KTGuideLine(Qt::Orientation o ,QGraphicsScene *scene): QGraphicsItem(0, scene), d(new Private)
{
	d->orientation = o;
	setAcceptsHoverEvents(true);
	setFlag ( QGraphicsItem::ItemIsMovable);
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

void KTGuideLine::paint( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
	painter->drawRect(boundingRect());
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

void KTGuideLine::hoverMoveEvent ( QGraphicsSceneHoverEvent * event)
{
	d->position = event->screenPos() - QPointF(1,1);
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
			globalPos = view->viewport()->mapToGlobal(mapToScene(scene()->sceneRect()).boundingRect().topLeft().toPoint());//FIXME: encontrar la posicion global verdadera
			
		}
	}
	
	if(d->orientation == Qt::Vertical)
	{
		QCursor::setPos((int)globalPos.x()+ boundingRect().width()/2, (int)QCursor::pos().y()) ;
	}
	else
	{
		QCursor::setPos((int)QCursor::pos().x(), (int)globalPos.y()+ boundingRect().height()/2);
	}
}

