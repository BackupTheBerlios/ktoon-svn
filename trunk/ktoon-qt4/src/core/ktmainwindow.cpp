/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "ktmainwindow.h"

#include "ktnewproject.h"

#include "ktdebug.h"
#include "kimageeffect.h"

#include "ktapplication.h"
#include "ktabout.h"

// dlslib
#include "dtabwidget.h"
#include "docksplitter.h"

// Qt
#include <QImage>
#include <QPixmap>
#include <QResizeEvent>
#include <QMenu>
#include <QCloseEvent>
#include <QTextEdit>
//

KTMainWindow::KTMainWindow() : DMainWindow(), m_exposureSheet(0), m_scenes(0)
{
	KTINIT;
	
	setObjectName("KTMainWindow_");
	
	m_osd = new KTOsd( centralWidget() );
	
	m_statusBar = new KTStatusBar(this);
	setStatusBar( m_statusBar );
	
	setWindowTitle(tr("KToon: 2D animation toolkit"));
	
	m_projectManager = new KTProjectManager(this);
	
	m_drawingSpace = new QWorkspace;
	m_drawingSpace->setScrollBarsEnabled ( true );
	
// 	m_drawingSpace->setBackground(QBrush(QPixmap(background_xpm))); 
	
	addWidget(m_drawingSpace, tr("Illustration"));
	
	m_animationSpace = new QWorkspace;
	m_animationSpace->setScrollBarsEnabled ( true );
	
	m_viewCamera = new KTViewCamera(m_animationSpace);
	connect(m_viewCamera, SIGNAL(sendMessage(const QString &, int)), m_statusBar, SLOT(setStatus(const QString &, int)));
	connect(m_viewCamera, SIGNAL(sendProgress(int, int)), m_statusBar, SLOT(advance(int, int))); 
	
	
	m_animationSpace->addWindow(m_viewCamera);
	m_viewCamera->show();
	addWidget(m_animationSpace, tr("Animation"));
	
	
	setupBackground();
	
	m_actionManager = new KTActionManager(this);
	
	// Create the menubar;
	setupFileActions();
	setupEditActions();
	setupProjectActions();
	setupHelpActions();
	
	setupMenu();
	createGUI();
	
	m_pActiveTabWidget->setCurrentIndex( 0 );
	
	showMaximized();
	
	createNewProject("test");
}


KTMainWindow::~KTMainWindow()
{
	KTEND;
}


// Modal

void KTMainWindow::createNewProject(const QString &name, const QSize &size)
{
	KTDocument *document = m_projectManager->createDocument(name);
	m_projectManager->setCurrentDocument(0);
	
	newViewDocument(name);
	
	m_viewCamera->animationArea()->setScene(m_projectManager->currentScene());
}

void KTMainWindow::newViewDocument(const QString &name)
{
	m_statusBar->setStatus(tr("Opening a new document..."));
	
	KTScene *scene = m_projectManager->currentScene();
	
	if ( scene )
	{
		KTViewDocument *viewDocument = new KTViewDocument(scene, m_drawingSpace);
		m_drawingSpace->addWindow(viewDocument);
		viewDocument->setWindowTitle(name);
		
		m_statusBar->advance(4);
		viewDocument->setActiveWindow();
		m_statusBar->advance(7);
		viewDocument->show();
		m_statusBar->advance(10);
		m_statusBar->setStatus(tr("Opened."));
	}
	else
	{
		m_statusBar->advance(0);
		m_statusBar->setStatus(tr("Project not open."));
	}
}

/**
 * @todo close current project
 */
void KTMainWindow::newProject()
{
	KTNewProject *wizard = new KTNewProject;
	if ( wizard->exec() != QDialog::Rejected )
	{
		createNewProject( wizard->projectName() );
	}
	delete wizard;
}

void KTMainWindow::closeProject()
{
	
}

void KTMainWindow::openProject()
{
}

void KTMainWindow::save()
{
	ktDebug() << "Saving.." << endl;
	qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ())->drawArea()->paintDevice().save("test.png", "PNG"); // only for test
}

void KTMainWindow::preferences()
{
	m_statusBar->setStatus( tr( "Preferences Dialog Opened" ), 2000 );
	Preferences *preferences = new Preferences( this );
	preferences->exec();
}

void KTMainWindow::aboutKToon()
{
	KTAbout *about = new KTAbout(this);
	about->exec();
	
	delete about;
}



// Drawing
void KTMainWindow::changeCurrentColors(const QBrush &foreground, const QBrush &background)
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		doc->drawArea()->currentBrush()->setPenBrush(foreground);
		doc->drawArea()->currentBrush()->setBrush(background);
	}
}

void KTMainWindow::changeCurrentBrush(const QPainterPath &form, int thickness)
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		KTBrush *current = doc->drawArea()->currentBrush();
		
		current->setBrushForm( form );
		current->setPenWidth( thickness );
	}
}

void KTMainWindow::changeFPS(int fps)
{
	m_projectManager->currentScene()->setFPS( fps );
}

void KTMainWindow::ui4project(QWidget *widget)
{
	connect(widget, SIGNAL(requestInsertFrame(bool)), m_projectManager, SLOT(createFrame(bool)));
	
	connect(widget, SIGNAL(requestInsertLayer()), m_projectManager, SLOT(createLayer()));
	
	connect(widget, SIGNAL(frameSelected( int, int )), this, SLOT(selectFrame(int,int)));
	
	connect(widget, SIGNAL(layerVisibilityChanged( int, bool)), m_projectManager, SLOT(setLayerVisibility( int, bool)));
	
	connect(widget, SIGNAL(layerSelected( int)), m_projectManager, SLOT(setCurrentLayer( int)));
	
	connect(widget, SIGNAL(requestCopyFrame(int)), m_projectManager, SLOT(copyFrame(int)));
	
	connect(widget, SIGNAL(requestPasteFrame(int)), m_projectManager, SLOT(pasteFrame(int)));
	
	connect(widget, SIGNAL(requestMoveFrame(bool)), m_projectManager, SLOT(moveFrame(bool)));
	
	connect(widget, SIGNAL(requestRemoveFrame()), m_projectManager, SLOT(removeFrame()));
	
	connect(widget, SIGNAL(requestLockFrame()), m_projectManager, SLOT(lockCurrentFrame()));
	
	connect(widget, SIGNAL(requestRemoveLayer()), m_projectManager, SLOT(removeLayer()));
}
