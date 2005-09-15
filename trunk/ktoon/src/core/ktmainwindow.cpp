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
	
	// Create the menubar;
	setupMenu();
	
	
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
	
	QPopupMenu *recents = new QPopupMenu( this );
	connect( recents, SIGNAL( activated( int ) ), SLOT( slotOpenRecent( int ) ) );
	m_fileMenu->insertItem( tr( "Open Recent" ), recents );
	
	m_fileMenu->insertSeparator();
	
	// Setup the edit menu
	m_editMenu = new QPopupMenu(this);
	menuBar() -> insertItem( tr( "&Edit" ), m_editMenu );
	
	// Setup the view menu
	m_viewMenu = new QPopupMenu(this);
	menuBar() -> insertItem( tr( "&View" ), m_viewMenu );
	
	// Setup the insert menu
	m_insertMenu = new QPopupMenu(this);
	menuBar() -> insertItem( tr( "&Insert" ), m_insertMenu );
	
	// Setup the tools menu
	m_toolsMenu = new QPopupMenu(this);
	menuBar() -> insertItem( tr( "&Tools" ), m_toolsMenu );
	
	// Setup the window menu
	m_windowMenu = new QPopupMenu(this);
	menuBar() -> insertItem( tr( "&Window" ), m_windowMenu );
	
	// Setup the help menu
	m_helpMenu = new QPopupMenu(this);
	menuBar() -> insertItem( tr( "&Help" ), m_helpMenu );
}

void KTMainWindow::createGUI()
{
}

void KTMainWindow::createActions()
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

