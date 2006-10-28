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

#include "kinaswidget.h"
#include "ddebug.h"

KinasWidget::KinasWidget(QWidget *parent) : KTModuleWidgetBase(parent)
{
	DINIT;
	
	setWindowTitle(tr("KToon Script"));
	setWindowIcon(QPixmap(HOME_DIR+"/images/icons/color_palette.png") );
	
	m_splitter = new QSplitter(this);
	addChild(m_splitter);
	
	m_functionViewer = new KTSFunctionView(m_splitter);
	m_splitter->addWidget(m_functionViewer);
	 
	m_editors = new QTabWidget(this);
	m_splitter->addWidget(m_editors);
	
	m_splitter->setSizes(QList<int>()<<160 << 400);
	
	addEditor("Example.kts");
}


KinasWidget::~KinasWidget()
{
	DEND;
}

void KinasWidget::addEditor(const QString &title)
{
	KTSEditor *m_editor = new KTSEditor;
	m_editors->addTab(m_editor, title);
}



