/***************************************************************************
 *   Copyright (C) 2004 by Toonka Films                                    *
 *   ktoon@toonka.com                                                      *
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

#include "ktcommandhistory.h"

//template <typename T>
static void clearPointerList (QValueList <KTCommand *> *listPtr)
{
    if (!listPtr)
        return;

    for (QValueList <KTCommand *>::iterator it = listPtr->begin ();
         it != listPtr->end ();
         it++)
    {
        delete (*it);
    }

    listPtr->clear ();
}

KTCommand::KTCommand(Ktoon *mainWindow) : ktoonMain(mainWindow)
{
}


KTCommand::~KTCommand()
{
}

/*******************
 * KTNamedCommand *
 *******************/
KTNamedCommand::KTNamedCommand (const QString &name, Ktoon *mainWindow)
    : KTCommand (mainWindow), m_name (name)
{
}

KTNamedCommand::~KTNamedCommand ()
{
}


// public virtual [base KTCommand]
QString KTNamedCommand::name () const
{
    return m_name;
}


/*******************
 *  KTMacroCommand  *
 *******************/

struct KTMacroCommandPrivate
{
};

KTMacroCommand::KTMacroCommand (const QString &name, Ktoon *mainWindow)
    : KTNamedCommand (name, mainWindow),
      d(new KTMacroCommandPrivate ())
{
}

KTMacroCommand::~KTMacroCommand ()
{
    clearPointerList(&m_commandList);
    delete d;
}


// public virtual [base KTCommand]
int KTMacroCommand::size () const
{
    int s = 0;
    
    for (QValueList <KTCommand *>::const_iterator it = m_commandList.begin ();
         it != m_commandList.end ();
         it++)
    {  
        if (s > INT_MAX - (*it)->size ())
        {
            s = INT_MAX;
            break;
        }
        else
        {
            s += (*it)->size ();
        }
    }

    return s;
}


// public virtual [base KTCommand]
void KTMacroCommand::execute ()
{
    for (QValueList <KTCommand *>::const_iterator it = m_commandList.begin ();
         it != m_commandList.end ();
         it++)
    {
        (*it)->execute ();
    }
}

// public virtual [base KTCommand]
void KTMacroCommand::unexecute ()
{
    QValueList <KTCommand *>::const_iterator it = m_commandList.end ();
    it--;

    while (it != m_commandList.end ())
    {
        (*it)->unexecute ();
        it--;
    }
}


// public
void KTMacroCommand::addCommand (KTCommand *command)
{
    m_commandList.push_back (command);
}

//
// KTCommandHistoryBase
//

struct KTCommandHistoryBasePrivate
{
};


KTCommandHistoryBase::KTCommandHistoryBase() : d ( new KTCommandHistoryBasePrivate () )
{
// 	m_actionUndo = new KToolBarPopupAction (undoActionText (),
// 			QString::fromLatin1 ("undo"),
// 			KStdAccel::shortcut (KStdAccel::Undo),
// 			this, SLOT (undo ()),
// 			ac, KStdAction::name (KStdAction::Undo));
// 
// 	m_actionRedo = new KToolBarPopupAction (redoActionText (),
// 			QString::fromLatin1 ("redo"),
// 			KStdAccel::shortcut (KStdAccel::Redo),
// 			this, SLOT (redo ()),
// 			ac, KStdAction::name (KStdAction::Redo));
// 	m_actionUndo->setEnabled (false);
// 	m_actionRedo->setEnabled (false);
// 
// 
// 	connect (m_actionUndo->popupMenu (), SIGNAL (activated (int)),
// 		 this, SLOT (undoUpToNumber (int)));
// 	connect (m_actionRedo->popupMenu (), SIGNAL (activated (int)),
// 		 this, SLOT (redoUpToNumber (int)));


// 	m_undoMinLimit = 10;
// 	m_undoMaxLimit = 500;
// 	m_undoMaxLimitSizeLimit = 16 * 1048576;
// 
// 	m_documentRestoredPosition = 0;
}

KTCommandHistoryBase::~KTCommandHistoryBase ()
{
	clearPointerList (&m_undoCommandList);
	clearPointerList (&m_redoCommandList);

	delete d;
}


// public
int KTCommandHistoryBase::undoLimit () const
{
	return undoMinLimit ();
}

