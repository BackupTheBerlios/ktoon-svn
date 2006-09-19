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

#include "ktsceneevent.h"

#include "ddebug.h"

#include "dgradientadjuster.h"

KTAnimationArea::KTAnimationArea(KTProject *project, QWidget *parent) : QFrame(parent), m_project(project), m_draw(false), m_ciclicAnimation(false), m_currentFramePosition(0), m_isRendered(false), m_currentSceneIndex(-1)
{
	setAttribute(Qt::WA_StaticContents);

	m_renderCamera = QImage(size(), QImage::Format_RGB32);
	m_renderCamera.fill(qRgb(255, 255, 255));
	
	m_timer = new QTimer(this);
	
	connect(m_timer, SIGNAL(timeout()), this, SLOT(advance()));
	
	setCurrentScene( 0 );
}


KTAnimationArea::~KTAnimationArea()
{
}


void KTAnimationArea::paintEvent(QPaintEvent *e)
{
	QPainter painter;

	painter.begin(this);
	painter.drawImage(QPoint(0, 0), m_renderCamera);
}

void KTAnimationArea::play()
{
	dDebug() << "Playing!";
	
	m_draw = true;
	
	if ( m_project && !m_timer->isActive() )
	{
		render();
		m_timer->start(1000 / 14/*FIXME fps*/ );
	}
	
// 	emit toStatusBar( tr("Playing... "), 2000 );
}

void KTAnimationArea::stop()
{
	dDebug() << "Stopping";
	m_timer->stop();
// 	m_draw = false;
	m_currentFramePosition = 0;
// 	m_currentFrame = 0;
	repaint();
}


void KTAnimationArea::advance()
{
	if ( m_project )
	{
		if (m_ciclicAnimation && m_currentFramePosition >= m_photograms.count())
		{
			m_currentFramePosition = 0;
		}
		
		if ( m_currentFramePosition < m_photograms.count() )
		{
			m_renderCamera = m_photograms[m_currentFramePosition];
			repaint();
			m_currentFramePosition++;
		}
		else if ( !m_ciclicAnimation)
		{
			stop();
		}
	}
}

void KTAnimationArea::frameEvent(KTFrameEvent *event)
{
}

void KTAnimationArea::layerEvent(KTLayerEvent *event)
{
	
}

void KTAnimationArea::sceneEvent(KTSceneEvent *event)
{
	switch(event->action())
	{
		case KTProjectEvent::Select:
		{
			setCurrentScene( event->sceneIndex() );
		}
		break;
		case KTProjectEvent::Remove:
		{
			if ( event->sceneIndex() == m_currentSceneIndex )
			{
				setCurrentScene( m_currentSceneIndex-1 );
			}
		}
		break;
		default: break;
	}
}

void KTAnimationArea::projectEvent(KTProjectEvent *event)
{
	
}

void KTAnimationArea::itemEvent(KTItemEvent *event)
{
	
}


void KTAnimationArea::render() // TODO: Extend to scenes
{
	m_photograms.clear();
	
	KTScene *scene = m_project->scene( m_currentSceneIndex );
	
	if (!scene) return;
	
	Layers layers = scene->layers();
	
	int nPhotogramsRenderized = 0;
	
	m_isRendered = false; // TODO: for test, remove this
	int totalPhotograms = photogramsCount();
	
	emit toStatusBar( tr("Rendering... "),totalPhotograms*70 );
	
	
	while ( ! m_isRendered )
	{
		QGraphicsScene *scn = new QGraphicsScene(rect(), this);
		
		Layers::iterator layerIterator = layers.begin();
		bool ok = true;
		
		QImage renderized = QImage(size(), QImage::Format_RGB32);
		renderized.fill(qRgb(255, 255, 255));
		
		QPainter painter(&renderized);
		painter.setRenderHint(QPainter::Antialiasing);
		
		while ( layerIterator != layers.end() )
		{
			ok = ok && (nPhotogramsRenderized >= (*layerIterator)->frames().count()-1);
			
			if ( *layerIterator && nPhotogramsRenderized < (*layerIterator)->frames().count() && (*layerIterator)->isVisible() )
			{
				KTFrame *frame = (*layerIterator)->frames()[nPhotogramsRenderized];
				
				foreach(QGraphicsItem *item, frame->graphics())
				{
					scn->addItem(item);
				}
			}
			++layerIterator;
		}
		
		scn->render(&painter, renderized.rect(), scn->sceneRect() );
		
		emit progressStep( nPhotogramsRenderized, totalPhotograms);
		m_photograms << renderized;
		
		
		if (ok )
		{
			m_isRendered = true;
		}
		
		nPhotogramsRenderized++;
		
		foreach(QGraphicsItem *item, scn->items())
		{
			scn->removeItem(item);
		}
		
		delete scn;
		
		scene->drawCurrentPhotogram();
	}
}

int KTAnimationArea::photogramsCount() const
{
	KTScene *scene = m_project->scene( m_currentSceneIndex );
	
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
	return m_renderCamera.size();
}


void  KTAnimationArea::resizeEvent ( QResizeEvent * event )
{
	stop();
	m_renderCamera = QImage(size(), QImage::Format_RGB32);
// 	m_size = size();
	m_renderCamera.fill(qRgb(255, 255, 255));
	update();
}

void KTAnimationArea::setLoop(bool l)
{
	m_ciclicAnimation = l;
}

void KTAnimationArea::setCurrentScene(int index)
{
	m_currentSceneIndex = index;
}

KTScene *KTAnimationArea::currentScene() const
{
	return m_project->scene( m_currentSceneIndex );
}

