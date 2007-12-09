/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktapplication.h"

#include <kgui/kcommandhistory.h>
#include <kgui/ktoolview.h>
#include <ksound/kaudioplayer.h>
#include <kcore/kdebug.h>

#include <QKeySequence>
#include <QTextBrowser>
#include <QToolBar>

void KTMainWindow::createGUI()
{
// 	setDockOptions(QMainWindow::ForceTabbedDocks /*| QMainWindow::AnimatedDocks*/);
	
	m_colorPalette = new KTColorPalette;
	KToolView *view = addToolView( m_colorPalette, Qt::LeftDockWidgetArea, Drawing );
	
	m_actionManager->insert( view->toggleViewAction(), "show palette");
	addToPerspective(view->toggleViewAction(), Drawing);
	
	connectToDisplays(m_colorPalette);
	ui4paintArea( m_colorPalette );
	
	////////////////////
	
	m_penWidget = new KTPenWidget;
	view = addToolView( m_penWidget, Qt::LeftDockWidgetArea, Drawing );
	
	m_actionManager->insert( view->toggleViewAction(), "show pen");
	addToPerspective(view->toggleViewAction(), Drawing);
	
	connectToDisplays(m_penWidget);
	ui4paintArea( m_penWidget );
	
	////////////////////
	m_libraryWidget = new KTLibraryWidget();
	m_libraryWidget->setLibrary(m_projectManager->project()->library());
	
	view = addToolView( m_libraryWidget, Qt::LeftDockWidgetArea, Drawing );
	
	m_actionManager->insert( view->toggleViewAction(), "show library");
	addToPerspective(view->toggleViewAction(), Drawing);
	
	connectToDisplays(m_libraryWidget);
	
	new KAction( QPixmap(), tr( "Import bitmap..." ), QKeySequence(), m_libraryWidget, SLOT(importBitmap()), m_actionManager, "importbitmap");
	new KAction( QPixmap(), tr( "Import audio file..." ), QKeySequence(), m_libraryWidget, SLOT(importSound()), m_actionManager, "importaudiofile");
	
	ui4project( m_libraryWidget );
	ui4localRequest(m_libraryWidget);
	
	/////////////////
	m_scenes = new KTScenesWidget;
	
	view = addToolView( m_scenes, Qt::RightDockWidgetArea, All );
	
	m_actionManager->insert( view->toggleViewAction(), "show scenes");
	
	ui4project(m_scenes);
	ui4localRequest(m_scenes);
	connectToDisplays(m_scenes);
	
	/////////////////////
	m_exposureSheet = new KTExposureSheet;
	
	view = addToolView( m_exposureSheet, Qt::RightDockWidgetArea, Drawing );
	
	m_actionManager->insert( view->toggleViewAction(), "show exposure");
	addToPerspective(view->toggleViewAction(), Drawing);
	
	ui4project( m_exposureSheet );
	ui4localRequest( m_exposureSheet );
	connectToDisplays(m_exposureSheet);
	
	///////////////////////
	m_helper = new KTHelpWidget(HOME_DIR+"/data/help/");
	view = addToolView( m_helper, Qt::RightDockWidgetArea, All );
	m_actionManager->insert( view->toggleViewAction(), "show help");
	
	connect(m_helper, SIGNAL(pageLoaded(const QString &, const QString &)), this, SLOT(showHelpPage(const QString &, const QString &)));
	
	connectToDisplays(m_helper);
	
	///////////////////////
	
	//////////////////////
	m_timeLine = new KTTimeLine;
	m_timeLine->setLibrary(m_projectManager->project()->library());
	
	view = addToolView( m_timeLine, Qt::BottomDockWidgetArea, Drawing );
	
	m_actionManager->insert( view->toggleViewAction(), "show timeline");
	addToPerspective(view->toggleViewAction(), Drawing);
	
	ui4project( m_timeLine );
	ui4localRequest( m_timeLine );
	connectToDisplays(m_timeLine);
	
	
	//////////////////
#ifdef ENABLE_KINAS
	KinasWidget *m_scriptEditor = new KinasWidget;
	addToolView( m_scriptEditor, Qt::BottomDockWidgetArea, Drawing );
#endif
		
	//////////////

        KTExportWidget *m_exportWidget = new KTExportWidget(m_projectManager->project());
        view = addToolView(m_exportWidget, Qt::BottomDockWidgetArea, Drawing);
        connectToDisplays(m_exportWidget);

	/////////////////////
	
	m_cameraWidget = new KTCameraWidget(m_projectManager->project());
	
	view = addToolView( m_cameraWidget, Qt::BottomDockWidgetArea, Animation );
	
	m_actionManager->insert( view->toggleViewAction(), "show camera");
	addToPerspective(view->toggleViewAction(), Drawing);
	
	ui4project( m_cameraWidget );
	connectToDisplays( m_cameraWidget );
	
	/////////////////////
}

