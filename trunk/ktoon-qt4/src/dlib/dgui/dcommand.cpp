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

#include "dcommand.h"
#include <daction.h>
#include <dactionmanager.h>
#include <ddebug.h>

#include <QMenu>

DCommand::~DCommand()
{
}

class DCommandHistory::DCommandHistoryPrivate 
{
	public:
		DCommandHistoryPrivate() {
			m_savedAt = -1;
			m_current = -1;
		}
		~DCommandHistoryPrivate() {}
		int m_savedAt;
		int m_current;
    /*
		If m_commands contains:  <c0> <c1> <c2> <c3>

		m_current = 1 means we are between <c1> and <c2>, i.e. undo would unexecute c1.
		So m_current is the index of the current undo command, m_current+1 the current redo command if any.

		Adding a command at this point would delete <c2> and <c3>.
		m_current compared to the commands:  -1 <c0> 0 <c1> 1 <c2> 2.

		m_savedAt = 1 means that we where at m_current == 1 when the document was saved.
		m_savedAt = -1 means that the document was saved with an empty history (initial state, too).
		m_savedAt = -2 means that the document wasn't saved in the current visible history
		(this happens when the undo history got truncated)
    */
};

////////////

DCommandHistory::DCommandHistory(DActionManager *manager) : m_undoLimit(50), m_redoLimit(30), d( new DCommandHistoryPrivate )
{
	m_undo = new DAction(tr("Undo"), manager, "undo");
	
	m_undo->setShortcut(QKeySequence(tr("Ctrl+Z")));
	m_undo->setShortcutContext ( Qt::ApplicationShortcut );
	m_undo->setStatusTip(tr("Undoes the last draw action"));
	
	connect( m_undo, SIGNAL( triggered( ) ), this, SLOT( undo() ) );
	
	m_redo = new DAction(tr("Redo"), manager, "redo");
	
	m_redo->setShortcut(QKeySequence(tr("Ctrl+SHIFT+Z")));
	m_redo->setShortcutContext ( Qt::ApplicationShortcut );
	m_redo->setStatusTip(tr("Redoes a previous undone action"));
	
	connect( m_redo, SIGNAL( triggered () ), this, SLOT( redo() ) );
	
	clear();
}

DCommandHistory::~DCommandHistory() 
{
	qDeleteAll( m_commands );
	delete d;
}

void DCommandHistory::clear() 
{
	if ( m_undo ) 
	{
		m_undo->setEnabled(false);
		m_undo->setText( tr("&Undo") );
	}
	if ( m_redo ) 
	{
		m_redo->setEnabled(false);
		m_redo->setText( tr("&Redo") );
	}
	qDeleteAll( m_commands );
	m_commands.clear();
	d->m_current = -1;
	d->m_savedAt = -1;
}

void DCommandHistory::addCommand(DCommand *command, bool execute) 
{
	if ( !command )
		return;

	++d->m_current;
	m_commands.insert( d->m_current, command );
    // Truncate history
	int count = m_commands.count();
	for ( int i = d->m_current + 1; i < count; ++i )
		delete m_commands.takeLast();

    // Check whether we still can reach savedAt
	if ( d->m_current < d->m_savedAt )
		d->m_savedAt = -2;

	if ( m_undo ) {
		m_undo->setEnabled(true);
		m_undo->setText( tr("&Undo: %1").arg(command->name()) );
	}
	if ( m_redo && m_redo->isEnabled() ) {
		m_redo->setEnabled(false);
		m_redo->setText( tr("&Redo") );
	}
	clipCommands();

	if ( execute )
	{
		command->execute();
		emit commandExecuted(command);
	}
}

DCommand * DCommandHistory::presentCommand() const
{
	if ( d->m_current >= 0 )
		return m_commands[ d->m_current ];
	return 0;
}

