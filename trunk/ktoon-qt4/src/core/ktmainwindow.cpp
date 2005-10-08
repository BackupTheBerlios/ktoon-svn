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
#include "images.h"
#include "ktdebug.h"
#include "kimageeffect.h"

#include "status.h"

#include "ktapplication.h"

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

KTMainWindow::KTMainWindow() : DMainWindow(0, "KToon-MainWindow")
{
	KTINIT;

	setStatusBar( new KTStatusBar(this) );
	
	setWindowTitle(tr("KToon: 2D animation tool kit"));
	
	m_workSpace = new QWorkspace;
	m_workSpace->setScrollBarsEnabled ( true );
	
// 	m_workSpace->setBackground(QBrush(QPixmap(background_xpm))); 
	
	newDocument();
	
	addWidget(m_workSpace, tr("Scene 1"));

	setupBackground();
	
	m_actionManager = new KTActionManager(this);
	
	// Create the menubar;
	setupFileActions();
	setupEditActions();
	setupProjectActions();
	
	setupMenu();
	
	statusBar()->message("Ready to play!");
	
	createGUI();
	
	showMaximized();
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
	
	m_actionManager->find("NewFile")->addTo(m_fileMenu);
	m_actionManager->find("OpenFile")->addTo(m_fileMenu);
	
	QMenu *recents = new QMenu( this );
	connect( recents, SIGNAL( activated( int ) ), SLOT( openRecent( int ) ) );
	m_fileMenu->insertItem( tr( "Open Recent" ), recents );

	m_actionManager->find("Save")->addTo(m_fileMenu);
	m_actionManager->find("SaveAs")->addTo(m_fileMenu);
	m_actionManager->find("Close")->addTo(m_fileMenu);
	m_fileMenu->insertSeparator();
	m_actionManager->find("Import")->addTo(m_fileMenu);
	m_actionManager->find("Export")->addTo(m_fileMenu);
	m_fileMenu->insertSeparator();
	m_actionManager->find("Properties")->addTo(m_fileMenu);
	m_fileMenu->insertSeparator();
	m_actionManager->find("Exit")->addTo(m_fileMenu);
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
	m_helpMenu->addAction(m_actionManager->find("Resize") );
}

void KTMainWindow::createGUI()
{
	// TODO: put setIcon in each class
	KTColorPalette *m_colorPalette = new KTColorPalette(this);
	m_colorPalette->setIcon(QPixmap(KTOON_HOME+"/images/icons/color_palette.xpm") );
	toolWindow(DDockWindow::Left)->addWidget(tr("Palette"),m_colorPalette);
// 	connect(m_colorPalette, SIGNAL(colorChanged(QColor)), 
	
	
	Brushes *m_brushesDialog = new Brushes( this);
	m_brushesDialog->setIcon(QPixmap(KTOON_HOME+"/images/icons/brushes.xpm"));
	toolWindow(DDockWindow::Left)->addWidget(tr("Brushes"),m_brushesDialog);
	
	Library *m_libraryDialog = new Library( this, KTStatus->currentDrawingArea());
	m_libraryDialog->setIcon(QPixmap(KTOON_HOME+"/images/icons/library.xpm"));
	toolWindow(DDockWindow::Left)->addWidget(tr("Library"),m_libraryDialog);
	
	Scenes *m_scenes = new Scenes( this);
	toolWindow(DDockWindow::Right)->addWidget(tr("Scenes"),m_scenes);
	
	KTExposureSheet *m_exposureSheet = new KTExposureSheet(this);
	m_exposureSheet->setIcon(QPixmap(KTOON_HOME+"/images/icons/exposure_sheet.xpm"));
	toolWindow(DDockWindow::Right)->addWidget(tr("Exposure Sheet"),m_exposureSheet);
	
	KTTimeLine *m_timeLine = new KTTimeLine(this);
	m_timeLine->setIcon(QPixmap(KTOON_HOME+"/images/icons/time_line.xpm"));
	toolWindow(DDockWindow::Bottom)->addWidget(tr("Time Line"),m_timeLine);
	
	KToonScript *m_scriptEditor = new KToonScript(this);
// 	m_scriptEditor->setIcon(QPixmap(KTOON_HOME+"/images/icons/color_palette.xpm") );
	toolWindow(DDockWindow::Bottom)->addWidget(tr("KToonScript"), m_scriptEditor);
}

