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

#include "ktanimationrenderer.h"
#include "ktgraphicsscene.h"
#include "ktlayer.h"
#include "ktframe.h"
#include "ktscene.h"
#include "ktgraphicobject.h"

#include <QPainter>


struct KTAnimationRenderer::Private
{
	KTGraphicsScene *scene;
	int totalPhotograms;
	int currentPhotogram;
	
	Private() : scene(0), totalPhotograms(-1), currentPhotogram(0) {}
	
	~Private()
	{
		delete scene;
	}
	
	int calculateTotalPhotograms(KTScene *scene);
};


int KTAnimationRenderer::Private::calculateTotalPhotograms(KTScene *scene)
{
	Layers layers = scene->layers();
	Layers::iterator layerIterator = layers.begin();
	
	int total = 0;

	while ( layerIterator != layers.end() )
	{
		if( *layerIterator )
		{
			total = qMax(total, (*layerIterator)->frames().count());
		}
		++layerIterator;
	}
	
	return total;
}

KTAnimationRenderer::KTAnimationRenderer() : d(new Private)
{
	d->scene = new KTGraphicsScene;
	d->scene->setBackgroundBrush(Qt::white);
}


KTAnimationRenderer::~KTAnimationRenderer()
{
	delete d;
}

void KTAnimationRenderer::setScene(KTScene *scene)
{
	d->scene->setCurrentScene(scene);
	d->scene->setSceneRect(QRectF(QPointF(0,0), QSizeF( 500, 400 ) )); // FIXME: this isn't real size
	
	d->scene->drawPhotogram(0); // ###: Why whithout this don't work?
	d->currentPhotogram = -1;
	
	d->totalPhotograms = d->calculateTotalPhotograms(scene);
}

bool KTAnimationRenderer::nextPhotogram()
{
	if( d->totalPhotograms < 0 ) return false;
	
	d->currentPhotogram++;
	
	if( d->currentPhotogram == d->totalPhotograms )
		return false;
	
	d->scene->drawPhotogram(d->currentPhotogram);
	return true;
}

void KTAnimationRenderer::render(QPainter *painter)
{
	d->scene->render(painter, QRect(0, 0, painter->device()->width(), painter->device()->height()), d->scene->sceneRect().toRect(), Qt::IgnoreAspectRatio );
}

int KTAnimationRenderer::currentPhotogram() const
{
	return d->currentPhotogram;
}

int KTAnimationRenderer::totalPhotograms() const
{
	return d->totalPhotograms;
}

