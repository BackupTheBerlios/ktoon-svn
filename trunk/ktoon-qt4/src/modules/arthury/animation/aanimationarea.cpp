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

#include "aanimationarea.h"
#include "ddebug.h"

#include "dgradientadjuster.h"

AAnimationArea::AAnimationArea(const QSize& size, QWidget *parent) : QFrame(parent), m_scene(0), m_draw(false), m_ciclicAnimation(false), m_currentFramePosition(0), m_isRendered(false)
{
	setAttribute(Qt::WA_StaticContents);

	m_renderCamera = QImage(size, QImage::Format_RGB32);
	m_renderCamera.fill(qRgb(255, 255, 255));
	
	m_timer = new QTimer(this);
	
	connect(m_timer, SIGNAL(timeout()), this, SLOT(advance()));
}


AAnimationArea::~AAnimationArea()
{
}

void AAnimationArea::setScene(KTScene *scene)
{
	m_scene = scene;
// 	if ( m_scene->frames().count() > 0 )
// 	{
// 		m_currentFrame = m_scene->currentFrame();
// 	}
	m_isRendered = false;
}

void AAnimationArea::paintEvent(QPaintEvent *e)
{
	QPainter painter;

	painter.begin(this);
	painter.drawImage(QPoint(0, 0), m_renderCamera);
}

void AAnimationArea::drawFrames(QPainter *painter)
{
// 	QList<KTKeyFrame *> frames = m_scene->frames();
// 	QList<KTKeyFrame *>::iterator frameIterator = frames.begin();
// 	
// 	while ( frameIterator != frames.end() )
// 	{
// 		if ( m_currentFrame )
// 		{
// 			QList<AGraphicComponent *> componentList = m_currentFrame->components();
// 			
// 			if ( componentList.count() > 0)
// 			{
// 				QList<AGraphicComponent *>::iterator it = componentList.begin();
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

void AAnimationArea::play()
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

void AAnimationArea::stop()
{
	dDebug() << "Stopping";
	m_timer->stop();
// 	m_draw = false;
	m_currentFramePosition = 0;
// 	m_currentFrame = 0;
	repaint();
}


void AAnimationArea::advance()
{
	if ( m_scene )
	{
		if ( m_currentFramePosition < m_photograms.count() )
		{
			m_renderCamera = m_photograms[m_currentFramePosition];
			repaint();
			m_currentFramePosition++;
		}
		else
		{
			if (m_ciclicAnimation)
			{
				m_currentFramePosition = 0;
			}
			else
			{
				stop();
			}
// 			repaint();
		}
	}
}

void AAnimationArea::render() // TODO: Extend to scenes
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
		
		QImage renderized = QImage(520, 340, QImage::Format_RGB32);
		renderized.fill(qRgb(255, 255, 255));
		
		QPainter painter(&renderized);
		painter.setRenderHint(QPainter::Antialiasing);
		
		while ( layerIterator != layers.end() )
		{
			ok = ok && (nPhotogramsRenderized > (*layerIterator)->frames().count());
			
			if ( *layerIterator && nPhotogramsRenderized < (*layerIterator)->frames().count() && (*layerIterator)->isVisible() )
			{
				KTKeyFrame *frame = (*layerIterator)->frames()[nPhotogramsRenderized];
				if ( frame )
				{
					QList<AGraphicComponent *> componentList = frame->components();
					
					if ( componentList.count() > 0  )
					{
						QList<AGraphicComponent *>::iterator it = componentList.begin();
												
						while ( it != componentList.end() )
						{
							renderGraphic(*it, &painter);
							++it;
						}
					}
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


// 				Frames frames = (*layerIterator)->frames();
// 				
// 				int nphotograms = frames.count();
// 				
// 				emit toStatusBar( tr("Rendering... "), nphotograms*70 );
// 				
// 				for(int i = 0; i < nphotograms; i++)
// 				{
// 					QPainter painter(&renderized);
// 					painter.setRenderHint(QPainter::Antialiasing);
// 					
// 					KTKeyFrame *frame = frames[i];
// 					if ( frame )
// 					{
// 						QList<AGraphicComponent *> componentList = frame->components();
// 						
// 						if ( componentList.count() > 0  )
// 						{
// 							QList<AGraphicComponent *>::iterator it = componentList.begin();
// 							
// 							while ( it != componentList.end() )
// 							{
// 								if ( *it )
// 								{
// 									painter.save();
// 									
// 									painter.setPen((*it)->pen());
// 									painter.setBrush((*it)->brush());
// 									
// 									painter.drawPath((*it)->path());
// 									
// 									painter.restore();
// 								}
// 								++it;
// 							}
// 						}
// 						
// 						emit progressStep( i+1, nphotograms);
// 						m_photograms << renderized;
// 					}
// 				}
// 			}
// 		}
// 	}
}

void AAnimationArea::renderGraphic(const AGraphicComponent *graphicComponent, QPainter *painter )
{
	painter->save();
	foreach(AGraphic *graphic, graphicComponent->graphics())
	{
		QPen pen = graphic->pen;
		QBrush brush = graphic->brush;
		
		painter->setPen(pen);
		painter->setBrush(brush);
		
		QList<QPolygonF> poligons = graphic->path.toSubpathPolygons();
		
		if ( poligons.count() == 1 )
		{
			painter->drawPath(graphic->path);
		}
		else
		{
			QList<QPolygonF>::const_iterator it;
			for(it = poligons.begin(); it != poligons.end(); ++it)
			{
				painter->drawPolygon(*it);
			}
		}
	}

	const QList< AGraphicComponent *> childs = graphicComponent->childs();
	if(childs.count() > 0)
	{
		foreach(AGraphicComponent *child, childs)
		{
			renderGraphic(child, painter);
		}
	}
	
	
	painter->restore();
}

int AAnimationArea::photogramsCount() const
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

QSize AAnimationArea::sizeHint() const
{
	return m_renderCamera.size();
}

void AAnimationArea::setSize(const QSize& size)
{
	m_renderCamera = QImage(size, QImage::Format_RGB32);
	m_renderCamera.fill(qRgb(255, 255, 255));
	update();
}

