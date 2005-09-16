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

// Qt
#include <qimage.h>
//


KTMainWindow::KTMainWindow() : QMainWindow(0, "KToon-MainWindow", Qt::WDestructiveClose)
{
	KTINIT;
	
	setCaption(tr("KToon: 2D animation tool kit"));
	
	m_workSpace = new QWorkspace(this);
	m_workSpace->setScrollBarsEnabled ( true );
	
	QMainWindow::setCentralWidget(m_workSpace);
	setupBackground();
	
	m_actionManager = new KTActionManager(this, "KTMainWindow-ACTMngr");
	
	// Create the menubar;
	setupFileActions();
	setupMenu();
	
	show();
}


KTMainWindow::~KTMainWindow()
{
	KTEND;
}

void KTMainWindow::setupMenu()
{
	// Setup the file menu
	m_fileMenu = new QPopupMenu(this);
	menuBar()->insertItem(tr("&File"), m_fileMenu);
	
	m_actionManager->find("NewFile")->addTo(m_fileMenu);
	m_actionManager->find("OpenFile")->addTo(m_fileMenu);
	
	QPopupMenu *recents = new QPopupMenu( this );
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
	m_editMenu = new QPopupMenu(this);
	menuBar()->insertItem( tr( "&Edit" ), m_editMenu );
	
	// Setup the view menu
	m_viewMenu = new QPopupMenu(this);
	menuBar()->insertItem( tr( "&View" ), m_viewMenu );
	
	// Setup the insert menu
	m_insertMenu = new QPopupMenu(this);
	menuBar()->insertItem( tr( "&Insert" ), m_insertMenu );
	
	// Setup the tools menu
	m_toolsMenu = new QPopupMenu(this);
	menuBar()->insertItem( tr( "&Tools" ), m_toolsMenu );
	
	// Setup the window menu
	m_windowMenu = new QPopupMenu(this);
	menuBar()->insertItem( tr( "&Window" ), m_windowMenu );
	
	// Setup the help menu
	m_helpMenu = new QPopupMenu(this);
	menuBar()->insertItem( tr( "&Help" ), m_helpMenu );
}

void KTMainWindow::createGUI()
{
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
	
	QAction *saveAs = new QAction( tr( "Save &As..." ), QString::null, this, "SaveAs");
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
	
	QAction *properties = new QAction( tr( "&Properties..." ),  QString::null, this, "Properties");
	connect(properties, SIGNAL(activated()), this, SLOT(properties()));
	properties->setStatusTip(tr("Opens the properties dialog box"));
	m_actionManager->insert(properties);
	
	QAction *exit = new QAction(QPixmap(export_xpm), tr( "E&xit" ),  tr("Ctrl+Q"), this, "Exit");
	connect(exit, SIGNAL(activated()), this, SLOT(close()));
	exit->setStatusTip(tr("Closes the application"));
	m_actionManager->insert(exit);
}

void KTMainWindow::setupToolBar()
{
}

void KTMainWindow::setupDialogs()
{
}

void KTMainWindow::closeEvent( QCloseEvent *event )
{
	QMainWindow::closeEvent(event);
}

void KTMainWindow::resizeEvent(QResizeEvent *event)
{
	QMainWindow::resizeEvent(event);
}

void KTMainWindow::updateOpenRecentMenu()
{
}

void KTMainWindow::setupBackground()
{
	QImage bgImg(background_xpm );
	KImageEffect::fade(bgImg, 0.2, palette().color(QPalette::Active , QColorGroup::Background) );
	bgImg.smoothScale(m_workSpace->size());
	
	m_workSpace->setPaletteBackgroundPixmap( bgImg );
}

void KTMainWindow::setPalette(const QPalette &pal)
{
	QMainWindow::setPalette(pal);
	setupBackground();
}

void KTMainWindow::newDocument()
{
	KTViewDocument *viewDocument = new KTViewDocument(m_workSpace);
	viewDocument->setActiveWindow();
	viewDocument->show();
}

