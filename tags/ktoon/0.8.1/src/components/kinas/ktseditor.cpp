//
// C++ Implementation: ktseditor
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "ktseditor.h"
#include <QDragEnterEvent>

KTSEditor::KTSEditor(QWidget *parent) : QTextEdit(parent)
{
	setAcceptDrops(true);
}


KTSEditor::~KTSEditor()
{
}

void KTSEditor::dragEnterEvent(QDragEnterEvent *e)
{
	qDebug("DRAG&");
	if ( e->mimeData()->hasFormat("text/plain") )
	{
		e->acceptProposedAction();
	}
	else
	{
	}
}

void KTSEditor::dropEvent(QDropEvent *e)
{
	qDebug("DROP");
	append(e->mimeData()->text());
	
	e->acceptProposedAction();
}

void KTSEditor::insertFromMimeData ( const QMimeData * source )
{
	qDebug("INSERT");
}

