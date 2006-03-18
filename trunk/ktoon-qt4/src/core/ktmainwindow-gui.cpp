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

#include "ddebug.h"
#include "ktapplication.h"

void KTMainWindow::createGUI()
{
	// TODO: put setWindowIcon in each class
	m_colorPalette = new KTColorPalette(this);
	m_colorPalette->setWindowIcon(QPixmap(THEME_DIR+"/icons/color_palette.png") );
	connect(m_colorPalette, SIGNAL(brushChanged(const QBrush &, const QBrush &)), this, SLOT(changeCurrentColors(const QBrush &, const QBrush &)));
	toolWindow(DDockWindow::Left)->addWidget(tr("Palette"),m_colorPalette);
	
	connectToDisplays(m_colorPalette);
	
	////////////////////
	
	m_penWidget = new KTPenWidget(this);
	toolWindow(DDockWindow::Left)->addWidget(tr("Pen"),m_penWidget);
	
	connect(m_penWidget, SIGNAL(penChanged( const QPen& )), this, SLOT(changeCurrentPen( const QPen &)));
	
	connectToDisplays(m_penWidget);
	
	
	
	////////////////////
	m_libraryWidget = new KTLibraryWidget( this );
	m_libraryWidget->setWindowIcon(QPixmap(THEME_DIR+"/icons/library.png"));
	toolWindow(DDockWindow::Left)->addWidget(tr("Library"),m_libraryWidget);
	
	connect(m_libraryWidget, SIGNAL(requestCurrentGraphic()), this, SLOT(addCurrentGraphicToLibrary()));
	connect(m_libraryWidget, SIGNAL(sendCurrentGraphic(const AGraphicComponent *)), this, SLOT(addGraphicComponent(const AGraphicComponent *)));
	
	connectToDisplays(m_libraryWidget);
	
	/////////////////
	m_scenes = new KTScenesWidget( this);
	m_scenes->setWindowIcon(QPixmap(THEME_DIR+"/icons/scenes.png"));
	toolWindow(DDockWindow::Right)->addWidget(tr("Scenes"),m_scenes);
	
	connect(m_scenes, SIGNAL(requestInsertScene()), m_projectManager, SLOT(createScene()));
	connect(m_scenes, SIGNAL(requestRemoveScene()), m_projectManager, SLOT(removeScene()));
	connect(m_scenes, SIGNAL(changeCurrentScene( int )), this, SLOT(changeScene(int)));
	
	
	connectToDisplays(m_scenes);
	
	/////////////////////
	m_exposureSheet = new KTExposureSheet(this);
	m_exposureSheet->setWindowIcon(QPixmap(THEME_DIR+"/icons/exposure_sheet.png"));
	toolWindow(DDockWindow::Right)->addWidget(tr("Exposure Sheet"),m_exposureSheet);
	
	ui4project( m_exposureSheet );
	connect(m_exposureSheet, SIGNAL(requestChangeScene( int ) ), this, SLOT(changeScene(int)));

	connectToDisplays(m_exposureSheet);
	
	///////////////////////
	m_helper = new KTHelpWidget(HOME+"/data/help/");
	toolWindow(DDockWindow::Right)->addWidget(tr("Help"),m_helper);
	connect(m_helper, SIGNAL(pageLoaded(const QString &, const QString &)), this, SLOT(showHelpPage(const QString &, const QString &)));
	
	connectToDisplays(m_helper);
	
	///////////////////////
	
	
	
	//////////////////////
	m_timeLine = new KTTimeLine(this);
	m_timeLine->setWindowIcon(QPixmap(THEME_DIR+"/icons/time_line.png"));
	toolWindow(DDockWindow::Bottom)->addWidget(tr("Time Line"),m_timeLine);
	connect(m_timeLine, SIGNAL(requestChangeFPS(int)), this, SLOT(changeFPS( int )));
// 	connect(m_timeLine, SIGNAL(requestChangeScene( int ) ), this, SLOT(changeScene(int)));
	ui4project( m_timeLine );
	
	connectToDisplays(m_timeLine);
	
	
	//////////////////
#ifdef ENABLE_KINAS
	KinasWidget *m_scriptEditor = new KinasWidget(this);
	m_scriptEditor->setWindowIcon(QPixmap(HOME+"/images/icons/color_palette.png") );
	toolWindow(DDockWindow::Bottom)->addWidget(tr("Kinas"), m_scriptEditor);
#endif
	/////////////////
	
	m_gcEditor = new KTGCEditor(this);
	connect(m_gcEditor, SIGNAL(requestRotate(int)), this, SLOT(rotateCurrentElement(int)));
	connect(m_gcEditor, SIGNAL(requestScale(double,double)), this, SLOT(scaleCurrentElement(double,double)));
	connect(m_gcEditor, SIGNAL(requestShear(double,double)), this, SLOT(shearCurrentElement(double,double)));
	connect(m_gcEditor, SIGNAL(requestTranslate(double,double)), this, SLOT(translateCurrentElement(double,double)));
	
	toolWindow(DDockWindow::Bottom)->addWidget(tr("GC Editor"), m_gcEditor);
	
	connectToDisplays(m_gcEditor);
	
	//////////////
	
	KTExportWidget *m_exportWidget = new KTExportWidget(m_projectManager, this);
	
	toolWindow(DDockWindow::Bottom)->addWidget(tr("Export"), m_exportWidget);
	
	connectToDisplays(m_exportWidget);
	
	//////////////
	
	
	// Connect the project manager with the components...
	connect(m_projectManager, SIGNAL(sceneCreated(const QString &, bool)), this, SLOT( insertScene(const QString &, bool)));
	
	connect(m_projectManager, SIGNAL(frameCreated( const QString &, bool)), this, SLOT(insertFrame( const QString &, bool)));
	
	connect(m_projectManager, SIGNAL(layerCreated(const QString &, bool)), this, SLOT(insertLayer(const QString &, bool)));
	
	connect(m_projectManager, SIGNAL(layerVisibilityChanged(int, bool)), this, SLOT(setLayerVisibilityChanged(int, bool)));
	
	connect(m_projectManager, SIGNAL(frameMoved(bool)), this, SLOT(moveFrame(bool)));
	
	connect(m_projectManager, SIGNAL(frameRemoved()), this, SLOT(removeFrame()));
	connect(m_projectManager, SIGNAL(frameLocked()), this, SLOT(lockFrame()));
	
	connect(m_projectManager, SIGNAL(layerRemoved(int )), this, SLOT(removeLayer(int)));
	connect(m_projectManager, SIGNAL(layerSelected(int )), this, SLOT(selectLayer(int)));
	connect(m_projectManager, SIGNAL(layerMoved(bool)), this, SLOT(moveLayer(bool)));
	
	
	connect(m_projectManager, SIGNAL(layerRenamed(int, const QString & )), this, SLOT(setLayerName(int, const QString &)));
	
	connect(m_projectManager, SIGNAL(frameRenamed(int , int , const QString & )), this, SLOT(setFrameName( int, int, const QString& )));
	
}