void KTMainWindow::setupFileActions()
{
	KTAction *newFile = new KTAction( QPixmap( new_xpm ), tr( "New Document" ), QKeySequence(tr("Ctrl+N")), this, SLOT(newDocument()), m_actionManager, "NewFile");
	
// 	connect(newFile, SIGNAL(activated()), this, SLOT(newDocument()));
	newFile->setStatusTip(tr( "Opens a new document"));
	
	KTAction *openFile = new KTAction( QPixmap(open_xpm), tr( "Open Document" ), tr("Ctrl+O"), this, SLOT(chooseFile()), m_actionManager, "OpenFile");
// 	connect(openFile, SIGNAL(activated()), this, SLOT(chooseFile()));
	openFile->setStatusTip(tr("Loads an existent document"));
	
	KTAction *save = new KTAction( QPixmap(save_xpm), tr( "Save Document" ),QKeySequence(tr("Ctrl+S")), this, SLOT(save()), m_actionManager, "Save");
// 	connect(save, SIGNAL(activated()), this, SLOT(save()));
	save->setStatusTip(tr("Saves the current document in the current location"));
	
	KTAction *saveAs = new KTAction( tr( "Save &As..." ), m_actionManager, "SaveAs");
	connect(saveAs, SIGNAL(activated()), this, SLOT(saveAs()));
	saveAs->setStatusTip(tr("Opens a dialog box to save the current document in any location"));
	
	KTAction *close = new KTAction(QPixmap(close_xpm), tr( "Cl&ose" ), QKeySequence(tr("Ctrl+W")), m_actionManager, "Close");
	close->setStatusTip(tr("Closes the active document"));
	
	KTAction *import = new KTAction( QPixmap(import_xpm), tr( "&Import..." ),  QKeySequence(tr("Ctrl+I")), this, SLOT(import()), m_actionManager, "Import");
// 	connect(import, SIGNAL(activated()), this, SLOT(import()));
	import->setStatusTip(tr("Imports a file in the supported format"));
	
	KTAction *exptr = new KTAction(QPixmap(export_xpm), tr( "&Export..." ),  QKeySequence(tr("Ctrl+E")), this, SLOT(export()), m_actionManager, "Export");
// 	connect(exptr, SIGNAL(activated()), this, SLOT(export()));
	exptr->setStatusTip(tr("Exports this document as a file in the available formats"));
	exptr->setVisible(false);
	
	KTAction *properties = new KTAction( tr( "&Properties..." ), m_actionManager, "Properties");
	connect(properties, SIGNAL(activated()), this, SLOT(properties()));
	properties->setStatusTip(tr("Opens the properties dialog box"));
	
	KTAction *exit = new KTAction(QPixmap(export_xpm), tr( "E&xit" ),  QKeySequence(tr("Ctrl+Q")), qApp, SLOT(closeAllWindows ()),m_actionManager, "Exit");
	exit->setStatusTip(tr("Closes the application"));
}

void KTMainWindow::setupEditActions()
{
	KTAction * undo = new KTAction( QPixmap(undo_xpm), tr( "Undo" ), QKeySequence("Ctrl+Z"), this, SLOT(slotUndo()), m_actionManager, "undo");
	undo->setStatusTip(tr("Undoes the last draw action"));
	
	KTAction *redo = new KTAction( QPixmap(redo_xpm), tr( "Redo" ), QKeySequence("CTRL+SHIFT+Z"), this, SLOT(slotRedo()), m_actionManager, "redo");
	redo->setStatusTip(tr("Redoes a previous undone action"));
	
	KTAction *cut = new KTAction( QPixmap(cut_xpm), tr( "&Cut" ), QKeySequence("Ctrl+X"), this, SLOT(slotCut()), m_actionManager, "cut");
	cut->setStatusTip(tr("Cuts the selection and puts it onto the clipboard"));
	
	KTAction *copy = new KTAction( QPixmap(copy_xpm), tr( "C&opy" ), QKeySequence("Ctrl+C"), this, SLOT(slotCopy()), m_actionManager, "copy");
	copy->setStatusTip(tr("Copies the selection and puts it onto the clipboard"));
	
	KTAction *paste = new KTAction( QPixmap(paste_xpm), tr( "&Paste" ), QKeySequence("Ctrl+V"), this, SLOT(slotPaste()), m_actionManager, "paste");
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

void KTMainWindow::setupToolBar()
{
}

void KTMainWindow::setupDialogs()
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
// 	bgImg.smoothScale(m_workSpace->size());
	
// 	m_workSpace->setPaletteBackgroundPixmap( bgImg );
}

void KTMainWindow::setPalette(const QPalette &pal)
{
	DMainWindow::setPalette(pal);
	setupBackground();
}

void KTMainWindow::newDocument(const QString &name, const QSize &size)
{
	static_cast<KTStatusBar*>(statusBar())->setStatus(tr("Opening a new document..."));
	
	KTViewDocument *viewDocument = new KTViewDocument(m_workSpace);
	m_workSpace->addWindow(viewDocument);
	viewDocument->setWindowTitle(name);
	
	static_cast<KTStatusBar*>(statusBar())->advance(4);
	viewDocument->setActiveWindow();
	static_cast<KTStatusBar*>(statusBar())->advance(7);
	viewDocument->show();
	static_cast<KTStatusBar*>(statusBar())->advance(10);
	static_cast<KTStatusBar*>(statusBar())->setStatus(tr("Opened."));
}

void KTMainWindow::newProject()
{
	KTNewProject *wizard = new KTNewProject;
	if ( wizard->exec() != QDialog::Rejected )
	{
		newDocument( wizard->projectName() );
	}
	delete wizard;
}

void KTMainWindow::closeProject()
{
	
}

void KTMainWindow::openProject()
{
	
}


void KTMainWindow::preferences()
{
	Preferences *preferences = new Preferences( this );
	preferences->exec();
	statusBar() -> message( tr( "Preferences Dialog Opened" ), 2000 );
}


