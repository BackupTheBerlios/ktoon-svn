/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "ktitemtweener.h"

#include <QGraphicsItem>

struct KTItemTweener::Private
{
	Private() : frames(0) {}
	
	int frames;
};

KTItemTweener::KTItemTweener(QObject *parent) : QGraphicsItemAnimation(parent), d(new Private)
{
}


KTItemTweener::~KTItemTweener()
{
	delete d;
}

void KTItemTweener::afterAnimationStep( qreal step )
{
	QGraphicsItem *item = this->item();
	if( item && step > 0 )
	{
		item->setFlag(QGraphicsItem::ItemIsMovable, false);
		item->setFlag(QGraphicsItem::ItemIsSelectable, false);
		item->setFlag(QGraphicsItem::ItemIsFocusable, false);
	}
}

void KTItemTweener::setFrames(int frames)
{
	d->frames = frames;
}

int KTItemTweener::frames() const
{
	return d->frames;
}

