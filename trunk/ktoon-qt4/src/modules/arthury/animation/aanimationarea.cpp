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
#include "ktdebug.h"

AAnimationArea::AAnimationArea(QWidget *parent) : QFrame(parent), m_layer(0), m_currentFrame(0), m_draw(false), m_ciclicAnimation(false), m_fps(24), m_currentFramePosition(0)
{
	setAttribute(Qt::WA_StaticContents);

	m_renderCamera = QImage(520, 340, QImage::Format_RGB32);
	m_renderCamera.fill(qRgb(255, 255, 255));
	
	m_timer = new QTimer(this);
	
	connect(m_timer, SIGNAL(timeout()), this, SLOT(advance()));
	
}


AAnimationArea::~AAnimationArea()
{
}

void AAnimationArea::setLayer(KTLayer *layer)
{
	m_layer = layer;
	if ( m_layer->frames().count() > 0 )
	{
		m_currentFrame = m_layer->currentFrame();
	}
}

void AAnimationArea::paintEvent(QPaintEvent *e)
{
	QPainter painter;
	
	m_renderCamera.fill(qRgb(255, 255, 255));
	
	painter.begin(&m_renderCamera);
	painter.setRenderHint(QPainter::Antialiasing);
	
	painter.save();
	if ( m_draw )
	{
		drawFrames(&painter);
	}
	painter.restore();
	painter.end();
	
	painter.begin(this);
	painter.drawImage(QPoint(0, 0), m_renderCamera);
}

void AAnimationArea::drawFrames(QPainter *painter)
{
	if ( m_currentFrame )
	{
		QList<AGraphicComponent *> componentList = m_currentFrame->components();
		
		if ( componentList.count() > 0)
		{
			QList<AGraphicComponent *>::iterator it = componentList.begin();
			
			while ( it != componentList.end() )
			{
				if ( *it )
				{
					painter->save();
					
					painter->setPen((*it)->pen());
					painter->setBrush((*it)->brush());
					
					painter->drawPath((*it)->path());
					
					painter->restore();
				}
				++it;
			}
		}
	}
}

void AAnimationArea::play()
{
	ktDebug() << "Playing!" << endl;
	m_draw = true;
	if ( m_layer && !m_timer->isActive() )
	{
		m_timer->start(1000/m_fps);
	}
// 	m_draw = false;
}

void AAnimationArea::stop()
{
	ktDebug() << "Stopping" << endl;
	m_timer->stop();
// 	m_draw = false;
	m_currentFramePosition = 0;
// 	m_currentFrame = 0;
	repaint();
}


void AAnimationArea::advance()
{
// 	ktDebug() << "Advance" << endl;
	if ( m_layer )
	{
		KTKeyFrame *frame = m_layer->frames()[m_currentFramePosition];
		
		if ( m_currentFramePosition < m_layer->frames().count() &&  frame )
		{
			m_currentFrame = frame;
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

QSize AAnimationArea::sizeHint() const
{
	return m_renderCamera.size();
}

