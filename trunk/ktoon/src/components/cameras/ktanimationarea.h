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

#include "ktscene.h"
#include "ktglobal.h"
#include "ktabstractprojectrequesthandler.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTOON_EXPORT KTAnimationArea : public QFrame, public KTAbstractProjectRequestHandler
{
	Q_OBJECT
	public:
		KTAnimationArea(KTProject *project, QWidget *parent = 0);
		~KTAnimationArea();
		
		QSize sizeHint() const;
		int photogramsCount() const;
		
		void setLoop(bool l);
		
		void setCurrentScene(int index);
		
		KTScene *currentScene() const;
		
		void setFPS(int fps);
		
	public slots:
		virtual void render();
		virtual void play();
		virtual void stop();
		
		
	private slots:
		void advance();
		
	protected:
		void frameRequest(KTProjectRequest *event);
		void layerRequest(KTProjectRequest *event);
		void sceneRequest(KTProjectRequest *event);
		void projectRequest(KTProjectRequest *event);
		void itemRequest(KTProjectRequest *event);
		void libraryRequest(KTProjectRequest *request);
		
	signals:
		void progressStep(int, int);
		void toStatusBar(const QString &, int);
		
		void sceneChanged(const KTScene *newScene );
		
		void requestTriggered(const KTProjectRequest *event);
		
	protected:
		void paintEvent(QPaintEvent *e);
		void resizeEvent ( QResizeEvent * event );
		
	private:
		QFrame *m_container;
		QImage m_renderCamera;
		
		KTProject *m_project;
		
		bool m_draw, m_ciclicAnimation;
		
		int m_currentFramePosition;
		
		QTimer *m_timer;
		
		QList<QImage> m_photograms;
		bool m_isRendered;
		
		int m_currentSceneIndex;
		
		int m_fps;
};

#endif
