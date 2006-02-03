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
		KTLayerManager *currentLayerManager();
		KTFrameSequenceContainer *currentFrameContainer();
		
	private:
		void setupPropertiesBar();
		
	private slots:
		void emitNewFPS(const QString &);
		
	signals:
		void fpsChanged(int);
// 		void requestChangeScene( int );
		
	public slots:
		void execAction(int action);
		
		
		void insertLayer(const QString &name, bool toEnd = true);
		
		void addScene(const QString &name);
		void setScene(int position);
		void removeCurrentLayer();
		void removeLayer(int index);
		void insertFrame(int position, const QString &name, bool toEnd);
		void moveFrame(bool up);
		void removeCurrentFrame();
		void lockCurrentFrame();
		void setCurrentCell(int layer, int frame);
		void selectCurrentLayer(int index);
		void setLayer(int index);
	private slots:
		void emitFrameSelected(int layer, int frame);
		
	signals:
		void requestInsertFrame(bool);
		void requestInsertLayer();
		void frameSelected( int, int );
		void layerVisibilityChanged( int, bool);
		void layerSelected( int);
		void requestCopyFrame(int);
		void requestPasteFrame(int);
		void requestMoveFrame(bool);
		void requestRemoveFrame();
		void requestLockFrame();
		void requestRemoveLayer();
		
		void requestChangeFPS(int);
		
	private slots:
		
		
	private:
		QStackedWidget *m_container;
		KTVHBox *m_propertiesBar;
		KTELabel *m_editLayer;
};

#endif
