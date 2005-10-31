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

#include "status.h"

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

void KTMainWindow::setupMenu()
{
	// Setup the file menu
	m_fileMenu = new QMenu(this);
	menuBar()->insertItem(tr("&File"), m_fileMenu);
	
	m_fileMenu->addAction(m_actionManager->find("NewFile"));
	m_fileMenu->addAction(m_actionManager->find("OpenFile"));

	// <TEST>
	KTAction *play = new KTAction(QIcon(), "Play (test)", QKeySequence(), m_viewCamera->animationArea(), SLOT(play()), m_actionManager, "play");
	m_fileMenu->addAction(play);
	KTAction *stop = new KTAction(QIcon(), "Stop (test)", QKeySequence(), m_viewCamera->animationArea(), SLOT(stop()), m_actionManager, "stop");
	m_fileMenu->addAction(stop);
	// </TEST>
	
	QMenu *recents = new QMenu( this );
	connect( recents, SIGNAL( activated( int ) ), SLOT( openRecent( int ) ) );
	m_fileMenu->insertItem( tr( "Open Recent" ), recents );

	m_fileMenu->addAction(m_actionManager->find("Save"));
	m_fileMenu->addAction(m_actionManager->find("SaveAs"));
	m_fileMenu->addAction(m_actionManager->find("Close"));
	m_fileMenu->insertSeparator();
	m_fileMenu->addAction(m_actionManager->find("Import"));
	m_fileMenu->addAction(m_actionManager->find("Export"));
	m_fileMenu->insertSeparator();
	m_fileMenu->addAction(m_actionManager->find("Properties"));
	m_fileMenu->insertSeparator();
	m_fileMenu->addAction(m_actionManager->find("Exit"));
	m_fileMenu->insertSeparator();
	
	// Setup the edit menu
	m_editMenu = new QMenu(this);
	menuBar()->insertItem( tr( "&Edit" ), m_editMenu );

	m_editMenu->insertSeparator();
	
	m_editMenu->addAction(m_actionManager->find("cut"));
	m_editMenu->addAction(m_actionManager->find("copy"));
	m_editMenu->addAction(m_actionManager->find("paste"));
	m_editMenu->addAction(m_actionManager->find("paste in place"));
	m_editMenu->addAction(m_actionManager->find("delete"));
	m_editMenu->insertSeparator();
	m_editMenu->addAction(m_actionManager->find("select all"));
	m_editMenu -> insertSeparator();
	m_editMenu->addAction(m_actionManager->find("preferences"));
	
	// Setup the view menu
	m_viewMenu = new QMenu(this);
	menuBar()->insertItem( tr( "&View" ), m_viewMenu );
	
	// Setup the proyect menu
	m_proyectMenu = new QMenu(this);
	menuBar()->insertItem( tr( "&Project" ), m_proyectMenu );
	m_proyectMenu->addAction(m_actionManager->find("NewProject"));
	m_proyectMenu->addAction(m_actionManager->find("CloseProject"));
	m_proyectMenu->addAction(m_actionManager->find("OpenProject"));
	
	// Setup the insert menu
	m_insertMenu = new QMenu(this);
	menuBar()->insertItem( tr( "&Insert" ), m_insertMenu );
	
	// Setup the tools menu
	m_toolsMenu = new QMenu(this);
	menuBar()->insertItem( tr( "&Tools" ), m_toolsMenu );
	
	// Setup the window menu
	m_windowMenu = new QMenu(this);
	menuBar()->insertItem( tr( "&Window" ), m_windowMenu );
	
	// Setup the help menu
	m_helpMenu = new QMenu(this);
	menuBar()->insertItem( tr( "&Help" ), m_helpMenu );
	m_helpMenu->addAction(m_actionManager->find("about ktoon") );
	
}