// public
void KTCommandHistoryBase::setUndoLimit (int limit)
{
	setUndoMinLimit (limit);
}


// public
int KTCommandHistoryBase::undoMinLimit () const
{
	return m_undoMinLimit;
}

// public
void KTCommandHistoryBase::setUndoMinLimit (int limit)
{
    if (limit < 1 || limit > 5000/*"ought to be enough for anybody"*/)
    {
        qDebug("Algo esta mal");
        return;
    }

    if (limit == m_undoMinLimit)
	    return;

    m_undoMinLimit = limit;
    trimCommandListsUpdateActions ();
}


// public
int KTCommandHistoryBase::undoMaxLimit () const
{
	return m_undoMaxLimit;
}

// public
void KTCommandHistoryBase::setUndoMaxLimit (int limit)
{
    if (limit < 1 || limit > 5000/*"ought to be enough for anybody"*/)
{
    qDebug("--");
	return;
}

    if (limit == m_undoMaxLimit)
	    return;

    m_undoMaxLimit = limit;
    trimCommandListsUpdateActions ();
}


// public
int KTCommandHistoryBase::undoMaxLimitSizeLimit () const
{
	return m_undoMaxLimitSizeLimit;
}

// public
void KTCommandHistoryBase::setUndoMaxLimitSizeLimit (int sizeLimit)
{

    if (sizeLimit < 0 ||
        sizeLimit > (500 * 1048576)/*"ought to be enough for anybody"*/)
{
    qDebug("--");
	return;
}

    if (sizeLimit == m_undoMaxLimitSizeLimit)
	    return;

    m_undoMaxLimitSizeLimit = sizeLimit;
    trimCommandListsUpdateActions ();
}


// public
void KTCommandHistoryBase::addCommand (KTCommand *command, bool execute)
{

    if (execute)
        command->execute ();

    m_undoCommandList.push_front (command);
    clearPointerList (&m_redoCommandList);

    if (m_documentRestoredPosition != INT_MAX)
{
	if (m_documentRestoredPosition > 0)
		m_documentRestoredPosition = INT_MAX;
	else
		m_documentRestoredPosition--;
}

    trimCommandListsUpdateActions ();
}

// public
void KTCommandHistoryBase::clear ()
{

    clearPointerList (&m_undoCommandList);
    clearPointerList (&m_redoCommandList);

    m_documentRestoredPosition = 0;

    updateActions ();
}


// protected slot
void KTCommandHistoryBase::undoInternal ()
{

    KTCommand *undoCommand = nextUndoCommand ();
    if (!undoCommand)
	    return;

    undoCommand->unexecute ();


    m_undoCommandList.erase (m_undoCommandList.begin ());
    m_redoCommandList.push_front (undoCommand);

    if (m_documentRestoredPosition != INT_MAX)
{
	m_documentRestoredPosition++;
	if (m_documentRestoredPosition == 0)
		emit documentRestored ();
}
}

// protected slot
void KTCommandHistoryBase::redoInternal ()
{

    KTCommand *redoCommand = nextRedoCommand ();
    if (!redoCommand)
	    return;

    redoCommand->execute ();


    m_redoCommandList.erase (m_redoCommandList.begin ());
    m_undoCommandList.push_front (redoCommand);


    if (m_documentRestoredPosition != INT_MAX)
{
	m_documentRestoredPosition--;
	if (m_documentRestoredPosition == 0)
		emit documentRestored ();
}
}


// public slot virtual
void KTCommandHistoryBase::undo ()
{

    undoInternal ();
    trimCommandListsUpdateActions ();
}

// public slot virtual
void KTCommandHistoryBase::redo ()
{

    redoInternal ();
    trimCommandListsUpdateActions ();
}


// public slot virtual
void KTCommandHistoryBase::undoUpToNumber (int which)
{

    for (int i = 0;
         i <= which && !m_undoCommandList.isEmpty ();
	 i++)
{
	undoInternal ();
}

    trimCommandListsUpdateActions ();
}

// public slot virtual
void KTCommandHistoryBase::redoUpToNumber (int which)
{

    for (int i = 0;
         i <= which && !m_redoCommandList.isEmpty ();
	 i++)
{
	redoInternal ();
}

    trimCommandListsUpdateActions ();
}


