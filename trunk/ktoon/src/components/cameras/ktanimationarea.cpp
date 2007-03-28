/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "ktanimationarea.h"


#include <QGraphicsItem>

#include "ktprojectresponse.h"
#include "ktgraphicobject.h"
#include "ktgraphicsscene.h"

#include <dcore/ddebug.h>
#include <dcore/dgradientadjuster.h>

struct KTAnimationArea::Private
{
	QFrame *container;
	QImage renderCamera;
	
	KTProject *project;
	
	bool draw, ciclicAnimation;
	
	int currentFramePosition;
	
	QTimer *timer;
	
	QList<QImage> photograms;
	bool isRendered;
	
	int currentSceneIndex;
	
	int fps;
};

KTAnimationArea::KTAnimationArea(KTProject *project, QWidget *parent) : QFrame(parent), d( new Private )
{
	d->project = project;
	
	d->draw = false;
	d->ciclicAnimation = false;
	d->isRendered = false;
	
	d->currentSceneIndex = -1;
	d->currentFramePosition = -1;
	
	d->fps = 14;
	
	setAttribute(Qt::WA_StaticContents);

	d->renderCamera = QImage(size(), QImage::Format_RGB32);
	d->renderCamera.fill(qRgb(255, 255, 255));
	
	d->timer = new QTimer(this);
	
	connect(d->timer, SIGNAL(timeout()), this, SLOT(advance()));
	
	setCurrentScene( 0 );
}


KTAnimationArea::~KTAnimationArea()
{
	delete d;
}

void KTAnimationArea::setFPS(int fps)
{
	d->fps = fps;
	
	if ( d->timer->isActive()  )
	{
		d->timer->stop();
		play();
	}
}


void KTAnimationArea::paintEvent(QPaintEvent *)
{
	if ( d->currentFramePosition >= 0 && d->currentFramePosition < d->photograms.count() )
	{
		d->renderCamera = d->photograms[d->currentFramePosition];
	}
	
	QPainter painter;
	
	painter.begin(this);
	painter.drawImage(QPoint(0, 0), d->renderCamera);
}

void KTAnimationArea::play()
{
	dDebug("camera") << "Playing!";
	
	d->draw = true;
	
	if ( d->project && !d->timer->isActive() )
	{
		if( !d->isRendered ) render();
		d->timer->start(1000 / d->fps);
	}
	
// 	emit toStatusBar( tr("Playing... "), 2000 );
}

void KTAnimationArea::stop()
{
	dDebug("camera") << "Stopping";
	d->timer->stop();
// 	d->draw = false;
	d->currentFramePosition = 0;
// 	d->currentFrame = 0;
	repaint();
}

void KTAnimationArea::nextFrame()
{
	if ( ! d->isRendered )
		render();
	
	if ( d->currentFramePosition >= d->photograms.count() ) return;
	
	d->currentFramePosition += 1;
	repaint();
}

void KTAnimationArea::previousFrame()
{
	if ( ! d->isRendered ) render();
	
	if ( d->currentFramePosition < 1 ) return;
	d->currentFramePosition -= 1;
	repaint();
}


void KTAnimationArea::advance()
{
	if ( d->project )
	{
		if (d->ciclicAnimation && d->currentFramePosition >= d->photograms.count())
		{
			d->currentFramePosition = 0;
		}
		
		if ( d->currentFramePosition < d->photograms.count() )
		{
			repaint();
			d->currentFramePosition++;
		}
		else if ( !d->ciclicAnimation)
		{
			stop();
		}
	}
}

void KTAnimationArea::frameResponse(KTFrameResponse *)
{
}

void KTAnimationArea::layerResponse(KTLayerResponse *)
{
}

void KTAnimationArea::sceneResponse(KTSceneResponse *event)
{
	switch(event->action())
	{
		case KTProjectRequest::Select:
		{
			setCurrentScene( event->sceneIndex() );
		}
		break;
		case KTProjectRequest::Remove:
		{
			if ( event->sceneIndex() == d->currentSceneIndex )
			{
				setCurrentScene( d->currentSceneIndex-1 );
			}
		}
		break;
		default: break;
	}
}

void KTAnimationArea::projectResponse(KTProjectResponse *)
{
}

void KTAnimationArea::itemResponse(KTItemResponse *)
{
}

void KTAnimationArea::libraryResponse(KTLibraryResponse *)
{
}

void KTAnimationArea::render() // TODO: Extend to scenes
{
	KTScene *scene = d->project->scene( d->currentSceneIndex );
	
	if (!scene) return;
	
	int totalPhotograms = photogramsCount();
	
	emit toStatusBar( tr("Rendering... "), totalPhotograms*70 );
	
	KTGraphicsScene *graphicScene = new KTGraphicsScene;
	graphicScene->setCurrentScene(scene);
	graphicScene->setBackgroundBrush(Qt::white);
	
	graphicScene->setSceneRect(QRectF(QPointF(0,0), QSizeF( 500, 400 ) )); // FIXME: this isn't real size
	
	graphicScene->drawPhotogram(0); // ###: Why whithout this don't work?
	
	d->photograms.clear();
	
	for(int nPhotogramsRenderized = 0; nPhotogramsRenderized < totalPhotograms; nPhotogramsRenderized++)
	{
		QImage renderized = QImage(size(), QImage::Format_RGB32);
		renderized.fill(qRgb(255, 255, 255));
		
		QPainter painter(&renderized);
		painter.setRenderHint(QPainter::Antialiasing);
		
		graphicScene->drawPhotogram(nPhotogramsRenderized);
		graphicScene->render(&painter, renderized.rect(), graphicScene->sceneRect().toRect(), Qt::IgnoreAspectRatio );
		
		d->photograms << renderized;
	}
	
	d->isRendered = true;
}

int KTAnimationArea::photogramsCount() const
{
	KTScene *scene = d->project->scene( d->currentSceneIndex );
	
	if (!scene) return -1;
	
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

QSize KTAnimationArea::sizeHint() const
{
	return d->renderCamera.size();
}


void  KTAnimationArea::resizeEvent ( QResizeEvent * e)
{
	QFrame::resizeEvent(e);
	
	stop();
	d->renderCamera = QImage(size(), QImage::Format_RGB32);
	d->renderCamera.fill(qRgb(255, 255, 255));
	
	d->isRendered = false;
	update();
}

void KTAnimationArea::setLoop(bool l)
{
	d->ciclicAnimation = l;
}

void KTAnimationArea::setCurrentScene(int index)
{
	d->currentSceneIndex = index;
	
}

KTScene *KTAnimationArea::currentScene() const
{
	return d->project->scene( d->currentSceneIndex );
}