void DCommandHistory::undo() {
	Q_ASSERT( d->m_current >= 0 );

	DCommand* command = m_commands[ d->m_current ];

	command->unexecute();
	emit commandExecuted( command );
	if ( m_redo ) 
	{
		m_redo->setEnabled(true);
		m_redo->setText( tr("&Redo: %1").arg(command->name()) );
	}

	--d->m_current;
	if ( d->m_current >= 0 ) {
        // undoing further is possible
		if (m_undo ) {
			DCommand* command = m_commands[ d->m_current ];
			m_undo->setEnabled(true);
			m_undo->setText( tr("&Undo: %1").arg(command->name()) );
		}
	} else {
        // undoing further is not possible
		if ( m_undo ) {
			m_undo->setEnabled(false);
			m_undo->setText( tr("&Undo") );
		}
	}

	if ( d->m_current == d->m_savedAt )
		emit documentRestored();

	clipCommands(); // only needed here and in addCommand, NOT in redo
	
	emit modified();
}

void DCommandHistory::redo() {
	DCommand* command = m_commands[ d->m_current + 1 ];
	command->execute();
	emit commandExecuted( command );

	if ( m_undo ) {
		m_undo->setEnabled(true);
		m_undo->setText( tr("&Undo: %1").arg(command->name()) );
	}

	++d->m_current;

	if ( d->m_current == d->m_savedAt )
		emit documentRestored();

	if ( isRedoAvailable() ) {
		if ( m_redo ) {
			command = m_commands[ d->m_current + 1 ];
			m_redo->setEnabled(true);
			m_redo->setText( tr("&Redo: %1").arg(command->name()) );
		}
	} else {
		if( m_redo ) {
			m_redo->setEnabled(false);
			m_redo->setText( tr("&Redo") );
		}
	}
	
	emit modified();
}

void DCommandHistory::documentSaved() {
	d->m_savedAt = d->m_current;
}

void DCommandHistory::setUndoLimit(int limit) {
	if ( limit>0 && limit != m_undoLimit ) {
		m_undoLimit = limit;
		clipCommands();
	}
}

void DCommandHistory::setRedoLimit(int limit) {
	if ( limit>0 && limit != m_redoLimit ) {
		m_redoLimit = limit;
		clipCommands();
	}
}

void DCommandHistory::clipCommands() {
	int count = m_commands.count();
	if ( count <= m_undoLimit && count <= m_redoLimit )
		return;

	if ( d->m_current >= m_undoLimit ) {
		const int toRemove = (d->m_current - m_undoLimit) + 1;
		for ( int i = 0; i < toRemove; ++i ) {
			delete m_commands.takeFirst();
			--d->m_savedAt;
			--d->m_current;
		}
		Q_ASSERT( d->m_current >= -1 );
		count = m_commands.count(); // update count for the redo branch below
		if ( d->m_savedAt < 0 )
			d->m_savedAt = -1; // savedAt went out of the history
	}

	if ( d->m_current + m_redoLimit + 1 < count ) {
		if ( d->m_savedAt > (d->m_current + m_redoLimit) )
			d->m_savedAt = -1;
		const int toRemove = count - (d->m_current + m_redoLimit + 1);
		for ( int i = 0; i< toRemove; ++i )
			delete m_commands.takeLast();
	}
}

void DCommandHistory::updateActions()
{
	if ( m_undo )
		m_undo->setEnabled( isUndoAvailable() );
	if ( m_redo )
		m_redo->setEnabled( isRedoAvailable() );
}

bool DCommandHistory::isUndoAvailable() const
{
	return d->m_current >= 0;
}

bool DCommandHistory::isRedoAvailable() const
{
	return d->m_current < m_commands.count() - 1;
}

QList<DCommand *> DCommandHistory::undoCommands( int maxCommands ) const
{
	QList<DCommand *> lst;
	for ( int i = d->m_current; i >= 0; --i ) {
		lst.append( m_commands[i] );
		if ( maxCommands > 0 && lst.count() == maxCommands )
			break;
	}
	return lst;
}

QList<DCommand *> DCommandHistory::redoCommands( int maxCommands ) const
{
	QList<DCommand *> lst;
	for ( int i = d->m_current + 1; i < m_commands.count(); ++i )
	{
		lst.append( m_commands[i] );
		if ( maxCommands > 0 && lst.count() == maxCommands )
			break;
	}
	return lst;
}


DAction *DCommandHistory::undoAction()
{
	return m_undo;
}

DAction *DCommandHistory::redoAction()
{
	return m_redo;
}
