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

#ifndef KTCOMMANDHISTORY_H
#define KTCOMMANDHISTORY_H

#include <ktoon.h>

class Ktoon;

/**
 * TODO: Add documentation
 * @author Toonka Films
*/
class KTCommand
{
    public:
        KTCommand(Ktoon *mainWindow);
        virtual ~KTCommand(); // base class

    virtual QString name () const = 0;

    // Returns the estimated size in bytes.
    //
    // You only have to factor in the size of variables that change according
    // to the amount of input e.g. pixmap size, text size.  There is no need
    // to include the size of O(1) variables unless they are huge.
    //
    // If in doubt, return the largest possible amount of memory that your
    // command will take.  This is better than making the user unexpectedly
    // run out of memory.
    virtual int size () const = 0;

    virtual void execute () = 0;
    virtual void unexecute () = 0;
    
    protected:
	Ktoon *mainWindow () const;

        // TODO: functions for return document, selection and the viewManager!! all consts

    protected:
	Ktoon *ktoonMain;
};

class KTNamedCommand : public KTCommand
{
    public:
        KTNamedCommand (const QString &name, Ktoon *mainWindow);
        virtual ~KTNamedCommand ();

        virtual QString name () const;

    protected:
        QString m_name;
};

class KTMacroCommand : public KTNamedCommand
{
    public:
        KTMacroCommand (const QString &name, Ktoon *mainWindow);
        virtual ~KTMacroCommand ();


        //
        // KTCommand Interface
        //

        virtual int size () const;

        virtual void execute ();
        virtual void unexecute ();

        //
        // Interface
        //

        void addCommand (KTCommand *command);

    protected:
        QValueList <KTCommand *> m_commandList;

    private:
        class KTMacroCommandPrivate *d;
};

// Clone of KCommandHistory with features required by KolourPaint:
// - nextUndoCommand()/nextRedoCommand()
// - undo/redo history limited by both number and size
//
// Features not required by KolourPaint (e.g. commandExecuted()) are not
// implemented and undo limit == redo limit.  So compared to
// KCommandHistory, this is only "almost source compatible".

class KTCommandHistoryBase : public QObject
{
    Q_OBJECT

    public:
        KTCommandHistoryBase ();
        virtual ~KTCommandHistoryBase ();

    public:
        // (provided for compatibility with KCommandHistory)
        int undoLimit () const;
        void setUndoLimit (int limit);


        int undoMinLimit () const;
        void setUndoMinLimit (int limit);

        int undoMaxLimit () const;
        void setUndoMaxLimit (int limit);

        int undoMaxLimitSizeLimit () const;
        void setUndoMaxLimitSizeLimit (int sizeLimit);

    public:
        void addCommand (KTCommand *command, bool execute = true);
        void clear ();

    protected slots:
	// (same as undo() & redo() except they don't call
	//  trimCommandListsUpdateActions())
        void undoInternal ();
        void redoInternal ();

    public slots:
        virtual void undo ();
        virtual void redo ();

        virtual void undoUpToNumber (int which);
        virtual void redoUpToNumber (int which);

    protected:
        QString undoActionText () const;
        QString redoActionText () const;

        void trimCommandListsUpdateActions ();
        void trimCommandList (QValueList <KTCommand *> *commandList);
        void trimCommandLists ();
        void updateActions ();

    public:
        KTCommand *nextUndoCommand () const;
        KTCommand *nextRedoCommand () const;

        void setNextUndoCommand (KTCommand *command);

    public slots:
        virtual void documentSaved ();

    signals:
        void documentRestored ();

    protected:

    	// (Front element is the next one)
        QValueList <KTCommand *> m_undoCommandList;
        QValueList <KTCommand *> m_redoCommandList;

        int m_undoMinLimit, m_undoMaxLimit, m_undoMaxLimitSizeLimit;

	// What you have to do to get back to the document's unmodified state:
	// * -x: must Undo x times
	// * 0: unmodified
	// * +x: must Redo x times
	// * INT_MAX: can never become unmodified again
	// ASSUMPTION: will never have INT_MAX commands in any list.
        int m_documentRestoredPosition;

    private:
        class KTCommandHistoryBasePrivate *d;
};

// Intercepts Undo/Redo requests:
//
// If the user is currently drawing a shape, it cancels it.
// Else it passes on the Undo/Redo request to KTCommandHistoryBase.
//
// TODO: This is wrong.  It won't work if the Undo action is disabled,
//       for instance.
//
//       Maybe the real solution is to call KTCommandHistoryBase::addCommand()
//       as _soon_ as the shape starts - not after it ends.  But the
//       trouble with this solution is that if the user Undoes/cancels
//       the shape s/he's currently drawing, it would replace a Redo
//       slot in the history.  Arguably you shouldn't be able to Redo
//       something you never finished drawing.
//
//       The solution is to add this functionality to KTCommandHistoryBase.

class KTCommandHistory : public KTCommandHistoryBase
{
	Q_OBJECT

	public:
                KTCommandHistory (Ktoon *mainWindow);
		virtual ~KTCommandHistory ();

	public slots:
		virtual void undo ();
		virtual void redo ();

	protected:
		Ktoon *m_mainWindow;
};


#endif
