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

#include <QLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QPixmap>
#include <QActionGroup>

#include "ktvhbox.h"

// TODO: DELETE THIS
#include "status.h"
#include "drawingarea.h"

KTViewDocument::KTViewDocument(KTScene *scene, QWidget *parent ) : KTMdiWindow(parent), m_scene(scene)
{
	setWindowIcon(QPixmap(KTOON_HOME+"/images/icons/layer_pic.xpm") ); // FIXME: new image for documents
	
	
	
	m_paintAreaContainer = new KTPaintAreaContainer(this);
	
	setCentralWidget ( m_paintAreaContainer );
	
	showPos(QPoint(0,0));
	
	connect( m_paintAreaContainer->drawArea(), SIGNAL(mousePos(const QPoint &)),  this,  SLOT(showPos(const QPoint &)) );
	
	
	m_paintAreaContainer->drawArea()->setLayer( m_scene->layers()[0] );
	
	createActions();
	createToolbar();
	createTools();
	createMenu();
	
	/**************************************/
	// FIXME: delete this
// 	DrawingArea *area  = new DrawingArea(0);
// 	area->hide();
// 	KTStatus->setDrawingArea(area);
	/**************************************/
	loadPlugins();
}


KTViewDocument::~KTViewDocument()
{
}

void KTViewDocument::showPos(const QPoint &p)
{
	QString messages =  "x: " +  QString::number(p.x()) +  " y: " + QString::number(p.y());
	statusBar()->showMessage ( messages ) ;
}

