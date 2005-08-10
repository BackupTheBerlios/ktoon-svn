/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado   *
 *   kuadrosx@toonka.com   *
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


#include <qhbuttongroup.h>
#include <qpushbutton.h>
#include <qaction.h>
#include <qvaluelist.h>
#include <qstringlist.h>


#include "kttableexposure.h"
#include <qtable.h>
#include <qlistbox.h>
#include <qgridview.h> 

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
		enum Actions { InsertLayer = 0, RemoveLayer, ShowManageLayer, InsertFrames,  RemoveFrame, LockFrame,  MoveFrameUp, MoveFrameDown };
	
	private:

		imgs m_imgs;
		QHButtonGroup *buttonsPanel;
		QActionGroup *m_actions;
		KTTableExposure *m_viewLayer;
		
	private:
		void setupButtons();
		
	public slots:
		void applyAction(int action);
		void slotInsertLayer();
		
	signals:
		void applyedAction(Actions action);
};

#endif
