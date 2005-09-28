//
// C++ Implementation: ktoonscript
//
// Description: 
//
//
// Author: David Cuadrado <krawek@toonka.com>, (C) 2005
//
// Copyright: See COPYING file that comes with this distribution
//
//

#include "ktoonscript.h"
#include "ktdebug.h"

KToonScript::KToonScript(QWidget *parent) : KTModuleWidgetBase(parent)
{
	KTINIT;
	
	setCaption(tr("KToon Script"));
	
	m_splitter = new QSplitter(this);
	addChild(m_splitter);
	
	m_functionViewer = new KTSFunctionView(m_splitter);
	m_splitter->addWidget(m_functionViewer);
	 
	m_editors = new QTabWidget(this);
	m_splitter->addWidget(m_editors);
	
	m_splitter->setSizes(QList<int>()<<160 << 400);
	
	addEditor("Example.kts");
}


KToonScript::~KToonScript()
{
	KTEND;
}

void KToonScript::addEditor(const QString &title)
{
	KTSEditor *m_editor = new KTSEditor;
	m_editors->addTab(m_editor, title);
}