void KTMainWindow::connectToDisplays(const QWidget *widget)
{
	connect(widget, SIGNAL(sendToStatus(const QString &)), this, SLOT(messageToStatus(const QString &)));
}

void KTMainWindow::setupMenu()
{
	// Setup the file menu
	setupFileActions();
	
	m_fileMenu = new QMenu(tr("&File"), this);
	menuBar()->addMenu(m_fileMenu);
	
	QMenu *newMenu = new QMenu(tr("&New..."), this );
	m_fileMenu->addMenu(newMenu);
	
	newMenu->addAction(m_actionManager->find("newproject"));
	
	newMenu->addSeparator();
	m_fileMenu->addAction(m_actionManager->find("openproject"));
	m_fileMenu->addAction(m_actionManager->find("opennetproject"));
	m_fileMenu->addAction(m_actionManager->find("importprojectserver"));
	m_recentProjectsMenu = new QMenu(tr("Recents"), this );
	connect( m_recentProjectsMenu, SIGNAL( activated(int) ), this, SLOT( openRecent( int ) ) );
	
	KCONFIG->beginGroup("General");
	QStringList recents = KCONFIG->value("recents").toString().split(';');
	
	updateOpenRecentMenu(m_recentProjectsMenu, recents);
	
	m_fileMenu->addMenu( m_recentProjectsMenu );

	m_fileMenu->addAction(m_actionManager->find("saveproject"));
	m_fileMenu->addAction(m_actionManager->find("saveprojectas"));
	m_fileMenu->addAction(m_actionManager->find("closeproject"));
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_actionManager->find("export"));
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_actionManager->find("ImportPalettes"));
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_actionManager->find("Exit"));
	m_fileMenu->addSeparator();
	
	// Setup the insert menu
	setupInsertActions();
	
	m_insertMenu = new QMenu(tr( "&Insert" ), this);
	menuBar()->addMenu( m_insertMenu );
	m_insertMenu->addAction(m_actionManager->find("InsertScene"));
	m_insertMenu->addAction(m_actionManager->find("InsertLayer"));
	m_insertMenu->addAction(m_actionManager->find("InsertFrame"));
	m_insertMenu->addSeparator();
	
	m_insertMenu->addAction(m_actionManager->find("importbitmap"));
	m_insertMenu->addAction(m_actionManager->find("importaudiofile"));
	
	// Setup the window menu
	setupWindowActions();
	m_windowMenu = new QMenu(tr( "&Window" ),this);
	menuBar()->addMenu(  m_windowMenu );
	m_windowMenu->addAction(m_actionManager->find("show debug"));
	m_windowMenu->addAction(m_actionManager->find("show palette"));
	m_windowMenu->addAction(m_actionManager->find("show pen"));
	m_windowMenu->addAction(m_actionManager->find("show library"));
	m_windowMenu->addAction(m_actionManager->find("show timeline"));
	m_windowMenu->addAction(m_actionManager->find("show scenes"));
	m_windowMenu->addAction(m_actionManager->find("show exposure"));
	m_windowMenu->addAction(m_actionManager->find("show help"));
	m_windowMenu->addSeparator();
	
	
	// Setup perspective menu
	
	QMenu *perspectiveMenu = new QMenu(tr("Perspective"),this);
	
	QActionGroup *group = new QActionGroup(this);
	group->setExclusive(true);

	QAction *drawingPerspective = new QAction(tr("Drawing"), this);
	drawingPerspective->setShortcut(QKeySequence(Qt::Key_F9));
	
	drawingPerspective->setData(Drawing);
	group->addAction(drawingPerspective);

	QAction *animationPerspective = new QAction(tr("Animation"), this);
	animationPerspective->setShortcut(QKeySequence(Qt::Key_F10));
