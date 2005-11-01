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

#ifndef KTLAYERMANAGER_H
#define KTLAYERMANAGER_H

#include <QPushButton>
#include <QToolTip>
#include <QLabel>
#include <QScrollBar>

#include "ktvhbox.h"

#include "ktimagebutton.h"
#include "ktlayersequence.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTLayerManager : public KTVHBox
{
	Q_OBJECT
	public:
		enum Actions
		{
			NoAction = 0,
			ShowOutlines,
			LockLayers,
			ToggleLayerView,
			InsertLayer,
			RemoveLayer,
			MoveLayerUp,
			MoveLayerDown
		};
		
		/**
		 * Default constructor
		 * @param parent 
		 * @return 
		 */
		KTLayerManager(QWidget *parent = 0);
		~KTLayerManager();
		QScrollBar *verticalScrollBar();
		
		KTLayerSequence *layerSequence();
		
	private slots:
		/**
		 * Toggle the layer state
		 * @param  state
		 */
		void changeLayersState(int state);
		
		/**
		 * Select a layer action (Insert, remove, move up, move down...)
		 * @param action 
		 */
		void selectLayerAction(int action);
		
	signals:
		void actionSelected(int);
		
	private:
		double m_currentTime, m_totalTime;
		
		KTVHBox *m_utilsInTop;
		KTImageButton *m_lockButton, *m_eyeButton, *m_outlineButton;
		
		KTVHBox *m_utilsInBottom;
		KTImageButton *m_insertButton, *m_removeButton, *m_moveUpButton, *m_moveDownButton;
		
		QLabel *m_time;
		
		KTLayerSequence *m_sequence;
		
		bool m_allSelected, m_allVisible, m_allLock;
};

#endif
