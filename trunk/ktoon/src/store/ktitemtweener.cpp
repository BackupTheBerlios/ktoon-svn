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
	
	QGraphicsItemAnimation *animation;
};

KTItemTweener::KTItemTweener(int frames, QObject *parent) : QObject(parent), d(new Private)
{
	d->animation = new QGraphicsItemAnimation(this);
	d->frames = frames;
}


KTItemTweener::~KTItemTweener()
{
	delete d;
}

// void KTItemTweener::afterAnimationStep( qreal step )
// {
// 	QGraphicsItem *item = this->item();
// 	if( item && step > 0 )
// 	{
// 		item->setFlag(QGraphicsItem::ItemIsMovable, false);
// 		item->setFlag(QGraphicsItem::ItemIsSelectable, false);
// 		item->setFlag(QGraphicsItem::ItemIsFocusable, false);
// 	}
// }

double KTItemTweener::horizontalScaleAt( int step ) const
{
	return d->animation->horizontalScaleAt( step / (double)d->frames );
}

double KTItemTweener::horizontalShearAt( int step ) const
{
	return d->animation->horizontalShearAt(step / (double)d->frames);
}

QGraphicsItem *KTItemTweener::item() const
{
	return d->animation->item();
}

QMatrix KTItemTweener::matrixAt( int step ) const
{
	return d->animation->matrixAt(step / (double)d->frames);
}

QPointF KTItemTweener::posAt( int step ) const
{
	return d->animation->posAt(step / (double)d->frames);
}

double KTItemTweener::rotationAt( int step ) const
{
	return d->animation->rotationAt(step / (double)d->frames);
}

void KTItemTweener::setItem( QGraphicsItem * item )
{
	d->animation->setItem(item);
}

void KTItemTweener::setPosAt( int step, const QPointF & point )
{
	d->animation->setPosAt(step / (double)d->frames, point);
}

void KTItemTweener::setRotationAt( int step, double angle )
{
	d->animation->setRotationAt(step / (double)d->frames, angle);
}

void KTItemTweener::setScaleAt( int step, double sx, double sy )
{
	d->animation->setScaleAt(step / (double)d->frames, sx, sy);
}

void KTItemTweener::setShearAt( int step, double sh, double sv )
{
	d->animation->setShearAt(step / (double)d->frames, sh, sv);
}

void KTItemTweener::setTranslationAt( int step, double dx, double dy )
{
	d->animation->setTranslationAt(step / (double)d->frames, dx, dy);
}

double KTItemTweener::verticalScaleAt( int step ) const
{
	return d->animation->verticalScaleAt(step / (double)d->frames);
}

double KTItemTweener::verticalShearAt( int step ) const
{
	return d->animation->verticalShearAt(step / (double)d->frames);
}

double KTItemTweener::xTranslationAt( int step ) const
{
	return d->animation->xTranslationAt(step / (double)d->frames);
}

double KTItemTweener::yTranslationAt( int step ) const
{
	return d->animation->yTranslationAt(step / (double)d->frames);
}


void KTItemTweener::setFrames(int frames)
{
	d->frames = frames;
}

int KTItemTweener::frames() const
{
	return d->frames;
}

void KTItemTweener::setStep( int step )
{
	d->animation->setStep(step / (double)d->frames);
}

