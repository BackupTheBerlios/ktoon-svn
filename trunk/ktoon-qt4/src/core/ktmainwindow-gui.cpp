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

#include "ktdebug.h"
#include "ktapplication.h"

void KTMainWindow::setupMenu()
{
	// Setup the file menu
	m_fileMenu = new QMenu(tr("&File"), this);
	menuBar()->addMenu(m_fileMenu);
	
	m_fileMenu->addAction(m_actionManager->find("NewFile"));
	m_fileMenu->addAction(m_actionManager->find("OpenFile"));
	
	QMenu *recents = new QMenu(tr("Recents"), this );
	connect( recents, SIGNAL( activated( int ) ), SLOT( openRecent( int ) ) );
	m_fileMenu->addMenu( recents );

	m_fileMenu->addAction(m_actionManager->find("Save"));
	m_fileMenu->addAction(m_actionManager->find("SaveAs"));
	m_fileMenu->addAction(m_actionManager->find("Close"));
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_actionManager->find("Import"));
	m_fileMenu->addAction(m_actionManager->find("ImportPalettes"));
	
	m_fileMenu->addAction(m_actionManager->find("Export"));
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_actionManager->find("Properties"));
	m_fileMenu->addSeparator();
	m_fileMenu->addAction(m_actionManager->find("Exit"));
	m_fileMenu->addSeparator();
	
	// Setup the edit menu
	m_editMenu = new QMenu(tr( "&Edit" ), this);
	menuBar()->addMenu( m_editMenu );

	m_editMenu->addSeparator();
	
	m_editMenu->addAction(m_actionManager->find("cut"));
	m_editMenu->addAction(m_actionManager->find("copy"));
	m_editMenu->addAction(m_actionManager->find("paste"));
	m_editMenu->addAction(m_actionManager->find("paste in place"));
	m_editMenu->addAction(m_actionManager->find("delete"));
	m_editMenu->addSeparator();
	m_editMenu->addAction(m_actionManager->find("select all"));
	m_editMenu -> addSeparator();
	m_editMenu->addAction(m_actionManager->find("preferences"));
	
	// Setup the view menu
	m_viewMenu = new QMenu(tr( "&View" ), this);
	menuBar()->addMenu( m_viewMenu );
	
	// Setup the proyect menu
	m_proyectMenu = new QMenu(tr( "&Project" ),this);
	menuBar()->addMenu(  m_proyectMenu );
	m_proyectMenu->addAction(m_actionManager->find("NewProject"));
	m_proyectMenu->addAction(m_actionManager->find("CloseProject"));
	m_proyectMenu->addAction(m_actionManager->find("OpenProject"));
	
	// Setup the insert menu
	m_insertMenu = new QMenu(tr( "&Insert" ), this);
	menuBar()->addMenu( m_insertMenu );
	
	// Setup the tools menu
	m_toolsMenu = new QMenu(tr( "&Tools" ),this);
	menuBar()->addMenu(  m_toolsMenu );
	
	// Setup the window menu
	setupWindowActions();
	m_windowMenu = new QMenu(tr( "&Window" ),this);
	menuBar()->addMenu(  m_windowMenu );
	m_windowMenu->addAction(m_actionManager->find("show timeline"));
	m_windowMenu->addAction(m_actionManager->find("show exposure"));
	m_windowMenu->addAction(m_actionManager->find("show brushes"));
	m_windowMenu->addAction(m_actionManager->find("show scenes"));
	
	// Setup the help menu
	m_helpMenu = new QMenu(tr( "&Help" ),this);
	menuBar()->addMenu(  m_helpMenu );
	m_helpMenu->addAction(m_actionManager->find("about ktoon") );
	
}

