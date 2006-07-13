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

#include <QStackedWidget>

class KTLayerManager;
class KTFramesTable;
class KTProjectActionBar;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTTimeLine : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTTimeLine(QWidget *parent = 0);
		~KTTimeLine();
		void closeAllScenes();
		
	private:
		KTLayerManager *layerManager(int sceneIndex);
		KTFramesTable *framesTable(int sceneIndex);
		
	protected:
		void sceneEvent(KTSceneEvent *e);
		void layerEvent(KTLayerEvent *e);
		void frameEvent(KTFrameEvent *e);
		
	public slots:
		void insertScene(int position, const QString &name);
		void removeScene(int position);
		
		
		
	private slots:
		void requestCommand(int action);
		void emitRequestRenameLayer(int layer, const QString &name);
		
	private:
		QStackedWidget *m_container;
		
		KTProjectActionBar *m_actionBar;
};

#endif
