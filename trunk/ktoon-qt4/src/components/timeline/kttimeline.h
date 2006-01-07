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

#ifndef KTTIMELINE_H
#define KTTIMELINE_H

#include <ktmodulewidgetbase.h>
#include <QSplitter>

#include "ktlayermanager.h"
#include "ktframesequencecontainer.h"

#include <QStackedWidget>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTTimeLine : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTTimeLine(QWidget *parent = 0);
		~KTTimeLine();
		void setScene(int index);
		KTLayerManager *currentLayerManager();
		KTFrameSequenceContainer *currentFrameContainer();
		void addFrame(int layerId, const QString &name, bool addToEnd );
		
	private:
		void setupPropertiesBar();
		
	private slots:
		void emitNewFPS(const QString &);
		
	signals:
		void fpsChanged(int);
// 		void requestChangeScene( int );
		
	public slots:
		void execAction(int action);
		void createLayer(const QString &name, bool toEnd = true);
		void removeCurrentLayer();
		
		void addScene(const QString &name);
		
	signals:
		void requestInsertLayer();
		void requestRemoveLayer();
		
		void requestChangeFPS(int fps);
		
		void layerSelected(int );
		
	private slots:
		void selectCurrentLayer(int index);
		
	private:
		QStackedWidget *m_container;
		KTVHBox *m_propertiesBar;
		KTELabel *m_editLayer;
};

#endif
