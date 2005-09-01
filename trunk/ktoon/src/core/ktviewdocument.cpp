/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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

#include "ktviewdocument.h"
#include "ktapplication.h"
#include "ktdebug.h"

#include <qlayout.h>
#include <qstatusbar.h>
#include <qmenubar.h>

#include <qcursor.h>


KTViewDocument::KTViewDocument(QWidget *parent , const char *name, WFlags f)
	: KTMdiWindow(parent, name, f)
{
	m_container = new QWidget(this);
	m_HRuler = new KTDocumentRuler(KTDocumentRuler::Horizontal, m_container);
	m_VRuler = new KTDocumentRuler(KTDocumentRuler::Vertical, m_container);
	
	QGridLayout *grid = new QGridLayout(m_container,1,1);
	grid->addWidget (m_HRuler,0,1);
	grid->addWidget (m_VRuler,1,0);
	
	m_drawArea  = new DrawingArea(m_container);

	QObject::connect( m_drawArea, SIGNAL(mousePos(QPoint)),  m_HRuler,  SLOT(movePointers(QPoint)) );
	QObject::connect( m_drawArea, SIGNAL(mousePos(QPoint)),  m_VRuler,  SLOT(movePointers(QPoint)) );
	
	QObject::connect( m_drawArea, SIGNAL(mousePos(QPoint)),  this,  SLOT(showPos(QPoint)) );
	
	
	grid->addWidget (m_drawArea,1,1);
	setCentralWidget ( m_container );
	showPos(QPoint(0,0));
	adjustSize();
	createActions();
	createToolbar();
	createMenu();
}


KTViewDocument::~KTViewDocument()
{
	
}

void KTViewDocument::showPos(QPoint p)
{
	QString messages =  "x: " +  QString::number(p.x()) +  " y: " + QString::number(p.y());
	statusBar()->message ( messages ) ;
}

void KTViewDocument::createActions()
{
	gridGroup = new QActionGroup( parent(), tr("&Grid" ));
	gridGroup->setExclusive( true ); 
	
	QAction *a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/nogrid.xpm" ), tr( "&No Grid" ), QString::null, gridGroup);
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotNoGrid()));
	a->setToggleAction ( true );
	a->setStatusTip(tr("Hides the grid" ));
	
	a = new QAction(QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/grid12.xpm" ),  tr( "&12 Field Grid" ), QString::null, gridGroup);
	a->setToggleAction ( true );
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotSeeGrid12()));
	a->setStatusTip(tr("Shows a 12 field grid" ));
	
	a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/grid16.xpm" ), tr( "&16 Field Grid" ), QString::null, gridGroup);
	a->setToggleAction ( true );
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotSeeGrid16()));
	a->setStatusTip(tr("Shows a 16 field grid" ));
	a->setOn(true);
	
	m_aSubGrid = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/subgrid.xpm" ), tr( "&Subgrid" ), QString::null, parent()),
	m_aSubGrid->setToggleAction ( true );
	connect(m_aSubGrid, SIGNAL(activated()), m_drawArea, SLOT(slotSeeSubgrid()));
	m_aSubGrid->setStatusTip(tr("Shows or hides a 3 field subgrid in the current grid" ));
	
	m_aFrontBackGrid = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/front_back_grid.xpm"), tr( "Grid to Front/Back" ), QString::null, this);
	m_aFrontBackGrid->setToggleAction ( true );
	m_aFrontBackGrid->setOn(true);
	connect(m_aFrontBackGrid, SIGNAL(activated()), m_drawArea, SLOT(slotFrontBackGrid()));
	m_aFrontBackGrid->setStatusTip(tr("Sends the grid to the front or to the back of the drawing area" ));
	
	m_aUndo = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/undo.xpm" ), tr( "Undo" ),tr("Ctrl+Z") , parent());
	connect(m_aUndo, SIGNAL(activated()), m_drawArea, SLOT(slotUndo()));
	m_aUndo->setStatusTip(tr("Undoes the last draw action"));
	
	m_aRedo = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/redo.xpm" ), tr( "Redo" ), tr("CTRL+SHIFT+Z"), parent());
	connect(m_aRedo, SIGNAL(activated()), m_drawArea, SLOT(slotRedo()));
	m_aRedo->setStatusTip(tr("Redoes a previous undone action"));
	
	
	editGroup = new QActionGroup( parent() );
	a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/cut.xpm" ), tr( "&Cut" ), tr("Ctrl+X"), editGroup);
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotCut()));
	a->setStatusTip(tr("Cuts the selection and puts it onto the clipboard"));
	
	a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/copy.xpm" ), tr( "C&opy" ), tr("Ctrl+C"), editGroup);
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotCopy()));
	a->setStatusTip(tr("Copies the selection and puts it onto the clipboard"));
	
	a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/paste.xpm" ), tr( "&Paste" ), tr("Ctrl+V"), editGroup);
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotPaste()));
	a->setStatusTip(tr("Pastes the clipboard into the current document"));
	
	
	editGroup2 = new QActionGroup( parent() );
	a = new QAction( tr(  "Paste &In Place" ),  tr("Ctrl+Shift+V"), editGroup2);
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotPasteInPlace()));
	a->setStatusTip(tr("Pastes the clipboard into the same place as the copy was did"));
	
	a = new QAction( tr(  "&Delete" ), Key_Delete , editGroup2);
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotDelete()));
	a->setStatusTip(tr("Deletes the selected object"));
	
	a = new QAction( tr(  "&Select All" ),  tr("Ctrl+A"), editGroup2);
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotSelectAll()));
	a->setStatusTip(tr("Selects all objects in the document"));
	
	m_aNtsc = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/ntsc.xpm" ), tr( "&NTSC Zone" ), QString::null, parent());
	m_aNtsc->setToggleAction ( true );
	connect(m_aNtsc, SIGNAL(activated()), m_drawArea, SLOT(slotSeeNTSC()));
	m_aNtsc->setStatusTip(tr("Shows or hides the NTSC Zone" ));
	
	m_aLightTable = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/light_table.xpm" ), tr( "&Light Table" ), QString::null, parent());
	connect(m_aLightTable, SIGNAL(activated()), m_drawArea, SLOT(slotLightTable()));
	m_aLightTable->setStatusTip(tr("Activates or deactivates the light table" ));
	
	m_aClose = new QAction(QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/close.xpm" ), tr( "Cl&ose" ), tr("Ctrl+Shift+W"), parent());
	connect(m_aClose, SIGNAL(activated()), this, SLOT(close()));
	m_aClose->setStatusTip(tr("Closes the active document"));
	
	viewPreviousGroup = new QActionGroup( this );
	viewPreviousGroup->setExclusive( true );
	
	a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/no_previous.xpm" ), tr( "No Previous" ), Key_1, viewPreviousGroup);
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotNoPreviousOnionSkin()));
	a->setToggleAction ( true );
	a->setStatusTip(tr("Disables previous onion skin visualization"));
	
	a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/previous.xpm" ), tr( "Previous One" ), Key_2, viewPreviousGroup);
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotPreviousOnionSkin()));
	a->setStatusTip(tr("Shows the previous onion skin" ));
	a->setToggleAction ( true );
	
	a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/previous2.xpm" ), tr( "Previous Two" ), Key_3, viewPreviousGroup);
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotPrevious2OnionSkin()));
	a->setStatusTip(tr("Shows the previous 2 onion skins" ));
	a->setToggleAction ( true );
	
	a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/previous3.xpm" ), tr( "Previous Three" ), Key_4, viewPreviousGroup);
	a->setToggleAction ( true );
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotPrevious3OnionSkin()));
	a->setStatusTip(tr("Shows the previous 3 onion skins" ));
	
	a->setOn( true );
	
	viewNextGroup = new QActionGroup( this );
	viewNextGroup->setExclusive( true );
	
	a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/no_next.xpm" ), tr( "No Next" ), CTRL+Key_1, viewNextGroup);
	a->setToggleAction ( true );
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotNoNextOnionSkin()));
	a->setStatusTip(tr("Disables next onion skin visualization" ));
	
	a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/next.xpm" ), tr( "Next One" ), CTRL+Key_2, viewNextGroup);
	a->setToggleAction ( true );
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotNextOnionSkin()));
	a->setStatusTip(tr("Shows the next onion skin"));
	
	a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/next2.xpm" ), tr( "Next Two" ), CTRL+Key_3, viewNextGroup);
	a->setToggleAction(true );
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotNext2OnionSkin()));
	a->setStatusTip(tr("Shows the next 2 onion skins"));
	
	a = new QAction( QPixmap::fromMimeSource(KTOON_HOME+"/images/icons/next3.xpm" ), tr( "Next Three" ), CTRL+Key_4, viewNextGroup);
	a->setToggleAction(true );
	connect(a, SIGNAL(activated()), m_drawArea, SLOT(slotNext3OnionSkin()));
	a->setStatusTip(tr("Shows the next 3 onion skins"));
	a->setOn( true );
	
}

