/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
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

#include <QGraphicsItemAnimation>
#include <QGraphicsItem>

#include <dcore/ddebug.h>

#define VERIFY_STEP(s) if( s > d->frames || d->frames == 0) { \
                         dWarning("items") << "Invalid step " << s << " for tweening, maximun step is " << d->frames << "; In " << __FUNCTION__; \
                         return; }

#define STEP(s) s / (double)d->frames

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
	return d->animation->horizontalScaleAt( STEP(step) );
}

double KTItemTweener::horizontalShearAt( int step ) const
{
	return d->animation->horizontalShearAt(STEP(step));
}

QGraphicsItem *KTItemTweener::item() const
{
	return d->animation->item();
}

QMatrix KTItemTweener::matrixAt( int step ) const
{
	return d->animation->matrixAt(STEP(step));
}

QPointF KTItemTweener::posAt( int step ) const
{
	return d->animation->posAt(STEP(step));
}

double KTItemTweener::rotationAt( int step ) const
{
	return d->animation->rotationAt(STEP(step));
}

void KTItemTweener::setItem( QGraphicsItem * item )
{
	d->animation->setItem(item);
}

void KTItemTweener::setPosAt( int step, const QPointF & point )
{
	VERIFY_STEP(step);
	d->animation->setPosAt(STEP(step), point);
}

void KTItemTweener::setRotationAt( int step, double angle )
{
	VERIFY_STEP(step);
	d->animation->setRotationAt(STEP(step), angle);
}

void KTItemTweener::setScaleAt( int step, double sx, double sy )
{
	VERIFY_STEP(step);
	d->animation->setScaleAt(STEP(step), sx, sy);
}

void KTItemTweener::setShearAt( int step, double sh, double sv )
{
	VERIFY_STEP(step);
	d->animation->setShearAt(STEP(step), sh, sv);
}

void KTItemTweener::setTranslationAt( int step, double dx, double dy )
{
	VERIFY_STEP(step);
	d->animation->setTranslationAt(STEP(step), dx, dy);
}

double KTItemTweener::verticalScaleAt( int step ) const
{
	return d->animation->verticalScaleAt(STEP(step));
}

double KTItemTweener::verticalShearAt( int step ) const
{
	return d->animation->verticalShearAt(STEP(step));
}

double KTItemTweener::xTranslationAt( int step ) const
{
	return d->animation->xTranslationAt(STEP(step));
}

double KTItemTweener::yTranslationAt( int step ) const
{
	return d->animation->yTranslationAt(STEP(step));
}

void KTItemTweener::addStep(const KTTweenerStep &step)
{
	int n = step.n();
	
	VERIFY_STEP(n);
	
	if(step.has(KTTweenerStep::Position) )
	{
		setPosAt(n, step.position());
	}
	
	if(step.has(KTTweenerStep::Scale) )
	{
		setScaleAt(n, step.horizontalScale(), step.verticalScale());
	}
	
	if(step.has(KTTweenerStep::Translation) )
	{
		setTranslationAt(n, step.xTranslation(), step.yTranslation());
	}
	
	if(step.has(KTTweenerStep::Shear) )
	{
		setScaleAt(n, step.horizontalShear(), step.verticalShear());
	}
	
	if(step.has(KTTweenerStep::Rotation) )
	{
		setRotationAt(n, step.rotation());
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

void KTItemTweener::setStep( int step )
{
	VERIFY_STEP(step);
	d->animation->setStep(STEP(step));
}

