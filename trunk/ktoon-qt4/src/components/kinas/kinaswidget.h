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
#ifndef KINASWIDGET_H
#define KINASWIDGET_H

#include <ktmodulewidgetbase.h>
#include <QSplitter>
#include <QTabWidget>

#include "ktseditor.h"
#include "ktsfunctionview.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KinasWidget : public KTModuleWidgetBase
{
	Q_OBJECT
	public:
		KinasWidget(QWidget *parent = 0);
		~KinasWidget();
		
	public slots:
		void addEditor(const QString &title);
		
	private:
		QSplitter *m_splitter;
		KTSFunctionView *m_functionViewer;
		QTabWidget *m_editors;

};

#endif