// 	drawingPerspective->setCheckable(true);
	group->addAction(animationPerspective);
	animationPerspective->setData(Animation);
	
	perspectiveMenu->addActions(group->actions());
	
	connect(group, SIGNAL(triggered(QAction *)), this, SLOT(changePerspective(QAction *)));
	
	menuBar()->addMenu(perspectiveMenu);
	
	// Setup the Settings menu
	setupSettingsActions();
	m_settingsMenu = new QMenu(tr( "&Settings" ), this);
	menuBar()->addMenu( m_settingsMenu );

	m_settingsMenu->addAction(m_actionManager->find("wizard"));
	m_settingsMenu->addAction(m_actionManager->find("preferences"));
	
	// Setup the help menu
	setupHelpActions();
	m_helpMenu = new QMenu(tr( "&Help" ),this);
	menuBar()->addMenu(  m_helpMenu );
	m_helpMenu->addAction(m_actionManager->find("tipofday") );
	m_helpMenu->addSeparator();
	m_helpMenu->addAction(m_actionManager->find("about ktoon") );
}


void KTMainWindow::setupActions()
{
// 	KAction *next = new KAction( QPixmap(), tr( "Back Frame" ), QKeySequence(Qt::Key_PageUp), this, SLOT(selectBackFrame()), m_actionManager, "BackFrame");
// 	next->setShortcutContext ( Qt::ApplicationShortcut );
// 	
// 	
// 	KAction *back = new KAction( QPixmap(), tr( "Next Frame" ), QKeySequence(Qt::Key_PageDown), this, SLOT(selectNextFrame()), m_actionManager, "Next Frame");
// 	back->setShortcutContext ( Qt::ApplicationShortcut );
// 	
// 	addAction(back);
// 	addAction(next);
}