void KTMainWindow::createGUI()
{
	// TODO: put setIcon in each class
	KTColorPalette *m_colorPalette = new KTColorPalette(this);
	m_colorPalette->setIcon(QPixmap(KTOON_THEME_DIR+"/icons/color_palette.png") );
	connect(m_colorPalette, SIGNAL(colorChanged(const QColor &, const QColor &)), this, SLOT(changeCurrentColors(const QColor &, const QColor &)));
	toolWindow(DDockWindow::Left)->addWidget(tr("Palette"),m_colorPalette);
	
	////////////////
	KTBrushWidget *m_brushWidget = new KTBrushWidget( this);
	m_brushWidget->setIcon(QPixmap(KTOON_THEME_DIR+"/icons/brushes.png"));
	toolWindow(DDockWindow::Left)->addWidget(tr("Brushes"),m_brushWidget);
	
	connect(m_brushWidget, SIGNAL(brushSelected(KTBrush *)), this, SLOT(changeCurrentBrush(KTBrush *)));
	
	////////////////////
	Library *m_libraryDialog = new Library( this, KTStatus->currentDrawingArea());
	m_libraryDialog->setIcon(QPixmap(KTOON_THEME_DIR+"/icons/library.png"));
	toolWindow(DDockWindow::Left)->addWidget(tr("Library"),m_libraryDialog);
	
	/////////////////
	m_scenes = new KTScenesWidget( this);
	m_scenes->setIcon(QPixmap(KTOON_THEME_DIR+"/icons/scenes.png"));
	toolWindow(DDockWindow::Right)->addWidget(tr("Scenes"),m_scenes);
	
	connect(m_scenes, SIGNAL(requestInsertScene()), m_projectManager, SLOT(createScene()));
	connect(m_scenes, SIGNAL(requestRemoveScene()), m_projectManager, SLOT(removeScene()));
	
	/////////////////////
	m_exposureSheet = new KTExposureSheet(this);
	m_exposureSheet->setIcon(QPixmap(KTOON_THEME_DIR+"/icons/exposure_sheet.png"));
	toolWindow(DDockWindow::Right)->addWidget(tr("Exposure Sheet"),m_exposureSheet);
	
	connect(m_exposureSheet, SIGNAL(requestInsertFrame(bool)), m_projectManager, SLOT(createFrame(bool)));
	
	connect(m_exposureSheet, SIGNAL(requestInsertLayer()), m_projectManager, SLOT(createLayer()));
	
	connect(m_exposureSheet, SIGNAL(frameSelected( int, int )), this, SLOT(selectFrame(int,int)));
	
	connect(m_exposureSheet, SIGNAL(layerVisibilityChanged( int, bool)), m_projectManager, SLOT(setLayerVisibility( int, bool)));
	
	
// 	connect(m_scenes, SIGNAL(sceneInserted( const QString &, int )), m_exposureSheet, SLOT(addScene( const QString &, int )));
// 	connect(m_scenes, SIGNAL(sceneRenamed( const QString &, int )), m_exposureSheet, SLOT(renameScene(const QString &, int)));
	
	//////////////////////
	KTTimeLine *m_timeLine = new KTTimeLine(this);
	m_timeLine->setIcon(QPixmap(KTOON_THEME_DIR+"/icons/time_line.png"));
	toolWindow(DDockWindow::Bottom)->addWidget(tr("Time Line"),m_timeLine);
	
	//////////////////
	KToonScript *m_scriptEditor = new KToonScript(this);
// 	m_scriptEditor->setIcon(QPixmap(KTOON_HOME+"/images/icons/color_palette.png") );
	toolWindow(DDockWindow::Bottom)->addWidget(tr("KToonScript"), m_scriptEditor);
	
	
	// Connect the project manager with the components...
	connect(m_projectManager, SIGNAL(sceneCreated(const QString &, bool)), this, SLOT( insertScene(const QString &, bool)));
	
	connect(m_projectManager, SIGNAL(frameCreated( const QString &, bool)), this, SLOT(insertFrame( const QString &, bool)));
	
	connect(m_projectManager, SIGNAL(layerCreated(const QString &, bool)), this, SLOT(insertLayer(const QString &, bool)));
	
	connect(m_projectManager, SIGNAL(layerVisibilityChanged(int, bool)), this, SLOT(setLayerVisibilityChanged(int, bool)));
	
}