void KTMainWindow::createGUI()
{
	// TODO: put setWindowIcon in each class
	m_colorPalette = new KTColorPalette(this);
	m_colorPalette->setWindowIcon(QPixmap(KTOON_THEME_DIR+"/icons/color_palette.png") );
	connect(m_colorPalette, SIGNAL(colorChanged(const QBrush &, const QBrush &)), this, SLOT(changeCurrentColors(const QBrush &, const QBrush &)));
	toolWindow(DDockWindow::Left)->addWidget(tr("Palette"),m_colorPalette);
	
	connectToDisplays(m_colorPalette);
	
	////////////////
	m_brushWidget = new KTBrushWidget( this);
	m_brushWidget->setWindowIcon(QPixmap(KTOON_THEME_DIR+"/icons/brushes.png"));
	toolWindow(DDockWindow::Left)->addWidget(tr("Brushes"),m_brushWidget);
	
	connect(m_brushWidget, SIGNAL(brushSelected( const KTBrush *)), this, SLOT(changeCurrentBrush(  const KTBrush * )));
	
	connectToDisplays(m_brushWidget);
	
	////////////////////
	m_libraryWidget = new KTLibraryWidget( this );
	m_libraryWidget->setWindowIcon(QPixmap(KTOON_THEME_DIR+"/icons/library.png"));
	toolWindow(DDockWindow::Left)->addWidget(tr("Library"),m_libraryWidget);
	
	connect(m_libraryWidget, SIGNAL(requestCurrentGraphic()), this, SLOT(addCurrentGraphicToLibrary()));
	connect(m_libraryWidget, SIGNAL(sendCurrentGraphic(const AGraphicComponent *)), this, SLOT(addGraphicComponent(const AGraphicComponent *)));
	
	connectToDisplays(m_libraryWidget);
	
	/////////////////
	m_scenes = new KTScenesWidget( this);
	m_scenes->setWindowIcon(QPixmap(KTOON_THEME_DIR+"/icons/scenes.png"));
	toolWindow(DDockWindow::Right)->addWidget(tr("Scenes"),m_scenes);
	
	connect(m_scenes, SIGNAL(requestInsertScene()), m_projectManager, SLOT(createScene()));
	connect(m_scenes, SIGNAL(requestRemoveScene()), m_projectManager, SLOT(removeScene()));
	connect(m_scenes, SIGNAL(changeCurrentScene( int )), this, SLOT(changeScene(int)));
	
	connectToDisplays(m_scenes);
	
	/////////////////////
	m_exposureSheet = new KTExposureSheet(this);
	m_exposureSheet->setWindowIcon(QPixmap(KTOON_THEME_DIR+"/icons/exposure_sheet.png"));
	toolWindow(DDockWindow::Right)->addWidget(tr("Exposure Sheet"),m_exposureSheet);
	
	ui4project( m_exposureSheet );
	connect(m_exposureSheet, SIGNAL(requestChangeScene( int ) ), this, SLOT(changeScene(int)));

	connectToDisplays(m_exposureSheet);
	
	///////////////////////
	m_helper = new KTHelpWidget(KTOON_HOME+"/data/help/");
	toolWindow(DDockWindow::Right)->addWidget(tr("Help"),m_helper);
	connect(m_helper, SIGNAL(pageLoaded(const QString &, const QString &)), this, SLOT(showHelpPage(const QString &, const QString &)));
	
	connectToDisplays(m_helper);
	
	//////////////////////
	m_timeLine = new KTTimeLine(this);
	m_timeLine->setWindowIcon(QPixmap(KTOON_THEME_DIR+"/icons/time_line.png"));
	toolWindow(DDockWindow::Bottom)->addWidget(tr("Time Line"),m_timeLine);
	connect(m_timeLine, SIGNAL(requestChangeFPS(int)), this, SLOT(changeFPS( int )));
// 	connect(m_timeLine, SIGNAL(requestChangeScene( int ) ), this, SLOT(changeScene(int)));
	ui4project( m_timeLine );
	
	connectToDisplays(m_timeLine);
	
	
	//////////////////
// 	KToonScript *m_scriptEditor = new KToonScript(this);
// 	m_scriptEditor->setWindowIcon(QPixmap(KTOON_HOME+"/images/icons/color_palette.png") );
// 	toolWindow(DDockWindow::Bottom)->addWidget(tr("KToonScript"), m_scriptEditor);
	
	/////////////////
	
	KTGCEditor *m_gcEditor = new KTGCEditor(this);
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
}

void KTMainWindow::connectToDisplays(const QWidget *widget)
{
	connect(widget, SIGNAL(sendToStatus(const QString &)), this, SLOT(messageToStatus(const QString &)));
	connect(widget, SIGNAL(sendToOSD(const QString &)), this, SLOT(messageToOSD(const QString &)));
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
	
	KTAction *importPalette = new KTAction( QPixmap(KTOON_THEME_DIR+"/icons/import.png"), tr( "&Import GIMP palettes..." ),  QKeySequence(), this, SLOT(importPalettes()), m_actionManager, "ImportPalettes");
	importPalette->setStatusTip(tr("Imports palettes"));
	
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
	
	KTAction *closeProject = new KTAction( QPixmap(), tr( "Close Project" ), QKeySequence(), this, SLOT(closeProject()), m_actionManager, "CloseProject");
	
	KTAction *openProject = new KTAction( QPixmap(), tr( "Open Project" ), QKeySequence(), this, SLOT(openProject()), m_actionManager, "OpenProject");
}

void KTMainWindow::setupHelpActions()
{
	new KTAction(QPixmap(), tr("About KToon"), QKeySequence(), this, SLOT(aboutKToon()), m_actionManager, "about ktoon");
}

void KTMainWindow::setupWindowActions()
{
	new KTAction(QPixmap(), tr("Show TimeLine widget"), QKeySequence("CTRL+K"), this, SLOT(showWidgetPage()), m_actionManager, "show timeline");
	new KTAction(QPixmap(), tr("Show exposure sheet widget"), QKeySequence("CTRL+H"), this, SLOT(showWidgetPage()), m_actionManager, "show exposure");
	new KTAction(QPixmap(), tr("Show scenes widget"), QKeySequence("CTRL+Y"), this, SLOT(showWidgetPage()), m_actionManager, "show scenes");
	new KTAction(QPixmap(), tr("Show brushes widget"), QKeySequence("CTRL+B"), this, SLOT(showWidgetPage()), m_actionManager, "show brushes");
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

void KTMainWindow::showWidgetPage()
{
	KTAction *action = qobject_cast<KTAction *>(sender());
	
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
		else if ( action == m_actionManager->find("show brushes") )
		{
			widget = m_brushWidget;
			position = DDockWindow::Left;
			actionText = "brushes widget";
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