void KTMainWindow::setupFileActions()
{
	KAction *newProject = new KAction( QPixmap( THEME_DIR+"/icons/project.png" ), tr( "New project" ), QKeySequence(), this, SLOT(newProject()), m_actionManager );
	newProject->setStatusTip(tr( "Opens a new project"));
	m_actionManager->insert( newProject, "newproject", "file" );
	
	
	KAction *openFile = new KAction( QPixmap(THEME_DIR+"/icons/open.png"), tr( "Open project" ), tr("Ctrl+O"), this, SLOT(openProject()), m_actionManager );
	m_actionManager->insert( openFile, "openproject", "file" );
	openFile->setStatusTip(tr("Loads an existent project"));
	
	KAction *openNetFile = new KAction(QPixmap(THEME_DIR+"/icons/open.png"), tr("Open project from server..."), tr(""), this, SLOT(openProjectFromServer()), m_actionManager);
	m_actionManager->insert(openNetFile, "opennetproject", "file");
	
	KAction *importNetFile = new KAction(QPixmap(THEME_DIR+""), tr("Import project to server..."), tr(""), this, SLOT(importProjectToServer()), m_actionManager);
	m_actionManager->insert(importNetFile, "importprojectserver", "file");
	
	KAction *save = new KAction( QPixmap(THEME_DIR+"/icons/save.png"), tr( "Save project" ),QKeySequence(tr("Ctrl+S")), this, SLOT(saveProject()), m_actionManager);
	m_actionManager->insert( save, "saveproject", "file" );
	save->setStatusTip(tr("Saves the current project in the current location"));
	
	KAction *saveAs = new KAction( tr( "Save project &As..." ), m_actionManager);
	connect(saveAs, SIGNAL(triggered()), this, SLOT(saveAs()));
	saveAs->setStatusTip(tr("Opens a dialog box to save the current project in any location"));
	m_actionManager->insert( saveAs, "saveprojectas", "file" );
	
	KAction *close = new KAction(QPixmap(THEME_DIR+"/icons/close.png"), tr( "Cl&ose project" ), QKeySequence(tr("Ctrl+W")), m_actionManager);
	connect(close, SIGNAL(triggered()), this, SLOT(closeProject()));
	close->setStatusTip(tr("Closes the active project"));
	m_actionManager->insert( close, "closeproject", "file" );
	
	//------
	
	KAction *importPalette = new KAction( QPixmap(THEME_DIR+"/icons/import.png"), tr( "&Import GIMP palettes..." ),  QKeySequence(), this, SLOT(importPalettes()), m_actionManager);
	importPalette->setStatusTip(tr("Imports palettes"));
	m_actionManager->insert( importPalette, "importpalettes", "file" );
	
	// -----
	KAction *exportProject = new KAction( QPixmap(THEME_DIR+"/icons/export.png"), tr( "&Export..." ),  QKeySequence(), this, SLOT(exportProject()), m_actionManager);
	exportProject->setStatusTip(tr("Exports project to different formats"));
	m_actionManager->insert( exportProject, "export", "file" );
	
	KAction *exit = new KAction(QPixmap(THEME_DIR+"/icons/export.png"), tr( "E&xit" ),  QKeySequence(tr("Ctrl+Q")), qApp, SLOT(closeAllWindows ()), m_actionManager);
	exit->setStatusTip(tr("Closes the application"));
	m_actionManager->insert( exit, "exit", "file" );
}

void KTMainWindow::setupSettingsActions()
{
	KAction *wizard = new KAction( tr( "Launch configuration wizard..." ), QKeySequence(), qobject_cast<KTApplication*>(qApp), SLOT(firstRun()), m_actionManager, "wizard");
	wizard->setStatusTip(tr("Launch first configuration wizard"));
	
	KAction * preferences = new KAction( tr( "Pr&eferences..." ), QKeySequence(), this, SLOT( preferences()), m_actionManager, "preferences");
	preferences->setStatusTip(tr("Opens the preferences dialog box"));
}

void KTMainWindow::setupHelpActions()
{
	new KAction(QPixmap(), tr("About KToon"), QKeySequence(), this, SLOT(aboutKToon()), m_actionManager, "about ktoon");
	new KAction(QPixmap(), tr("Tip of day"), QKeySequence(), this, SLOT(showTipDialog()), m_actionManager, "tipofday");
}

void KTMainWindow::setupWindowActions()
{
#if defined(QT_GUI_LIB) && !defined(K_NODEBUG)
	new KAction(QPixmap(), tr("Show debug dialog"), QKeySequence(), KDebug::browser(), SLOT(show()), m_actionManager, "show debug");
#endif
}

void KTMainWindow::setupInsertActions()
{
	new KAction( QPixmap(THEME_DIR+"/icons/scene.png"), tr( "Insert scene" ), QKeySequence(), m_scenes, SLOT(emitRequestInsertScene()), m_actionManager, "InsertScene");
	
// 	new KAction( QPixmap(THEME_DIR+"/icons/layer.png"), tr( "Insert layer" ), QKeySequence(), m_exposureSheet, SLOT(createLayer()), m_actionManager, "InsertLayer");
// 	
// 	new KAction( QPixmap(THEME_DIR+"/icons/frame.png"), tr( "Insert frame" ), QKeySequence(), m_projectManager, SLOT(createFrame()), m_actionManager, "InsertFrame");
}

