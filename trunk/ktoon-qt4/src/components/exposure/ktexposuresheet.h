/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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

#ifndef KTEXPOSURESHEET_H
#define KTEXPOSURESHEET_H

#include "ktmodulewidgetbase.h"
#include "kttableexposure.h"

// Qt
#include <QButtonGroup>
#include <QGroupBox>
#include <QPushButton>
#include <QStringList>
#include <QListWidget>
#include <QList>
#include <QListView>
#include <QActionGroup>
#include <QTabWidget>

#include "ktimagebutton.h"

/**
* @author Jorge Cuadrado
*/

class KTExposureSheet : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KTExposureSheet(QWidget *parent = 0);
		~KTExposureSheet();

		enum Actions { /*NoAction = 0,*/ InsertLayer = 0, RemoveLayer, ShowManageLayer, InsertFrames,  RemoveFrame, LockFrame,  MoveFrameUp, MoveFrameDown };
		
		void createFrame();
		
	private:
		QList<QPixmap> m_imgs;
		QGroupBox *m_buttonsPanel;
		QButtonGroup *m_buttonGroup;
		KTTableExposure *m_viewLayer;
		QListView *m_layerManager;
		QTabWidget *m_scenes;

	private:
		void setupButtons();
		
		void createLayerManager();
		
	public slots:
		void insertCurrentLayer();
		void removeCurrentLayer();
		void removeCurrentFrame();
		void moveUpFrame();
		void moveDownFrame();
		
		void applyAction(int action);
		
		void loadLayersAndKeyframes( QList<Layer*> layers );
		void updateLayersAndKeyframes();
		
		void addScene(const QString &name);
		void renameScene(const QString &name, int id);
		
	private slots:
		void actionButton(QAbstractButton *);
		
	signals:
		void requestInsertLayer();
		void requestRemoveLayer();
		
		void requestInsertFrame();
		
		void requestRemoveFrame();
		void requestMoveUpFrame();
		void requestMoveDownFrame();
		
// 		void applyedAction(Actions action);
		void frameSelected(int idLayer, int idFrame);
		void layerSelected(int);
// 		void layerInserted();
// 		void layerRemoved();
};

#endif
