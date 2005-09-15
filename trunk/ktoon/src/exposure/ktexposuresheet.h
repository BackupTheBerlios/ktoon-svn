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

#include "ktdialogbase.h"
#include "kttableexposure.h"

#include <qhbuttongroup.h>
#include <qpushbutton.h>
#include <qaction.h>
#include <qvaluelist.h>
#include <qstringlist.h>
#include <qtable.h>
#include <qgridview.h>

#include <qlistview.h>

#include "ktimagebutton.h"

// #include "layer.h"
/**
* @author Jorge Cuadrado
*/


typedef QValueList<QPixmap> imgs;

class KTExposureSheet : public KTDialogBase
{
	
	Q_OBJECT
	public:
		KTExposureSheet(QWidget *parent = 0, const char *name = 0);
		~KTExposureSheet();

		enum Actions { /*NoAction = 0,*/ InsertLayer = 0, RemoveLayer, ShowManageLayer, InsertFrames,  RemoveFrame, LockFrame,  MoveFrameUp, MoveFrameDown };
		
	private:
		imgs m_imgs;
		QButtonGroup *buttonsPanel;
		QActionGroup *m_actions;
		KTTableExposure *m_viewLayer;
// 		//Widget for handling the layer visibility
		QListView *m_layerManager;

	private:
		void setupButtons();
		
		void createLayerManager();
		
	public slots:
		void applyAction(int action);
		void loadLayersAndKeyframes( QPtrList<Layer> layers );
		void updateLayersAndKeyframes();
		
	signals:
		void applyedAction(Actions action);
		void frameSelected();
		void layerSelected(int);
		
		
		void layerInserted();
		
		void layerRemoved();
		
		
		
		void layerRenamed( int pos, const QString &name );
		void framesInsertedAtTheEnd( int number );
		void frameRemoved( int pos );
		void frameMovedUp( int pos );
		void frameMovedDown( int pos );
		
		
};

#endif
