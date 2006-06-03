/***************************************************************************
 *                                                                         *
 *  Copyright (C) 2000 Werner Trobin <trobin@kde.org>                      *
 *  Copyright (C) 2000,2006 David Faure <faure@kde.org>                    *
 *  Copyright (C) 2006 by David Cuadrado <krawek@toonka.com>               *
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

#ifndef DCOMMAND_H
#define DCOMMAND_H

#include <QList>
#include <QString>
#include <QObject>


class DAction;
class DActionManager;
class QMenu;


class DCommand
{
	protected:
		DCommand() {}

	public:
		virtual ~DCommand();
		virtual void execute() = 0;
		virtual void unexecute() = 0;
		virtual QString name() const = 0;

};

class DCommandHistory : public QObject 
{
	Q_OBJECT
	public:
		DCommandHistory(DActionManager *manager);

		virtual ~DCommandHistory();

		void clear();
		void addCommand(DCommand *command, bool execute=true);
		int undoLimit() const { return m_undoLimit; }
		void setUndoLimit(int limit);
		int redoLimit() const { return m_redoLimit; }
		void setRedoLimit(int limit);
		void updateActions();
		DCommand * presentCommand() const;
		bool isUndoAvailable() const;
		bool isRedoAvailable() const;
		QList<DCommand *> undoCommands( int maxCommands = 0 ) const;

		QList<DCommand *> redoCommands( int maxCommands = 0 ) const;
		
		DAction *undoAction();
		DAction *redoAction();

	public slots:
		virtual void undo();
		virtual void redo();
		virtual void documentSaved();

	signals:
		void commandExecuted(DCommand *command);
		void documentRestored();
		void modified();

	private:
		void clipCommands();  // ensures that the limits are kept

		QList<DCommand *> m_commands;
		DAction *m_undo, *m_redo;
		
		int m_undoLimit, m_redoLimit;
		
	private:
		class DCommandHistoryPrivate;
		DCommandHistoryPrivate * const d;
};

#endif