// protected
QString KTCommandHistoryBase::undoActionText () const
{
	KTCommand *undoCommand = nextUndoCommand ();

	if (undoCommand)
		return tr ("&Undo: %1").arg (undoCommand->name ());
	else
		return tr ("&Undo");
}

// protected
QString KTCommandHistoryBase::redoActionText () const
{
	KTCommand *redoCommand = nextRedoCommand ();

	if (redoCommand)
		return tr ("&Redo: %1").arg (redoCommand->name ());
	else
		return tr ("&Redo");
}


// protected
void KTCommandHistoryBase::trimCommandListsUpdateActions ()
{
    trimCommandLists ();
    updateActions ();
}

// protected
/**
 * @param commandList 
 */
void KTCommandHistoryBase::trimCommandList (QValueList <KTCommand *> *commandList)
{
    if (!commandList)
    {
        qDebug("Passed 0 to commandList!!!");
	return;
    }

    if ((int) commandList->size () <= m_undoMinLimit)
    {
        return;
    }

    QValueList <KTCommand *>::iterator it = commandList->begin ();
    int upto = 0;

    int sizeSoFar = 0;

    while (it != commandList->end ())
    {
	bool advanceIt = true;

	if (sizeSoFar <= m_undoMaxLimitSizeLimit)
	{
		if (sizeSoFar > INT_MAX - (*it)->size ())
			sizeSoFar = INT_MAX;
		else
			sizeSoFar += (*it)->size ();
	}

        if (upto >= m_undoMinLimit)
        {
            if (upto >= m_undoMaxLimit || sizeSoFar > m_undoMaxLimitSizeLimit)
            {
                    delete (*it);
                    it = m_undoCommandList.erase (it);
                    advanceIt = false;
            }
        }

        if (advanceIt)
		it++;
	upto++;
    }
}

// protected
void KTCommandHistoryBase::trimCommandLists ()
{
    trimCommandList (&m_undoCommandList);
    trimCommandList (&m_redoCommandList);

    if (m_documentRestoredPosition != INT_MAX)
    {
        if (m_documentRestoredPosition > (int) m_redoCommandList.size () ||
            -m_documentRestoredPosition > (int) m_undoCommandList.size ())
        {
            m_documentRestoredPosition = INT_MAX;
        }
    }
}

// protected
void KTCommandHistoryBase::updateActions ()
{
//     m_actionUndo->setEnabled ((bool) nextUndoCommand ());
//     m_actionUndo->setText (undoActionText ());
// 
//     m_actionRedo->setEnabled ((bool) nextRedoCommand ());
//     m_actionRedo->setText (redoActionText ());
}


// public
KTCommand *KTCommandHistoryBase::nextUndoCommand () const
{
	if (m_undoCommandList.isEmpty ())
		return 0;

	return m_undoCommandList.first ();
}

// public
KTCommand *KTCommandHistoryBase::nextRedoCommand () const
{
	if (m_redoCommandList.isEmpty ())
		return 0;

	return m_redoCommandList.first ();
}


// public
void KTCommandHistoryBase::setNextUndoCommand (KTCommand *command)
{
    if (m_undoCommandList.isEmpty ())
        return;


    delete m_undoCommandList [0];
    m_undoCommandList [0] = command;


    trimCommandListsUpdateActions ();
}


// public slot virtual
void KTCommandHistoryBase::documentSaved ()
{
    m_documentRestoredPosition = 0;
}


//
// KTCommandHistory
//

KTCommandHistory::KTCommandHistory (Ktoon *mainWindow)
	: KTCommandHistoryBase (),
      m_mainWindow (mainWindow)
{
}

KTCommandHistory::~KTCommandHistory ()
{
}


// public slot virtual [base KCommandHistory]
void KTCommandHistory::undo ()
{
    if (m_mainWindow )
    {
    }
    else
	    KTCommandHistoryBase::undo ();
}

// public slot virtual [base KCommandHistory]
void KTCommandHistory::redo ()
{
	if (m_mainWindow )
	{
        // Not completely obvious but what else can we do?
		//
        // Ignoring the request would not be intuitive for tools like
        // Polygon & Polyline (where it's not always apparent to the user
        // that s/he's still drawing a shape even though the mouse isn't down
	}
	else
		KTCommandHistoryBase::redo ();
}

//#include "ktcommandhistory.moc"