void KTMainWindow::setupFileActions()
{
	KTAction *newFile = new KTAction( QPixmap( KTOON_THEME_DIR+"/icons/new.png" ), tr( "New Document" ), QKeySequence(tr("Ctrl+N")), this, SLOT(newViewDocument()), m_actionManager, "NewFile");
	
// 	connect(newFile, SIGNAL(activated()), this, SLOT(newDocument()));
	newFile->setStatusTip(tr( "Opens a new document"));
	
	KTAction *openFile = new KTAction( QPixmap(KTOON_THEME_DIR+"/icons/open.png"), tr( "Open Document" ), tr("Ctrl+O"), this, SLOT(chooseFile()), m_actionManager, "OpenFile");
// 	connect(openFile, SIGNAL(activated()), this, SLOT(chooseFile()));
	openFile->setStatusTip(tr("Loads an existent document"));
	
	KTAction *save = new KTAction( QPixmap(KTOON_THEME_DIR+"/icons/save.png"), tr( "Save Document" ),QKeySequence(tr("Ctrl+S")), this, SLOT(save()), m_actionManager, "Save");
// 	connect(save, SIGNAL(activated()), this, SLOT(save()));
	save->setStatusTip(tr("Saves the current document in the current location"));
	
	KTAction *saveAs = new KTAction( tr( "Save &As..." ), m_actionManager, "SaveAs");
	connect(saveAs, SIGNAL(activated()), this, SLOT(saveAs()));
	saveAs->setStatusTip(tr("Opens a dialog box to save the current document in any location"));
	
	KTAction *close = new KTAction(QPixmap(KTOON_THEME_DIR+"/icons/close.png"), tr( "Cl&ose" ), QKeySequence(tr("Ctrl+W")), m_actionManager, "Close");
	close->setStatusTip(tr("Closes the active document"));
	
	KTAction *import = new KTAction( QPixmap(KTOON_THEME_DIR+"/icons/import.png"), tr( "&Import..." ),  QKeySequence(tr("Ctrl+I")), this, SLOT(import()), m_actionManager, "Import");
// 	connect(import, SIGNAL(activated()), this, SLOT(import()));
	import->setStatusTip(tr("Imports a file in the supported format"));
	
	KTAction *exptr = new KTAction(QPixmap(KTOON_THEME_DIR+"/icons/export.png"), tr( "&Export..." ),  QKeySequence(tr("Ctrl+E")), this, SLOT(export()), m_actionManager, "Export");
// 	connect(exptr, SIGNAL(activated()), this, SLOT(export()));
	exptr->setStatusTip(tr("Exports this document as a file in the available formats"));
	exptr->setVisible(false);
	
	KTAction *properties = new KTAction( tr( "&Properties..." ), m_actionManager, "Properties");
	connect(properties, SIGNAL(activated()), this, SLOT(properties()));
	properties->setStatusTip(tr("Opens the properties dialog box"));
	
	KTAction *exit = new KTAction(QPixmap(KTOON_THEME_DIR+"/icons/export.png"), tr( "E&xit" ),  QKeySequence(tr("Ctrl+Q")), qApp, SLOT(closeAllWindows ()),m_actionManager, "Exit");
	exit->setStatusTip(tr("Closes the application"));
}

void KTMainWindow::setupEditActions()
{
	KTAction * undo = new KTAction( QPixmap(KTOON_THEME_DIR+"/icons/undo.png"), tr( "Undo" ), QKeySequence("Ctrl+Z"), this, SLOT(slotUndo()), m_actionManager, "undo");
	undo->setStatusTip(tr("Undoes the last draw action"));
	
	KTAction *redo = new KTAction( QPixmap(KTOON_THEME_DIR+"/icons/redo.png"), tr( "Redo" ), QKeySequence("CTRL+SHIFT+Z"), this, SLOT(slotRedo()), m_actionManager, "redo");
	redo->setStatusTip(tr("Redoes a previous undone action"));
	
	KTAction *cut = new KTAction( QPixmap(KTOON_THEME_DIR+"/icons/cut.png"), tr( "&Cut" ), QKeySequence("Ctrl+X"), this, SLOT(slotCut()), m_actionManager, "cut");
	cut->setStatusTip(tr("Cuts the selection and puts it onto the clipboard"));
	
	KTAction *copy = new KTAction( QPixmap(KTOON_THEME_DIR+"/icons/copy.png"), tr( "C&opy" ), QKeySequence("Ctrl+C"), this, SLOT(slotCopy()), m_actionManager, "copy");
	copy->setStatusTip(tr("Copies the selection and puts it onto the clipboard"));
	
	KTAction *paste = new KTAction( QPixmap(KTOON_THEME_DIR+"/icons/paste.png"), tr( "&Paste" ), QKeySequence("Ctrl+V"), this, SLOT(slotPaste()), m_actionManager, "paste");
	paste->setStatusTip(tr("Pastes the clipboard into the current document"));
	
	KTAction *pasteInPlace = new KTAction( tr(  "Paste &In Place" ),  QKeySequence("Ctrl+Shift+V"), this, SLOT(slotPasteInPlace()), m_actionManager, "paste in place");
	pasteInPlace->setStatusTip(tr("Pastes the clipboard into the same place as the copy was did"));
	
	KTAction * adelete = new KTAction( tr(  "&Delete" ), Qt::Key_Delete , this, SLOT(slotDelete()), m_actionManager, "delete");
	adelete->setStatusTip(tr("Deletes the selected object"));
	
	KTAction * selectAll = new KTAction( tr(  "&Select All" ),  tr("Ctrl+A"), this, SLOT(slotSelectAll()), m_actionManager, "select all");
	selectAll->setStatusTip(tr("Selects all objects in the document"));
	
	KTAction * preferences = new KTAction( tr( "Pr&eferences..." ), QKeySequence(), this, SLOT( preferences()), m_actionManager, "preferences");
	preferences->setStatusTip(tr("Opens the preferences dialog box"));
}

