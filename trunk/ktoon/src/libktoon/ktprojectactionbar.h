/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#ifndef KTPROJECTACTIONBAR_H
#define KTPROJECTACTIONBAR_H

#include <qwidget.h>

#include <QButtonGroup>

class QToolButton;
class QBoxLayout;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class KTProjectActionBar : public QWidget
{
	Q_OBJECT;
	
	public:
		enum Action
		{
			NoAction = 0x00,
			
			InsertFrame = 1 << 1,
			RemoveFrame = 1 << 2,
			MoveFrameUp = 1 << 3,
			MoveFrameDown = 1 << 4,
			
			InsertLayer = 1 << 5,
			RemoveLayer = 1 << 6,
			MoveLayerUp = 1 << 7,
			MoveLayerDown = 1 << 8,
			
			InsertScene = 1 << 9,
			RemoveScene = 1 << 10,
			MoveSceneUp = 1 << 11,
			MoveSceneDown = 1 << 12,
			
			AllActions = InsertFrame | RemoveFrame | MoveFrameUp | MoveFrameDown | InsertLayer | RemoveLayer | MoveLayerUp | MoveLayerDown | InsertScene | RemoveScene | MoveSceneUp | MoveSceneDown,
			FrameActions = InsertFrame | RemoveFrame | MoveFrameUp | MoveFrameDown,
			LayerActions = InsertLayer | RemoveLayer | MoveLayerUp | MoveLayerDown,
			SceneActions = InsertScene | RemoveScene | MoveSceneUp | MoveSceneDown
		};
		
		Q_DECLARE_FLAGS(Actions, Action);
		
		KTProjectActionBar(Actions actions, Qt::Orientation orientation = Qt::Horizontal, QWidget *parent = 0);
		~KTProjectActionBar();
		void setFixedSize(int s);
		
		void insertSeparator(int position);
		
		QToolButton *button(Action action);
		
		
	signals:
		void actionSelected(int action);
		
	private:
		void setup(Actions actions);
		
	private:
		Qt::Orientation m_orientation;
		
		int m_fixedSize;
		QButtonGroup m_actions;
		
		QBoxLayout *m_buttonLayout;
};

Q_DECLARE_OPERATORS_FOR_FLAGS(KTProjectActionBar::Actions);

#endif
