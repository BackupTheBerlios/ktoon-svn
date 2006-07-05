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
#include "ddebug.h"

#include "dgradientadjuster.h"

KTAnimationArea::KTAnimationArea(const QSize& size, QWidget *parent) : QFrame(parent), m_scene(0), m_draw(false), m_ciclicAnimation(false), m_currentFramePosition(0), m_size(size), m_isRendered(false)
{
	setAttribute(Qt::WA_StaticContents);

	m_renderCamera = QImage(size, QImage::Format_RGB32);
	m_renderCamera.fill(qRgb(255, 255, 255));
	
	m_timer = new QTimer(this);
	
	connect(m_timer, SIGNAL(timeout()), this, SLOT(advance()));
}


KTAnimationArea::~KTAnimationArea()
{
}

void KTAnimationArea::setScene(KTScene *scene)
{
	m_scene = scene;
	
	m_isRendered = false;
	
	emit sceneChanged( m_scene );
}

void KTAnimationArea::paintEvent(QPaintEvent *e)
{
	QPainter painter;

	painter.begin(this);
	painter.drawImage(QPoint(0, 0), m_renderCamera);
}

void KTAnimationArea::drawFrames(QPainter *painter)
{
// 	QList<KTFrame *> frames = m_scene->frames();
// 	QList<KTFrame *>::iterator frameIterator = frames.begin();
// 	
// 	while ( frameIterator != frames.end() )
// 	{
// 		if ( m_currentFrame )
// 		{
// 			QList<KTGraphicComponent *> componentList = m_currentFrame->components();
// 			
// 			if ( componentList.count() > 0)
// 			{
// 				QList<KTGraphicComponent *>::iterator it = componentList.begin();
// 				
// 				while ( it != componentList.end() )
// 				{
// 					if ( *it )
// 					{
// 						painter->save();
// 						
// 						painter->setPen((*it)->pen());
// 						painter->setBrush((*it)->brush());
// 						
// 						painter->drawPath((*it)->path());
// 						
// 						painter->restore();
// 					}
// 					++it;
// 				}
// 			}
// 		}
// 	}
}

void KTAnimationArea::play()
{
	dDebug() << "Playing!";
	
	m_draw = true;
	
	if ( m_scene && !m_timer->isActive() )
	{
		render();
		m_timer->start(1000/m_scene->fps() );
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
	if ( m_scene )
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

void KTAnimationArea::render() // TODO: Extend to scenes
{
	m_photograms.clear();
	
	Layers layers = m_scene->layers();
	
	int nPhotogramsRenderized = 0;
	
	m_isRendered = false; // TODO: for test
	int totalPhotograms = photogramsCount();
	
	emit toStatusBar( tr("Rendering... "),totalPhotograms*70 );
	
	while ( ! m_isRendered )
	{
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
				if ( frame )
				{
// 					QList<KTGraphicComponent *> componentList = frame->components();
// 					
// 					if ( componentList.count() > 0  )
// 					{
// 						QList<KTGraphicComponent *>::iterator it = componentList.begin();
// 										
// 						while ( it != componentList.end() )
// 						{
// 							renderGraphic(*it, &painter);
// 							++it;
// 						}
// 					}
				}
			}
			++layerIterator;
		}
		
		emit progressStep( nPhotogramsRenderized, totalPhotograms);
		m_photograms << renderized;
		
		
		if (ok )
		{
			m_isRendered = true;
		}
		
		nPhotogramsRenderized++;
	}
}

// void KTAnimationArea::renderGraphic(KTGraphicComponent *graphicComponent, QPainter *painter )
// {
// 	painter->save();
// 	float sx = 1, sy = 1, offset =0;
// 	sx = static_cast<float>(size().width()-offset) / static_cast<float>(m_size.width());
// 
// 	sy = static_cast<float>(size().height()-offset) / static_cast<float>(m_size.height());
// 
// 	painter->scale(sx,sy);
// 	
// 	graphicComponent->draw(painter);
// 	painter->restore();
// }

int KTAnimationArea::photogramsCount() const
{
	Layers layers = m_scene->layers();
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

const KTScene *KTAnimationArea::currentScene() const
{
	return m_scene;
}