void KTMainWindow::setupProjectActions()
{
	KTAction *newProject = new KTAction( QPixmap(), tr( "New Project" ), QKeySequence(), this, SLOT(newProject()), m_actionManager, "NewProject");
	
	KTAction *closeProject = new KTAction( QPixmap(), tr( "Close Project" ), QKeySequence(), this, SLOT(currentProject()), m_actionManager, "CloseProject");
// 	newProject->setStatusTip(tr("create new project"));
	
	KTAction *openProject = new KTAction( QPixmap(), tr( "Open Project" ), QKeySequence(), this, SLOT(openProject()), m_actionManager, "OpenProject");
}

void KTMainWindow::setupHelpActions()
{
	new KTAction(QPixmap(), tr("About KToon"), QKeySequence(), this, SLOT(aboutKToon()), m_actionManager, "about ktoon");
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
}

void KTMainWindow::resizeEvent(QResizeEvent *event)
{
	DMainWindow::resizeEvent(event);
}

void KTMainWindow::updateOpenRecentMenu()
{
}

void KTMainWindow::setupBackground()
{
// 	QImage bgImg(background_xpm );
// 	KImageEffect::fade(bgImg, 0.2, palette().color(QPalette::Active , QColorGroup::Background) );
// 	bgImg.smoothScale(m_drawingSpace->size());
	
// 	m_drawingSpace->setPaletteBackgroundPixmap( bgImg );
}

void KTMainWindow::setPalette(const QPalette &pal)
{
	DMainWindow::setPalette(pal);
	setupBackground();
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

// Animation

void KTMainWindow::insertScene( const QString &name, bool addedToEnd)
{
	m_scenes->insertScene( name, addedToEnd);
	m_exposureSheet->addScene( name);
}

void KTMainWindow::insertLayer(const QString &name, bool addedToEnd)
{
	ktDebug() << "KTMainWindow::insertLayer(" << name << "," << addedToEnd << ")" << endl;
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
// 		doc->drawArea()->setScene( m_projectManager->currentScene() );
	}
	
	m_exposureSheet->insertLayer(name);
}

void KTMainWindow::setLayerVisibilityChanged(int idLayer, bool isVisible)
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		doc->drawArea()->redrawAll();
	}
}


void KTMainWindow::insertFrame(const QString &name, bool addedToEnd)
{
	ktDebug() << "KTMainWindow::insertFrame(" << name << "," << addedToEnd << ")" << endl;
	ktDebug() << k_funcinfo << endl;
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
// 		doc->drawArea()->setKeyFrame( m_projectManager->currentLayer()->frames().count()-1);
// 		ktDebug( ) << "******************" << endl;
		
		
// 		doc->drawArea()->setKeyFrame(m_projectManager->currentLayer()->indexCurrentFrame());
		
	}
	
	m_exposureSheet->addFrame(m_projectManager->currentScene()->indexCurrentLayer(), name, addedToEnd);
}

void KTMainWindow::selectFrame(int layer, int frame)
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		ktDebug() << "SELECT LAYER: " << layer << " FRAME: " << frame << endl;
		m_projectManager->currentScene()->setCurrentLayer(layer);
		m_projectManager->setCurrentFrame( frame );
		
		doc->drawArea()->setLayer( layer );
		doc->drawArea()->setKeyFrame( frame );
	}
}

// Drawing
void KTMainWindow::changeCurrentColors(const QColor &foreground, const QColor &background)
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		doc->drawArea()->setPenColor(foreground);
		doc->drawArea()->setBrushColor(background);
	}
}

void KTMainWindow::changeCurrentBrush(KTBrush *brush)
{
	KTViewDocument *doc = qobject_cast<KTViewDocument *>(m_drawingSpace->activeWindow ());
	
	if ( doc )
	{
		doc->drawArea()->setBrush(brush);
	}
}

