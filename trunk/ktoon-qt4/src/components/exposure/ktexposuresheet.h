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
#include <QListWidget>
#include <QList>
#include <QActionGroup>

#include "kttabwidget.h"
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

		enum Actions { /*NoAction = 0,*/ InsertLayer = 0, RemoveLayer /*ShowManageLayer*/, InsertFrames,  RemoveFrame, LockFrame,  MoveFrameUp, MoveFrameDown };
		
		void addFrame(int idLayer, const QString &name, bool addedToEnd);
		
		void moveFrame(bool up);
		void removeCurrentLayer();
		void removeCurrentFrame();
		void lockCurrentFrame();
		void setScene(int index);
		
		
	private:
		QList<QPixmap> m_imgs;
		QList<KTTableExposure*> m_tables;
		QGroupBox *m_buttonsPanel;
		QButtonGroup *m_buttonGroup;
		KTTabWidget *m_scenes;
		KTTableExposure *m_currentTable;
		
	private:
		void setupButtons();
		
		void createLayerManager();
		
	public slots:
		void closeAllScenes();
		void insertLayer(const QString& name);
		void applyAction(int action);
		
// 		void loadLayersAndKeyframes( QList<Layer*> layers );
		void updateLayersAndKeyframes();
		
		void addScene(const QString &name);
		void renameScene(const QString &name, int id);
		
	private slots:
		void actionButton(QAbstractButton *);
		void emitRequestChangeScene(int index);
				
	signals:
		//layers
		void requestInsertLayer();
		void requestRemoveLayer();
		void requestInsertFrame(bool addedToEnd);
		void layerVisibilityChanged( int idLayer, bool value);
		
		//frames
		void requestRemoveFrame();
		void requestMoveFrame(bool up);
		void requestLockFrame();
		void requestCopyFrame(int);
		void requestPasteFrame(int);
		
		//scenes
		void requestChangeScene( int index );
		
		//others
		void frameSelected(int idLayer, int idFrame);
		void layerSelected(int);
		
		
		
		
};

#endif