void KTMainWindow::setupToolBar()
{
	QToolBar * toolbar = new QToolBar(tr("Bar Actions"), this);
	toolbar->setIconSize( QSize(22,22) );
	addToolBar(Qt::TopToolBarArea, toolbar);
	
// 	DCommandHistory *history = new DCommandHistory(m_undoCommands, this);
	
	
	QAction * undo = m_projectManager->undoHistory()->createUndoAction( this, tr("Undo"));
	
	undo->setShortcut(QKeySequence(QKeySequence::Undo));
	
	toolbar->addAction(undo);
	
	
	QAction *redo =  m_projectManager->undoHistory()->createRedoAction ( this );
	redo->setShortcut(QKeySequence(QKeySequence::Redo));
	toolbar->addAction(redo);
	
	undo->setIcon( QPixmap(THEME_DIR+"/icons/undo.png" ));
	redo->setIcon(QPixmap(THEME_DIR+"/icons/redo.png" ));
	
	kApp->insertGlobalAction(undo, "undo");
	kApp->insertGlobalAction(redo, "redo");
	
}

void KTMainWindow::updateOpenRecentMenu(QMenu *menu, QStringList recents)
{	
	menu->clear();
	m_recentProjects.clear();
	
	foreach(QString recent, recents)
	{
		if ( !recent.isEmpty() && m_recentProjects.indexOf(recent) == -1 )
		{
			m_recentProjects << recent;
			connect(menu->addAction(recent), SIGNAL(triggered()), this, SLOT(openRecentProject()));
		}
	}
}

void KTMainWindow::showWidgetPage()
{
// 	KAction *action = qobject_cast<KAction *>(sender());
// 	
// 	if ( action )
// 	{
// 		QWidget *widget = 0;
// 		DiDockWidget::Position position;
// 		QString actionText = "";
// 		
// 		if ( action == m_actionManager->find("show timeline") )
// 		{
// 			widget = m_timeLine;
// 			position = DiDockWidget::Bottom;
// 			actionText = "time line widget";
// 		}
// 		else if ( action == m_actionManager->find("show exposure") )
// 		{
// 			widget = m_exposureSheet;
// 			position = DiDockWidget::Right;
// 			actionText = "exposure widget";
// 		}
// 		else if ( action == m_actionManager->find("show library") )
// 		{
// 			widget = m_libraryWidget;
// 			position = DiDockWidget::Left;
// 			actionText = "library widget";
// 		}
// 		else if ( action == m_actionManager->find("show scenes") )
// 		{
// 			widget = m_scenes;
// 			position = DiDockWidget::Right;
// 			actionText = "scenes widget";
// 		}
// 		else if ( action == m_actionManager->find("show help") )
// 		{
// 			widget = m_helper;
// 			position = DiDockWidget::Right;
// 			actionText = "help widget";
// 		}
// 		
// 		else if ( action == m_actionManager->find("show palette") )
// 		{
// 			widget = m_colorPalette;
// 			position = DiDockWidget::Left;
// 			actionText = "color palette widget";
// 		}
// 		if ( widget )
// 		{
// 			if ( widget->isVisible() )
// 			{
// 				toolWindow( position)->centralWidget()->setExpanded(false);
// 				action->setText("Show "+actionText);
// 			}
// 			else
// 			{
// 				toolWindow( position)->centralWidget()->raiseWidget(widget);
// 				action->setText("Hide "+actionText);
// 			}
// 		}
// 	}
}

void KTMainWindow::changePerspective(QAction *a)
{
	int perspective = a->data().toInt();
	setCurrentPerspective(perspective);
	a->setChecked(true);
}


