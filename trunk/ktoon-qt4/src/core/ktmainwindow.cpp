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
#include "images.h"
#include "ktdebug.h"
#include "kimageeffect.h"

#include "status.h"

#include "ktapplication.h"

// dlslib
#include "dtabwidget.h"

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
	
	KTStatus->setupDrawingArea(m_workSpace);

	addWidget(m_workSpace, tr("Scene 1"));

	setupBackground();
	
	m_actionManager = new KTActionManager(this, "KTMainWindow-ACTMngr");
	
	// Create the menubar;
	setupFileActions();
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
	
	// Setup the view menu
	m_viewMenu = new QMenu(this);
	menuBar()->insertItem( tr( "&View" ), m_viewMenu );
	
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
	ColorPalette *m_colorPalette = new ColorPalette(this);
	m_colorPalette->setIcon(QPixmap(KTOON_HOME+"/images/icons/color_palette.xpm") );
	toolWindow(DDockWindow::Left)->addWidget(tr("Palette"),m_colorPalette);
	
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
	
	QTextEdit *m_scriptEditor = new QTextEdit("Soon...", this);
	m_scriptEditor->setIcon(QPixmap(KTOON_HOME+"/images/icons/color_palette.xpm") );
	toolWindow(DDockWindow::Bottom)->addWidget(tr("KToonScript"), m_scriptEditor);
}

void KTMainWindow::setupFileActions()
{
	QAction *newFile = new QAction( QPixmap( new_xpm ), tr( "New Document" ), tr("Ctrl+N"), this, "NewFile");
	connect(newFile, SIGNAL(activated()), this, SLOT(newDocument()));
	newFile->setStatusTip(tr( "Opens a new document"));
	m_actionManager->insert(newFile);
	
	QAction *openFile = new QAction( QPixmap(open_xpm), tr( "Open Document" ), tr("Ctrl+O"), this, "OpenFile");
	connect(openFile, SIGNAL(activated()), this, SLOT(chooseFile()));
	openFile->setStatusTip(tr("Loads an existent document"));
	m_actionManager->insert(openFile);
	
	QAction *save = new QAction( QPixmap(save_xpm), tr( "Save Document" ),tr("Ctrl+S") , this, "Save");
	connect(save, SIGNAL(activated()), this, SLOT(save()));
	save->setStatusTip(tr("Saves the current document in the current location"));
	m_actionManager->insert(save);
	
	QAction *saveAs = new QAction( tr( "Save &As..." ), QKeySequence(), this, "SaveAs");
	connect(saveAs, SIGNAL(activated()), this, SLOT(saveAs()));
	saveAs->setStatusTip(tr("Opens a dialog box to save the current document in any location"));
	m_actionManager->insert(saveAs);
	
	QAction *close = new QAction(QPixmap(close_xpm), tr( "Cl&ose" ), tr("Ctrl+W"), this, "Close");
	close->setStatusTip(tr("Closes the active document"));
	m_actionManager->insert(close);
	
	QAction *import = new QAction( QPixmap(import_xpm), tr( "&Import..." ),  tr("Ctrl+I"), this, "Import");
	connect(import, SIGNAL(activated()), this, SLOT(import()));
	import->setStatusTip(tr("Imports a file in the supported format"));
	m_actionManager->insert(import);
	
	QAction *exptr = new QAction(QPixmap(export_xpm), tr( "&Export..." ),  tr("Ctrl+E"), this, "Export");
	connect(exptr, SIGNAL(activated()), this, SLOT(export()));
	exptr->setStatusTip(tr("Exports this document as a file in the available formats"));
	exptr->setVisible(false);
	m_actionManager->insert(exptr);
	
	QAction *properties = new QAction( tr( "&Properties..." ),  QKeySequence(), this, "Properties");
	connect(properties, SIGNAL(activated()), this, SLOT(properties()));
	properties->setStatusTip(tr("Opens the properties dialog box"));
	m_actionManager->insert(properties);
	
	QAction *exit = new QAction(QPixmap(export_xpm), tr( "E&xit" ),  tr("Ctrl+Q"), this, "Exit");
	connect(exit, SIGNAL(activated()), this, SLOT(close()));
	exit->setStatusTip(tr("Closes the application"));
	m_actionManager->insert(exit);
	
	QAction *resize  = new QAction("Resize", this);
	resize->setObjectName("Resize");
	connect(resize, SIGNAL(activated()), this, SLOT(resize()));
	m_actionManager->insert(resize);
	
}

#include "docksplitter.h"

void KTMainWindow::resize()
{
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

void KTMainWindow::newDocument()
{
	static_cast<KTStatusBar*>(statusBar())->setStatus(tr("Opening a new document..."));
	
	KTViewDocument *viewDocument = new KTViewDocument(m_workSpace);
	m_workSpace->addWindow(viewDocument);
	static_cast<KTStatusBar*>(statusBar())->advance(4);
	viewDocument->setActiveWindow();
	static_cast<KTStatusBar*>(statusBar())->advance(7);
	viewDocument->show();
	static_cast<KTStatusBar*>(statusBar())->advance(10);
	static_cast<KTStatusBar*>(statusBar())->setStatus(tr("Opened."));
}