void KTViewDocument::createActions()
{
	gridGroup = new QActionGroup( parent());
	gridGroup->setExclusive( true );
	QAction *a = new QAction( QPixmap(KTOON_HOME+"/images/icons/nogrid.xpm" ), tr( "&No Grid" ), this);
	gridGroup->addAction ( a );
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotNoGrid()));

	a->setCheckable ( true );
	a->setStatusTip(tr("Hides the grid" ));
	a = new QAction(QPixmap(KTOON_HOME+"/images/icons/grid12.xpm" ),  tr( "&12 Field Grid" ), this);
	a->setCheckable ( true );
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotSeeGrid12()));
	a->setStatusTip(tr("Shows a 12 field grid" ));
	gridGroup->addAction ( a );

	a = new QAction( QPixmap(KTOON_HOME+"/images/icons/grid16.xpm" ), tr( "&16 Field Grid" ), this);
	a->setCheckable ( true );
	gridGroup->addAction ( a );
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotSeeGrid16()));
	a->setStatusTip(tr("Shows a 16 field grid" ));
	a->setChecked( true);
	
	m_aSubGrid = new QAction( QPixmap(KTOON_HOME+"/images/icons/subgrid.xpm" ), tr( "&Subgrid" ), this);
	m_aSubGrid->setCheckable ( true );
	connect(m_aSubGrid, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotSeeSubgrid()));
	m_aSubGrid->setStatusTip(tr("Shows or hides a 3 field subgrid in the current grid" ));
	
	m_aFrontBackGrid = new QAction( QPixmap(KTOON_HOME+"/images/icons/front_back_grid.xpm"), tr( "Grid to Front/Back" ), this);
	m_aFrontBackGrid->setCheckable ( true );
	m_aFrontBackGrid->setChecked(true);
	connect(m_aFrontBackGrid, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotFrontBackGrid()));
	m_aFrontBackGrid->setStatusTip(tr("Sends the grid to the front or to the back of the drawing area" ));
	
	m_aUndo = new QAction( QPixmap(KTOON_HOME+"/images/icons/undo.xpm" ), tr( "Undo" ), parent());
	m_aUndo->setShortcut(tr("Ctrl+Z"));
	
	connect(m_aUndo, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(undo()));
	m_aUndo->setStatusTip(tr("Undoes the last draw action"));
	
	m_aRedo = new QAction( QPixmap(KTOON_HOME+"/images/icons/redo.xpm" ), tr( "Redo" ),  parent());
	m_aRedo->setShortcut(tr("CTRL+SHIFT+Z"));
	connect(m_aRedo, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(redo()));
	m_aRedo->setStatusTip(tr("Redoes a previous undone action"));
	
	
	editGroup = new QActionGroup( parent() );
	a = new QAction( QPixmap(KTOON_HOME+"/images/icons/cut.xpm" ), tr( "&Cut" ),  editGroup);
	a->setShortcut(tr("Ctrl+X"));
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotCut()));
	a->setStatusTip(tr("Cuts the selection and puts it onto the clipboard"));
	
	a = new QAction( QPixmap(KTOON_HOME+"/images/icons/copy.xpm" ), tr( "C&opy" ),  editGroup);
	a->setShortcut(tr("Ctrl+C"));
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotCopy()));
	a->setStatusTip(tr("Copies the selection and puts it onto the clipboard"));
	
	a = new QAction( QPixmap(KTOON_HOME+"/images/icons/paste.xpm" ), tr( "&Paste" ),  editGroup);
	a->setShortcut(tr("Ctrl+V"));
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotPaste()));
	a->setStatusTip(tr("Pastes the clipboard into the current document"));
	
	
	editGroup2 = new QActionGroup( parent() );
	a = new QAction( tr(  "Paste &In Place" ), editGroup2);
	a->setShortcut(tr("Ctrl+Shift+V"));
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotPasteInPlace()));
	a->setStatusTip(tr("Pastes the clipboard into the same place as the copy was did"));
	
	a = new QAction( tr(  "&Delete" ),  editGroup2);
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotDelete()));
	a->setStatusTip(tr("Deletes the selected object"));
	
	a = new QAction( tr(  "&Select All" ),  editGroup2);
	a->setShortcut( tr("Ctrl+A"));
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotSelectAll()));
	a->setStatusTip(tr("Selects all objects in the document"));
	
	m_aNtsc = new QAction( QPixmap(KTOON_HOME+"/images/icons/ntsc.xpm" ), tr( "&NTSC Zone" ), parent());
	
	m_aNtsc->setCheckable ( true );
	connect(m_aNtsc, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotSeeNTSC()));
	m_aNtsc->setStatusTip(tr("Shows or hides the NTSC Zone" ));
	
	m_aLightTable = new QAction( QPixmap(KTOON_HOME+"/images/icons/light_table.xpm" ), tr( "&Light Table" ),  parent());
	connect(m_aLightTable, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotLightTable()));
	m_aLightTable->setStatusTip(tr("Activates or deactivates the light table" ));
	
	m_aClose = new QAction(QPixmap(KTOON_HOME+"/images/icons/close.xpm" ), tr( "Cl&ose" ), parent());
	m_aClose->setShortcut( tr("Ctrl+Shift+W"));
	connect(m_aClose, SIGNAL(triggered()), this, SLOT(close()));
	m_aClose->setStatusTip(tr("Closes the active document"));
	
	viewPreviousGroup = new QActionGroup( this );
	viewPreviousGroup->setExclusive( true );
	
	a = new QAction( QPixmap(KTOON_HOME+"/images/icons/no_previous.xpm" ), tr( "No Previous" ),  viewPreviousGroup);
	a->setShortcut( Qt::Key_1);
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotNoPreviousOnionSkin()));
	a->setCheckable ( true );
	a->setStatusTip(tr("Disables previous onion skin visualization"));
	
	a = new QAction( QPixmap(KTOON_HOME+"/images/icons/previous.xpm" ), tr( "Previous One" ), viewPreviousGroup);
	a->setShortcut( Qt::Key_2);
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotPreviousOnionSkin()));
	a->setStatusTip(tr("Shows the previous onion skin" ));
	a->setCheckable ( true );
	
	a = new QAction( QPixmap(KTOON_HOME+"/images/icons/previous2.xpm" ), tr( "Previous Two" ),  viewPreviousGroup);
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotPrevious2OnionSkin()));
	a->setStatusTip(tr("Shows the previous 2 onion skins" ));
	a->setCheckable ( true );
	a->setShortcut( Qt::Key_3);
	
	a = new QAction( QPixmap(KTOON_HOME+"/images/icons/previous3.xpm" ), tr( "Previous Three" ),  viewPreviousGroup);
	a->setCheckable ( true );
	a->setShortcut( Qt::Key_4);
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotPrevious3OnionSkin()));
	a->setStatusTip(tr("Shows the previous 3 onion skins" ));
	
	a->setChecked( true );
	
	viewNextGroup = new QActionGroup( this );
	viewNextGroup->setExclusive( true );
	
	a = new QAction( QPixmap(KTOON_HOME+"/images/icons/no_next.xpm" ), tr( "No Next" ),  viewNextGroup);
	a->setShortcut( Qt::CTRL+Qt::Key_1);
	a->setCheckable ( true );
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotNoNextOnionSkin()));
	a->setStatusTip(tr("Disables next onion skin visualization" ));
	
	a = new QAction( QPixmap(KTOON_HOME+"/images/icons/next.xpm" ), tr( "Next One" ),  viewNextGroup);
	a->setShortcut( Qt::CTRL+Qt::Key_2);
	a->setCheckable ( true );
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotNextOnionSkin()));
	a->setStatusTip(tr("Shows the next onion skin"));
	
	a = new QAction( QPixmap(KTOON_HOME+"/images/icons/next2.xpm" ), tr( "Next Two" ),  viewNextGroup);
	a->setShortcut( Qt::CTRL+Qt::Key_3);
	a->setToggleAction(true );
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotNext2OnionSkin()));
	a->setStatusTip(tr("Shows the next 2 onion skins"));
	
	a = new QAction( QPixmap(KTOON_HOME+"/images/icons/next3.xpm" ), tr( "Next Three" ),  viewNextGroup);
	a->setShortcut( Qt::CTRL+Qt::Key_4);
	a->setToggleAction(true );
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotNext3OnionSkin()));
	a->setStatusTip(tr("Shows the next 3 onion skins"));
	a->setChecked( true );
}


