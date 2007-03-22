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

#ifndef DCOMMANDHISTORY_H
#define DCOMMANDHISTORY_H

#include <QObject>
#include <QHash>

class QUndoStack;
class QMenu;
class QAction;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class DCommandHistory : public QObject
{
	Q_OBJECT;
	
	public:
		DCommandHistory(QUndoStack *stack, QObject *parent = 0);
		~DCommandHistory();
		
		QAction *redoAction() const;
		QAction *undoAction() const;
		
		QUndoStack *stack() const;
		
	public slots:
		void enableRedoMenu(bool e);
		void enableUndoMenu(bool e);
		
		void undo();
		void redo();
		
	private:
		void updateMenu();
		
	private slots:
		void updateFromIndex(int idx);
		void undoFromAction(QAction *a);
		void redoFromAction(QAction *a);
		
	private:
		QUndoStack *m_stack;
		QMenu *m_redoMenu;
		QMenu *m_undoMenu;
		
		int m_currentIndex;
		
		QHash<int, QAction *> m_actions;
		
		bool m_isLastRedo;
};

#endif



