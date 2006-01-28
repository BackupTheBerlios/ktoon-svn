//
// C++ Interface: ktseditor
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KTSEDITOR_H
#define KTSEDITOR_H

#include <QTextEdit>
#include <QMimeData>

/**
	@author David Cuadrado <krawek@toonka.com>
*/
class KTSEditor : public QTextEdit
{
	Q_OBJECT
	public:
		KTSEditor(QWidget *parent = 0);
		~KTSEditor();
		void dragEnterEvent(QDragEnterEvent *e);
		void dropEvent(QDropEvent *e);
		void insertFromMimeData ( const QMimeData * source );

};

#endif