void KTViewDocument::createTools()
{
	m_toolbar = new QToolBar(tr("Toolbar"), this);
	m_toolbar->setIconSize( QSize(22,22) );
	addToolBar ( Qt::LeftToolBarArea, m_toolbar );
	
	// Brushes menu
	m_brushesMenu = new QMenu(tr("Brushes"), m_toolbar);
	m_brushesMenu->setIcon( QPixmap(KTOON_HOME+"/images/icons/brush.xpm") );
	connect( m_brushesMenu, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	
	m_toolbar->addAction(m_brushesMenu->menuAction());
	
	// Selection menu
	
	m_toolsSelection = new QMenu( tr("selection"), m_toolbar );
	m_toolsSelection->setIcon(QPixmap(KTOON_HOME+"/images/icons/selection.xpm"));
	connect( m_toolsSelection, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	
	 m_toolsSelection->addAction(QPixmap(KTOON_HOME+"/images/icons/selection.xpm"), tr( "Normal &Selection" ), m_paintAreaContainer->drawArea(), SLOT( slotNormalSelection()),tr("S"));

	
	m_toolsSelection->addAction(QPixmap(KTOON_HOME+"/images/icons/nodes.xpm"), tr( "Con&tour Selection" ), m_paintAreaContainer->drawArea(), SLOT( slotContourSelection()), tr("T") );
	
	m_toolsDraw = new QMenu( m_toolbar );
	connect( m_toolsDraw, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	
	m_toolsDraw->setIcon(QPixmap(KTOON_HOME+"/images/icons/brush.xpm"));
	m_toolsDraw->addAction(QPixmap(KTOON_HOME+"/images/icons/brush.xpm"), tr( "&Brush" ), m_paintAreaContainer->drawArea(), SLOT(slotBrush()), tr("B"));
	
	m_toolsDraw->addAction(QPixmap(KTOON_HOME+"/images/icons/pencil.xpm"), tr( "&Pencil" ), m_paintAreaContainer->drawArea(), SLOT( slotPencil()), tr("P"));
	
	m_toolsDraw->addAction(QPixmap(KTOON_HOME+"/images/icons/bezier.xpm"), tr( "&Pen" ), m_paintAreaContainer->drawArea(), SLOT( slotPen()), tr("N"));
	
	m_toolsDraw->addAction(QPixmap(KTOON_HOME+"/images/icons/line.xpm"), tr( "&line" ), m_paintAreaContainer->drawArea(), SLOT( slotLine()),tr("L"));
	
	m_toolsDraw->addAction(QPixmap(KTOON_HOME+"/images/icons/square.xpm"), tr( "&Rectangle" ), m_paintAreaContainer->drawArea(), SLOT( slotRectangle()), tr("R"));
	
	m_toolsDraw->addAction(QPixmap(KTOON_HOME+"/images/icons/ellipse.xpm"), tr( "&Ellipse" ), m_paintAreaContainer->drawArea(), SLOT(slotEllipse()),  tr("E"));
	
	m_toolsFills = new QMenu( "fills", m_toolbar );
	m_toolsFills->setIcon(QPixmap(KTOON_HOME+"/images/icons/fill.xpm"));
	connect( m_toolsFills, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	
	m_toolsFills->addAction(QPixmap(KTOON_HOME+"/images/icons/fill.xpm"), tr( "&Fill" ), m_paintAreaContainer->drawArea(), SLOT( slotFill()), tr("F"));
	
	m_toolsFills->addAction(QPixmap(KTOON_HOME+"/images/icons/removefill.xpm"), tr( "&Remove Fill" ), m_paintAreaContainer->drawArea(), SLOT( slotRemoveFill()), tr("Shift+F"));
	
	 m_toolsFills->addAction(QPixmap(KTOON_HOME+"/images/icons/contour.xpm"), tr( "&Contour Fill" ), m_paintAreaContainer->drawArea(), SLOT( slotContourFill()), tr("Ctrl+F"));
 
	 m_toolsFills->addAction(QPixmap(KTOON_HOME+"/images/icons/dropper.xpm"), tr( "&Dropper"), m_paintAreaContainer->drawArea(), SLOT(slotDropper()), tr("D"));

	m_toolsErasers = new QMenu(tr( "Eraser" ), m_toolbar );
	m_toolsErasers->setIcon(QPixmap(KTOON_HOME+"/images/icons/eraser.xpm"));
	connect( m_toolsErasers, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	
	m_toolsErasers->addAction(QPixmap(KTOON_HOME+"/images/icons/eraser.xpm"), tr( "&Eraser"), m_paintAreaContainer->drawArea(), SLOT( slotEraser()), Qt::SHIFT+Qt::Key_Delete);
	
	m_toolsErasers->addAction(QPixmap(KTOON_HOME+"/images/icons/dropper.xpm"), tr( "&Slicer" ), m_paintAreaContainer->drawArea(), SLOT( slotSlicer()),  Qt::CTRL+Qt::Key_Delete);
	
	m_toolsView = new QMenu(tr( "View" ), m_toolbar );
	m_toolsView->setIcon(QPixmap(KTOON_HOME+"/images/icons/magnifying.xpm"));
	connect( m_toolsView, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	
	m_toolsView->addAction(QPixmap(KTOON_HOME+"/images/icons/magnifying.xpm"), tr("&Magnifying Glass" ), m_paintAreaContainer->drawArea(), SLOT( slotMagnifyingGlass()), tr("M"));
	
	m_toolsView->addAction(QPixmap(KTOON_HOME+"/images/icons/hand.xpm"), tr( "&Hand" ), m_paintAreaContainer->drawArea(),  SLOT( slotHand()), tr("H"));
	
	m_toolsOrder = new QMenu(tr("Order"), m_toolbar);
	connect( m_toolsOrder, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	m_toolsOrder->setIcon(QPixmap(KTOON_HOME+"/images/icons/group.xpm"));
	m_toolsOrder->addAction(QPixmap(KTOON_HOME+"/images/icons/group.xpm"), tr( "&Group" ), m_paintAreaContainer->drawArea(), SLOT( slotGroup()));
	m_toolsOrder->addAction(QPixmap(KTOON_HOME+"/images/icons/ungroup.xpm"), tr( "&Ungroup" ), m_paintAreaContainer->drawArea(), SLOT( slotUngroup()));
	

// 	a->setStatusTip(tr("Group the selected objects into a single one"));
// 	a->setStatusTip(tr("Ungroups the selected object"));
	
	m_toolsOrder->addAction(QPixmap(KTOON_HOME+"/images/icons/bring_to_front.xpm"), tr( "&Bring to Front" ), m_paintAreaContainer->drawArea(), SLOT(slotBringToFront()), Qt::CTRL+Qt::SHIFT+Qt::Key_Up);
	m_toolsOrder->addAction(QPixmap(KTOON_HOME+"/images/icons/send_to_back.xpm"),  tr( "&Send to Back" ), m_paintAreaContainer->drawArea(), SLOT( slotSendToBack()), Qt::CTRL+Qt::SHIFT+Qt::Key_Down);

	m_toolsOrder->addAction(QPixmap(KTOON_HOME+"/images/icons/one_forward.xpm"), tr( "One Step &Forward" ), m_paintAreaContainer->drawArea(), SLOT( slotOneStepForward()), Qt::CTRL+Qt::Key_Up);
	
	m_toolsOrder->addAction(QPixmap(KTOON_HOME+"/images/icons/one_backward.xpm"), tr( "One Step B&ackward" ), m_paintAreaContainer->drawArea(), SLOT(slotOneStepBackward()));
// 	tools_bring_front->setStatusTip(tr("Brings the selected object to the front"));
// 	tools_send_back->setStatusTip(tr("Sends the selected objects to the back"));
// 	tools_one_step_forward->setStatusTip(tr("Moves the selected object one step forward"));
// 	tools_one_step_backward->setStatusTip(tr("Moves the selected object one step backward"));

	m_toolsAlign = new QMenu(tr( "Align"), this );
	connect( m_toolsAlign, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	m_toolsAlign->setIcon(QPixmap(KTOON_HOME+"/images/icons/align_l.xpm"));
	
	m_toolsAlign->addAction( QPixmap(KTOON_HOME+"/images/icons/align_l.xpm"), tr("&Left" ), m_paintAreaContainer->drawArea(), SLOT( slotAlignLeft()));
	
	m_toolsAlign->addAction( QPixmap(KTOON_HOME+"/images/icons/align_cv.xpm"), tr( "&Center Vertically" ),  m_paintAreaContainer->drawArea(), SLOT(slotCenterVertically()));
	
	m_toolsAlign->addAction( QPixmap(KTOON_HOME+"/images/icons/align_r.xpm"), tr("&Right" ), m_paintAreaContainer->drawArea(), SLOT(slotAlignRight()));
	m_toolsAlign->addSeparator();
	m_toolsAlign->addAction( QPixmap(KTOON_HOME+"/images/icons/align_t.xpm"), tr( "&Top" ), m_paintAreaContainer->drawArea(), SLOT(slotAlignTop()));
	m_toolsAlign->addAction(QPixmap(KTOON_HOME+"/images/icons/align_ch.xpm"), tr("Center &Horizontally" ), m_paintAreaContainer->drawArea(),  SLOT( slotCenterHorizontally()));
	m_toolsAlign->addAction(QPixmap(KTOON_HOME+"/images/icons/align_b.xpm"), tr( "&Bottom" ), m_paintAreaContainer->drawArea(), SLOT( slotAlignBottom()));
	
// 	tools_left->setStatusTip(tr("Aligns the selected object to the left"));
// 	tools_center_vertically->setStatusTip(tr("Centers vertically the selected object"));
// 	tools_right->setStatusTip(tr("Aligns the selected object to the right"));
// 	tools_top->setStatusTip(tr("Aligns the selected object to the top"));
// 	tools_center_horizontally->setStatusTip(tr("Centers horizontally the selected object"));
// 	tools_bottom->setStatusTip(tr("Aligns the selected object to the bottom"));

	m_toolsTransform = new QMenu( tr( "Transform " ), this);
	connect( m_toolsTransform, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
// 	m_toolsTransform->setIcon(QPixmap(KTOON_HOME+"/images/icons/align_l.xpm"));

	m_toolsTransform->addAction(tr( "Flip &Horizontally" ), m_paintAreaContainer->drawArea(), SLOT(slotFlipHorizontally()));
	m_toolsTransform->addAction(tr( "Flip &Vertically" ), m_paintAreaContainer->drawArea(), SLOT(slotFlipVertically()));
	m_toolsTransform->addSeparator();
	m_toolsTransform->addAction(tr( "&Rotate 90 CW" ), m_paintAreaContainer->drawArea(), SLOT( slotRotateCW90()));
	m_toolsTransform->addAction(tr( "&Rotate 90 CCW" ), m_paintAreaContainer->drawArea(), SLOT( slotRotateCCW90()));
	m_toolsTransform->addSeparator();
	m_toolsTransform->addAction(tr( "&Rotate &180" ),m_paintAreaContainer->drawArea(),SLOT( slotRotate180()));
	m_toolsTransform->addAction( QPixmap(KTOON_HOME+"/images/icons/perspective.xpm"),tr( "&Perspective" ) ,m_paintAreaContainer->drawArea(), SLOT( slotRotate180()));
	
	
// 	tools_flip_horizontally->setStatusTip(tr("Flips the selected object horizontally"));
// 	tools_flip_vertically->setStatusTip(tr("Flips the selected object vertically"));
// 	tools_rotate_cw90->setStatusTip(tr("Rotates the selected object 90 degrees clockwise"));
// 	tools_rotate_ccw90->setStatusTip(tr("Rotates the selected object 90 degrees counterclockwise"));
// 	tools_rotate180->setStatusTip(tr("Rotates the selected object 180 degrees"));
// 	tools_perspective->setStatusTip(tr("Activates the perspective tool"));
	
	m_toolbar->addAction(m_toolsSelection->menuAction());
	m_toolbar->addAction(m_toolsDraw->menuAction());
	m_toolbar->addAction(m_toolsFills->menuAction());
	m_toolbar->addAction(m_toolsErasers->menuAction());
	m_toolbar->addAction(m_toolsView->menuAction());
	m_toolbar->addAction(m_toolsOrder->menuAction());
	m_toolbar->addAction(m_toolsAlign->menuAction());
// 	m_toolbar->addAction(m_toolsTransform->menuAction());
}

void KTViewDocument::loadPlugins()
{
	m_pluginDirectory = QDir(KTOON_HOME+"/plugins/");

	foreach (QString fileName, m_pluginDirectory.entryList(QDir::Files))
	{
		QPluginLoader loader(m_pluginDirectory.absoluteFilePath(fileName));
		QObject *plugin = loader.instance();
		
		ktDebug() << "******FILE: " << fileName << endl;
		
		if (plugin)
		{
			AFilterInterface *aFilter = qobject_cast<AFilterInterface *>(plugin);
			AToolInterface *aTool = qobject_cast<AToolInterface *>(plugin);
			
			if ( aFilter )
			{
				QStringList::iterator it;
				QStringList keys = aFilter->keys();
				
								
				for (it = keys.begin(); it != keys.end(); ++it)
				{
					ktDebug() << "*******Filter Loaded: " << *it << endl;
					
					QAction *act = aFilter->actions()[*it];
					if ( act )
					{
						connect(act, SIGNAL(triggered()), this, SLOT(applyFilter()));
						m_filterMenu->addAction(act);
					}
				}
			}
			else if (aTool)
			{
				QStringList::iterator it;
				QStringList keys = aTool->keys();
				
				
				for (it = keys.begin(); it != keys.end(); ++it)
				{
					ktDebug() << "*******Tool Loaded: " << *it << endl;
					
					QAction *act = aTool->actions()[*it];
					if ( act )
					{
						connect(act, SIGNAL(triggered()), this, SLOT(selectTool()));
						m_brushesMenu->addAction(act);
						m_paintAreaContainer->drawArea()->setTool(aTool, *it);
					}
				}
			}
		}
		else
		{
			ktError() << tr("Not load, please try to rebuild from the scratch") << endl;
		}
	}
}

void KTViewDocument::selectTool()
{
	QAction *action = qobject_cast<QAction *>(sender());
	
	if ( action )
	{
		AToolInterface *aTool = qobject_cast<AToolInterface *>(action->parent());
		QString tool = action->text();
	
		m_paintAreaContainer->drawArea()->setTool(aTool, tool);
	}
}


void KTViewDocument::applyFilter()
{
	QAction *action = qobject_cast<QAction *>(sender());
	
	if ( action )
	{
		AFilterInterface *aFilter = qobject_cast<AFilterInterface *>(action->parent());
		QString filter = action->text();
		
		QImage image = aFilter->filter(action->text(), m_paintAreaContainer->drawArea()->paintDevice(), this);
		
		m_paintAreaContainer->drawArea()->setPaintDevice(image);
	}
}

void KTViewDocument::changeTool( QAction *a)
{
	QMenu *menuTmp = qobject_cast<QMenu*>(a->parentWidget());
	if(menuTmp)
	{
		menuTmp->setIcon(a->icon());
		menuTmp->setDefaultAction (  a );
		menuTmp->setActiveAction ( a );
	}
	else
	{
// 		ktDebug( 1) << "else" << endl;
	}
	
}

void KTViewDocument::createToolbar()
{
	m_barGrid = new QToolBar(tr("Bar Actions"), this);
	m_barGrid->setIconSize( QSize(22,22) );
	addToolBar(m_barGrid);
	m_barGrid->addActions(gridGroup->actions());
	m_barGrid->addAction(m_aSubGrid);
	m_barGrid->addAction(m_aFrontBackGrid);
	m_barGrid->addSeparator();
	m_barGrid->addAction(m_aUndo);
	m_barGrid->addAction(m_aRedo);
	m_barGrid->addSeparator();
	m_barGrid->addActions(editGroup->actions());
	m_barGrid->addSeparator();
	m_barGrid->addAction(m_aNtsc);
	m_barGrid->addAction(m_aLightTable);
	m_barGrid->addSeparator();
	m_barGrid->addActions(viewPreviousGroup->actions());
	m_barGrid->addSeparator();
	m_barGrid->addActions(viewNextGroup->actions());
	
}

void KTViewDocument::createMenu()
{
	m_filterMenu = new QMenu(this);
	menuBar()->insertItem(tr("&Filters"), m_filterMenu);
	
	//FIXME:crear el menu
// 	m_menuFile = new Q3PopupMenu( this );
// 	menuBar()->insertItem( tr("&File"), m_menuFile );
// 	m_aClose->addTo(m_menuFile);
// 	
// 	Q3PopupMenu *menuEdit= new Q3PopupMenu(this);
// 	menuBar()->insertItem( tr("&Edit"), menuEdit );
// 	editGroup->addTo(menuEdit);
// 	menuEdit->insertSeparator();
// 	editGroup2->addTo(menuEdit);
// 	
// 	m_menuGrid = new Q3PopupMenu( this );
// 	
// 	menuBar()->insertItem( tr("&view"), m_menuGrid );
// 	gridGroup->addTo(m_menuGrid);
// 	m_aSubGrid->addTo(m_menuGrid);
}

void KTViewDocument::close()
{
	m_paintAreaContainer->drawArea()->close();
}

APaintArea *KTViewDocument::drawArea()
{
	return m_paintAreaContainer->drawArea();
}

void KTViewDocument::setCursor(const QCursor &c)
{
	m_paintAreaContainer->drawArea()->setCursor(c);
}


