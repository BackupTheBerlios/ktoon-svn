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
#ifndef AANIMATIONAREA_H
#define AANIMATIONAREA_H

#include <QImage>
#include <QPainter>
#include <QPaintEvent>
#include <QTimer>
#include <QFrame>

#include "ktlayer.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class AAnimationArea : public QFrame
{
	Q_OBJECT
	public:
		AAnimationArea(QWidget *parent = 0);
		~AAnimationArea();
		void setLayer(KTLayer *layer);
		
		QSize sizeHint() const;
		
	public slots:
		virtual void render();
		virtual void play();
		virtual void stop();
		
	private slots:
		void advance();
		
	signals:
		void progressStep(int, int);
		void toStatusBar(const QString &, int);
		
	protected:
		void paintEvent(QPaintEvent *e);
		virtual void drawFrames(QPainter *painter);
		
	private:
		QFrame *m_container;
		QImage m_renderCamera;
		
		KTKeyFrame *m_currentFrame;
		KTLayer *m_layer;
		
		bool m_draw, m_ciclicAnimation;
		
		int m_fps, m_currentFramePosition;
		
		QTimer *m_timer;
		
		QList<QImage> m_photograms;
};

#endif