void KTViewDocument::createToolbar()
{
	m_barGrid = new QToolBar(this);
	gridGroup->addTo(m_barGrid);
	m_aSubGrid->addTo(m_barGrid);
	m_aFrontBackGrid->addTo(m_barGrid);
	m_barGrid->addSeparator();
	m_aUndo->addTo(m_barGrid);
	m_aRedo->addTo(m_barGrid);
	m_barGrid->addSeparator();
	editGroup->addTo(m_barGrid);
	m_barGrid->addSeparator();
	m_aNtsc->addTo(m_barGrid);
	m_aLightTable->addTo(m_barGrid);
	m_barGrid->addSeparator();
	viewPreviousGroup->addTo(m_barGrid);
	m_barGrid->addSeparator();
	viewNextGroup->addTo(m_barGrid);

	
}

void KTViewDocument::createMenu()
{
	//FIXME:crear el menu
	m_menuFile = new QPopupMenu( this );
	menuBar()->insertItem( tr("&File"), m_menuFile );
	m_aClose->addTo(m_menuFile);
	
	QPopupMenu *menuEdit= new QPopupMenu(this);
	menuBar()->insertItem( tr("&Edit"), menuEdit );
	editGroup->addTo(menuEdit);
	menuEdit->insertSeparator();
	editGroup2->addTo(menuEdit);
	
	m_menuGrid = new QPopupMenu( this );
	
	menuBar()->insertItem( tr("&view"), m_menuGrid );
	gridGroup->addTo(m_menuGrid);
	m_aSubGrid->addTo(m_menuGrid);
	

}

QSize  KTViewDocument::sizeHint () const
{
	const QSize hint(641, 481);
	return hint;
}

void KTViewDocument::close()
{
	m_drawArea->close();
}

void KTViewDocument::setCursor(QCursor c)
{
	m_drawArea->setCursor(c);
}