void KTMainWindow::connectToDisplays(const QWidget *widget)
{
	connect(widget, SIGNAL(sendToStatus(const QString &)), this, SLOT(messageToStatus(const QString &)));
	connect(widget, SIGNAL(sendToOSD(const QString &, int)), this, SLOT(messageToOSD(const QString &, int)));
}

void KTMainWindow::setupMenu()
{
	// Setup the file menu
	setupFileActions();
	m_fileMenu = new QMenu(tr("&File"), this);
	menuBar()->addMenu(m_fileMenu);
	
	QMenu *newMenu = new QMenu(tr("&New...") );
	m_fileMenu->addMenu(newMenu);
	
	newMenu->addAction(m_actionManager->find("newproject"));
#if 0
	newMenu->addAction(m_actionManager->find("newdocument")); // TODO: Documents
#endif
	newMenu->addSeparator();
	newMenu->addAction(m_actionManager->find("newarea") );
	
	
	m_fileMenu->addAction(m_actionManager->find("openproject"));
	
	QMenu *recents = new QMenu(tr("Recents"), this );
	connect( recents, SIGNAL( activated( int ) ), SLOT( openRecent( int ) ) );
	
	updateOpenRecentMenu(recents);
	
	m_fileMenu->addMenu( recents );

	m_fileMenu->addAction(m_actionManager->find("saveproject"));
	m_fileMenu->addAction(m_actionManager->find("saveprojectas"));
	m_fileMenu->addAction(m_actionManager->find("closeproject"));

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
#if 0
	m_insertMenu->addAction(m_actionManager->find("insertimage")); // FIXME: brush is not handled
#endif
	
	// Setup the window menu
	setupWindowActions();
	m_windowMenu = new QMenu(tr( "&Window" ),this);
	menuBar()->addMenu(  m_windowMenu );
	m_windowMenu->addAction(m_actionManager->find("show palette"));
	m_windowMenu->addAction(m_actionManager->find("show brushes"));
	m_windowMenu->addAction(m_actionManager->find("show library"));
	m_windowMenu->addAction(m_actionManager->find("show timeline"));
	m_windowMenu->addAction(m_actionManager->find("show scenes"));
	m_windowMenu->addAction(m_actionManager->find("show exposure"));
	m_windowMenu->addAction(m_actionManager->find("show help"));
	m_windowMenu->addSeparator();
	
	
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

void KTMainWindow::setupFileActions()
{
	DAction *newProject = new DAction( QPixmap( THEME_DIR+"/icons/project.png" ), tr( "New project" ), QKeySequence(), this, SLOT(newProject()), m_actionManager, "newproject");
	newProject->setStatusTip(tr( "Opens a new project"));
	
	DAction *newArea = new DAction( QPixmap( THEME_DIR+"/icons/new.png" ), tr( "New paint area" ), QKeySequence(tr("Ctrl+N")), this, SLOT(newViewDocument()), m_actionManager, "newarea");
	newArea->setStatusTip(tr( "Opens a new paint area"));
	
#if 0
	// TODO: to implement
	DAction *newDocument = new DAction( QPixmap( THEME_DIR+"/icons/new.png" ), tr( "New document" ), QKeySequence(tr("Ctrl+N")), this, SLOT(), m_actionManager, "newdocument");
	newDocument->setStatusTip(tr( "Opens a new document"));
#endif
	
	DAction *openFile = new DAction( QPixmap(THEME_DIR+"/icons/open.png"), tr( "Open project" ), tr("Ctrl+O"), this, SLOT(openProject()), m_actionManager, "openproject");
	openFile->setStatusTip(tr("Loads an existent project"));
	
	DAction *save = new DAction( QPixmap(THEME_DIR+"/icons/save.png"), tr( "Save project" ),QKeySequence(tr("Ctrl+S")), this, SLOT(saveProject()), m_actionManager, "saveproject");
	save->setStatusTip(tr("Saves the current project in the current location"));
	
	DAction *saveAs = new DAction( tr( "Save project &As..." ), m_actionManager, "saveprojectas");
	connect(saveAs, SIGNAL(triggered()), this, SLOT(saveProjectAs()));
	saveAs->setStatusTip(tr("Opens a dialog box to save the current project in any location"));
	
	DAction *close = new DAction(QPixmap(THEME_DIR+"/icons/close.png"), tr( "Cl&ose project" ), QKeySequence(tr("Ctrl+W")), m_actionManager, "closeproject");
	connect(close, SIGNAL(triggered()), this, SLOT(closeProject()));
	close->setStatusTip(tr("Closes the active project"));
	
	//------
	
	DAction *importPalette = new DAction( QPixmap(THEME_DIR+"/icons/import.png"), tr( "&Import GIMP palettes..." ),  QKeySequence(), this, SLOT(importPalettes()), m_actionManager, "ImportPalettes");
	importPalette->setStatusTip(tr("Imports palettes"));
	
	DAction *exit = new DAction(QPixmap(THEME_DIR+"/icons/export.png"), tr( "E&xit" ),  QKeySequence(tr("Ctrl+Q")), qApp, SLOT(closeAllWindows ()),m_actionManager, "Exit");
	exit->setStatusTip(tr("Closes the application"));
}

void KTMainWindow::setupSettingsActions()
{
	DAction *wizard = new DAction( tr( "Launch configuration wizard..." ), QKeySequence(), qobject_cast<KTApplication*>(qApp), SLOT(firstRun()), m_actionManager, "wizard");
	wizard->setStatusTip(tr("Launch first configuration wizard"));
	
	DAction * preferences = new DAction( tr( "Pr&eferences..." ), QKeySequence(), this, SLOT( preferences()), m_actionManager, "preferences");
	preferences->setStatusTip(tr("Opens the preferences dialog box"));
}

void KTMainWindow::setupHelpActions()
{
	new DAction(QPixmap(), tr("About KToon"), QKeySequence(), this, SLOT(aboutKToon()), m_actionManager, "about ktoon");
	new DAction(QPixmap(), tr("Tip of day"), QKeySequence(), this, SLOT(showTipDialog()), m_actionManager, "tipofday");
}

void KTMainWindow::setupWindowActions()
{
	new DAction(QPixmap(), tr("Show TimeLine widget"), QKeySequence("CTRL+K"), this, SLOT(showWidgetPage()), m_actionManager, "show timeline");
	
	new DAction(QPixmap(), tr("Show exposure sheet widget"), QKeySequence("CTRL+H"), this, SLOT(showWidgetPage()), m_actionManager, "show exposure");
	
	new DAction(QPixmap(), tr("Show scenes widget"), QKeySequence("CTRL+Y"), this, SLOT(showWidgetPage()), m_actionManager, "show scenes");
	
	new DAction(QPixmap(), tr("Show brushes widget"), QKeySequence("CTRL+B"), this, SLOT(showWidgetPage()), m_actionManager, "show brushes");
	
	new DAction(QPixmap(), tr("Show library widget"), QKeySequence("CTRL+L"), this, SLOT(showWidgetPage()), m_actionManager, "show library");
	
	new DAction(QPixmap(), tr("Show help widget"), QKeySequence(""), this, SLOT(showWidgetPage()), m_actionManager, "show help");
	
	new DAction(QPixmap(), tr("Show color palette widget"), QKeySequence(), this, SLOT(showWidgetPage()), m_actionManager, "show palette");
	
}

void KTMainWindow::setupInsertActions()
{
	new DAction( QPixmap(THEME_DIR+"/icons/scene.png"), tr( "Insert scene" ), QKeySequence(), m_projectManager, SLOT(createScene()), m_actionManager, "InsertScene");
	
	new DAction( QPixmap(THEME_DIR+"/icons/layer.png"), tr( "Insert layer" ), QKeySequence(), m_projectManager, SLOT(createLayer()), m_actionManager, "InsertLayer");
	
	new DAction( QPixmap(THEME_DIR+"/icons/frame.png"), tr( "Insert frame" ), QKeySequence(), m_projectManager, SLOT(createFrame()), m_actionManager, "InsertFrame");
	
	new DAction( QPixmap(), tr( "Insert image" ), QKeySequence(), this, SLOT(insertImage()), m_actionManager, "insertimage");
}

void KTMainWindow::setupToolBar()
{
}

void KTMainWindow::closeEvent( QCloseEvent *event )
{
	DMainWindow::closeEvent(event);
	
	delete m_pBottomDock;
	delete m_pLeftDock;
	delete m_pRightDock;
	
	DCONFIG->beginGroup("General");
	DCONFIG->setValue("recents", m_recentProjects);
	
}

void KTMainWindow::updateOpenRecentMenu(QMenu *menu)
{
	DCONFIG->beginGroup("General");
	QStringList recents = DCONFIG->value("recents").toString().split(';');
	
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
	DAction *action = qobject_cast<DAction *>(sender());
	
	if ( action )
	{
		QWidget *widget = 0;
		DDockWindow::Position position;
		QString actionText = "";
		
		if ( action == m_actionManager->find("show timeline") )
		{
			widget = m_timeLine;
			position = DDockWindow::Bottom;
			actionText = "time line widget";
		}
		else if ( action == m_actionManager->find("show exposure") )
		{
			widget = m_exposureSheet;
			position = DDockWindow::Right;
			actionText = "exposure widget";
		}
		else if ( action == m_actionManager->find("show library") )
		{
			widget = m_libraryWidget;
			position = DDockWindow::Left;
			actionText = "library widget";
		}
		else if ( action == m_actionManager->find("show scenes") )
		{
			widget = m_scenes;
			position = DDockWindow::Right;
			actionText = "scenes widget";
		}
		else if ( action == m_actionManager->find("show help") )
		{
			widget = m_helper;
			position = DDockWindow::Right;
			actionText = "help widget";
		}
		
		else if ( action == m_actionManager->find("show palette") )
		{
			widget = m_colorPalette;
			position = DDockWindow::Left;
			actionText = "color palette widget";
		}
		if ( widget )
		{
			if ( widget->isVisible() )
			{
				toolWindow( position)->centralWidget()->setExpanded(false);
				action->setText("Show "+actionText);
			}
			else
			{
				toolWindow( position)->centralWidget()->raiseWidget(widget);
				action->setText("Hide "+actionText);
			}
		}
	}
}

