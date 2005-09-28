//
// C++ Interface: ktoonscript
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef KTOONSCRIPT_H
#define KTOONSCRIPT_H

#include <ktmodulewidgetbase.h>
#include <QSplitter>
#include <QTabWidget>

#include "ktseditor.h"
#include "ktsfunctionview.h"

/**
	@author David Cuadrado <krawek@toonka.com>
*/
class KToonScript : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KToonScript(QWidget *parent = 0);
		~KToonScript();
		
	public slots:
		void addEditor(const QString &title);
		
	private:
		QSplitter *m_splitter;
		KTSFunctionView *m_functionViewer;
		QTabWidget *m_editors;

};

#endif
