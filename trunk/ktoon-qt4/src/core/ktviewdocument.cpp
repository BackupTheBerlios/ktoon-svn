/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
#include "ddebug.h"

#include <QLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QPixmap>
#include <QActionGroup>
#include <QDockWidget>
#include <QTimer>

#include "dvhbox.h"
#include "kttoolpluginobject.h"

KTViewDocument::KTViewDocument(const QSize &size, const QString& projectName, const QString & renderType, KTDocument *doc, QWorkspace *parent ) : DMdiWindow(parent), m_document(doc), m_title(projectName)
{
	setWindowIcon(QPixmap(THEME_DIR+"/icons/layer_pic.png") ); // FIXME: new image for documents
	
	m_actionManager = new DActionManager(this);
	APaintArea::RenderType type;
	if(renderType == tr("Native"))
	{
		type = APaintArea::Native;
	}
	else if(renderType == tr("OpenGL"))
	{
		type = APaintArea::OpenGL;
	}
	else
	{
		type = APaintArea::Image;
	}
	
	m_paintAreaContainer = new KTPaintAreaContainer(size,type,  this);
	
	setCentralWidget ( m_paintAreaContainer );
	
	showPos(QPoint(0,0));
	
	connect( m_paintAreaContainer->drawArea(), SIGNAL(mousePos(const QPoint &)),  this,  SLOT(showPos(const QPoint &)) );
	
	setWindowTitle( m_title + " - " + m_document->currentScene()->sceneName() );
	
	m_paintAreaContainer->drawArea()->setScene( m_document->currentScene() );
	
	connect(m_document, SIGNAL(sceneChanged( KTScene* )) , this, SLOT(setScene( KTScene* )  )); 
	
	createActions();
	setupGridActions();
	setupEditActions();
	setupViewActions();
	setupOrderActions();
	createToolbar();
	createTools();
	createMenu();
	
	m_configurationArea = new KTConfigurationArea;
	addDockWidget(Qt::RightDockWidgetArea, m_configurationArea);
	
	
	QTimer::singleShot(0, this, SLOT(loadPlugins()));
}

KTViewDocument::~KTViewDocument()
{
}

void KTViewDocument::showPos(const QPoint &p)
{
	QString messages =  "X: " +  QString::number(p.x()- m_paintAreaContainer->drawAreaDelta().x() ) +  " Y: " + QString::number(p.y()- m_paintAreaContainer->drawAreaDelta().y() );
	statusBar()->showMessage ( messages ) ;
}

void KTViewDocument::createActions()
{
	DAction *undo = new DAction( QPixmap(THEME_DIR+"/icons/undo.png" ), tr( "&Undo" ),  QKeySequence(tr("Ctrl+Z")), m_paintAreaContainer->drawArea(), SLOT(undo()), m_actionManager, "undo" );
	undo->setStatusTip(tr("Undoes the last draw action"));
	undo->setShortcutContext ( Qt::ApplicationShortcut );
	

	DAction *redo = new DAction( QPixmap(THEME_DIR+"/icons/redo.png" ), tr( "&Redo" ),  QKeySequence(tr("Ctrl+SHIFT+Z")), m_paintAreaContainer->drawArea(), SLOT(redo()), m_actionManager, "redo" );
	redo->setShortcutContext ( Qt::ApplicationShortcut );
	redo->setStatusTip(tr("Redoes a previous undone action"));
	

	
}

void KTViewDocument::setupGridActions()
{
#if 0
	m_gridGroup = new QActionGroup( parent());
	gridGroup->setExclusive( true );
	QAction *a = new QAction( QPixmap(THEME_DIR+"/icons/nogrid.png" ), tr( "&No Grid" ), this);
	gridGroup->addAction ( a );
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotNoGrid()));

	a->setCheckable ( true );
	a->setStatusTip(tr("Hides the grid" ));
	a = new QAction(QPixmap(THEME_DIR+"/icons/grid12.png" ),  tr( "&12 Field Grid" ), this);
	a->setCheckable ( true );
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotSeeGrid12()));
	a->setStatusTip(tr("Shows a 12 field grid" ));
	gridGroup->addAction ( a );

	a = new QAction( QPixmap(THEME_DIR+"/icons/grid16.png" ), tr( "&16 Field Grid" ), this);
	a->setCheckable ( true );
	gridGroup->addAction ( a );
	connect(a, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotSeeGrid16()));
	a->setStatusTip(tr("Shows a 16 field grid" ));
	a->setChecked( true);
	
	m_aSubGrid = new QAction( QPixmap(THEME_DIR+"/icons/subgrid.png" ), tr( "&Subgrid" ), this);
	m_aSubGrid->setCheckable ( true );
	connect(m_aSubGrid, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotSeeSubgrid()));
	m_aSubGrid->setStatusTip(tr("Shows or hides a 3 field subgrid in the current grid" ));
	
	m_aFrontBackGrid = new QAction( QPixmap(THEME_DIR+"/icons/front_back_grid.png"), tr( "Grid to Front/Back" ), this);
	m_aFrontBackGrid->setCheckable ( true );
	m_aFrontBackGrid->setChecked(true);
	connect(m_aFrontBackGrid, SIGNAL(triggered()), m_paintAreaContainer->drawArea(), SLOT(slotFrontBackGrid()));
	m_aFrontBackGrid->setStatusTip(tr("Sends the grid to the front or to the back of the drawing area" ));
#endif
}

void KTViewDocument::setupEditActions()
{
	
	m_editGroup = new QActionGroup( parent() );
	DAction *a = new DAction( QPixmap(THEME_DIR+"/icons/cut.png" ), tr( "&Cut" ),  QKeySequence(tr("Ctrl+X")), m_paintAreaContainer->drawArea(), SLOT(cut()),m_actionManager, "cut" );
	a->setShortcutContext ( Qt::ApplicationShortcut );
	m_editGroup->addAction(a);

	
	a = new DAction( QPixmap(THEME_DIR+"/icons/copy.png" ), tr( "C&opy" ),  QKeySequence(tr("Ctrl+C")), m_paintAreaContainer->drawArea(), SLOT(copy()), m_actionManager, "copy");
	
	a->setShortcutContext ( Qt::ApplicationShortcut );
	m_editGroup->addAction(a);
	
	a->setStatusTip(tr("Copies the selection and puts it onto the clipboard"));
	
	a = new DAction( QPixmap(THEME_DIR+"/icons/paste.png" ), tr( "&Paste" ),   QKeySequence(tr("Ctrl+V")), m_paintAreaContainer->drawArea(), SLOT(paste()), m_actionManager, "paste");
	
	a->setShortcutContext ( Qt::ApplicationShortcut );
	m_editGroup->addAction(a);
	a->setStatusTip(tr("Pastes the clipboard into the current document"));
	
	a = new DAction( QPixmap(THEME_DIR+"/icons/delete.png" ), tr( "&Delete" ), QKeySequence( Qt::Key_Delete ), m_paintAreaContainer->drawArea(), SLOT(removeSelectsGraphics()), m_actionManager, "delete");
// 	m_editGroup->addAction(a);
	a->setShortcutContext ( Qt::ApplicationShortcut );
	a->setStatusTip(tr("Deletes the selected object"));
	
	
	a = new DAction( QPixmap(THEME_DIR+"/icons/group.png" ), tr( "&Group" ),   QKeySequence(  ), m_paintAreaContainer->drawArea(), SLOT(group()), m_actionManager, "group");
	m_editGroup->addAction(a);
	a->setStatusTip(tr("Group the selected objects into a single one"));
	
	a = new DAction( QPixmap(THEME_DIR+"/icons/ungroup.png" ), tr( "&Ungroup" ),   QKeySequence(  ), m_paintAreaContainer->drawArea(), SLOT(ungroup()), m_actionManager, "ungroup");
	m_editGroup->addAction(a);
	a->setStatusTip(tr("Ungroups the selected object"));
	
	a = new DAction( QPixmap(THEME_DIR+"/icons/" ), tr( "Select &All" ),   QKeySequence(tr("Ctrl+A")), m_paintAreaContainer->drawArea(), SLOT(selectAll()), m_actionManager, "selectAll");
	a->setStatusTip(tr("selected all object"));
	
	a = new DAction( tr("Properties..."), QKeySequence(), this, SLOT(configure()), m_actionManager, "properties");
	a->setStatusTip(tr("Configure the paint area"));
}

void KTViewDocument::setupOrderActions()
{
	DAction *bringtoFront = new DAction(QPixmap(THEME_DIR+"/icons/bring_to_front.png" ), tr( "&Bring to Front" ),  QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_Up), m_paintAreaContainer->drawArea(), SLOT(bringToFromSelected()), m_actionManager, "bringToFront" );
	
	bringtoFront->setShortcutContext ( Qt::ApplicationShortcut );
	bringtoFront->setStatusTip(tr("Brings the selected object to the front"));
	
	
	DAction *sendToBack = new DAction(QPixmap(THEME_DIR+"/icons/send_to_back.png" ), tr( "&Send to Back" ),  QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_Down), m_paintAreaContainer->drawArea(), SLOT(sendToBackSelected()), m_actionManager, "sendToBack" );
	
	sendToBack->setShortcutContext ( Qt::ApplicationShortcut );
	sendToBack->setStatusTip(tr("Sends the selected objects to the back"));
	

	
	DAction *oneStepForward = new DAction(QPixmap(THEME_DIR+"/icons/one_forward.png" ), tr( "One Step &Forward" ),  QKeySequence(Qt::CTRL+Qt::Key_Up), m_paintAreaContainer->drawArea(), SLOT(oneStepForwardSelected()), m_actionManager, "oneStepForward" );
	
	oneStepForward->setShortcutContext ( Qt::ApplicationShortcut );
	oneStepForward->setStatusTip(tr("Moves the selected object one step forward"));
	
	
	DAction *oneStepBackward = new DAction(QPixmap(THEME_DIR+"/icons/one_backward.png" ), tr( "One Step B&ackward" ),  QKeySequence(Qt::CTRL+Qt::Key_Down), m_paintAreaContainer->drawArea(), SLOT(oneStepBackwardSelected()), m_actionManager, "oneStepBackward" );
	
	oneStepBackward->setShortcutContext ( Qt::ApplicationShortcut );
	oneStepBackward->setStatusTip(tr("Moves the selected object one step backward"));
	
}

void KTViewDocument::setupViewActions()
{
	m_viewPreviousGroup = new QActionGroup( this );
	m_viewPreviousGroup->setExclusive( true );
	
	DAction *noPrevious = new DAction( QPixmap(THEME_DIR+"/icons/no_previous.png" ), tr( "No Previous" ), QKeySequence(Qt::Key_1), this, SLOT(disablePreviousOnionSkin()), m_actionManager, "no_previous" );
	
	m_viewPreviousGroup->addAction(noPrevious);
	
	noPrevious->setCheckable ( true );
	noPrevious->setStatusTip(tr("Disables previous onion skin visualization"));
	
	noPrevious->setChecked(true);
	
	DAction *onePrevious = new DAction( QPixmap(THEME_DIR+"/icons/previous.png" ), tr( "Previous One" ), QKeySequence(Qt::Key_2), this, SLOT(onePreviousOnionSkin()), m_actionManager, "previews_one");
	
	m_viewPreviousGroup->addAction(onePrevious);
	
	onePrevious->setStatusTip(tr("Shows the previous onion skin" ));
	onePrevious->setCheckable ( true );
	
	DAction *twoPrevious = new DAction( QPixmap(THEME_DIR+"/icons/previous2.png" ), tr( "Previous Two" ), QKeySequence(Qt::Key_3), this, SLOT(twoPreviousOnionSkin()), m_actionManager, "previews_two");
	m_viewPreviousGroup->addAction(twoPrevious);
	twoPrevious->setStatusTip(tr("Shows the previous 2 onion skins" ));
	twoPrevious->setCheckable ( true );
	
	DAction *threePrevious = new DAction( QPixmap(THEME_DIR+"/icons/previous3.png" ), tr( "Previous Three" ), QKeySequence(Qt::Key_4), this, SLOT(threePreviousOnionSkin()), m_actionManager, "previews_three");
	m_viewPreviousGroup->addAction(threePrevious);
	threePrevious->setCheckable ( true );
	threePrevious->setStatusTip(tr("Shows the previous 3 onion skins" ));

// 	// NEXT 

	m_viewNextGroup = new QActionGroup( this );
	m_viewNextGroup->setExclusive( true );
	
	DAction *noNext = new DAction( QPixmap(THEME_DIR+"/icons/no_next.png" ), tr( "No Next" ), QKeySequence(Qt::CTRL+Qt::Key_1), this, SLOT(disableNextOnionSkin()), m_actionManager, "no_next");
	m_viewNextGroup->addAction(noNext);
	
	
	noNext->setCheckable ( true );
	noNext->setStatusTip(tr("Disables next onion skin visualization" ));
	
	DAction *oneNext = new DAction( QPixmap(THEME_DIR+"/icons/next.png" ), tr( "Next One" ), QKeySequence(Qt::CTRL+Qt::Key_2), this, SLOT(oneNextOnionSkin()), m_actionManager, "next_one");
	m_viewNextGroup->addAction(oneNext);
	
	oneNext->setCheckable ( true );
	oneNext->setStatusTip(tr("Shows the next onion skin"));
	
	DAction *twoNext = new DAction( QPixmap(THEME_DIR+"/icons/next2.png" ), tr( "Next Two" ), QKeySequence(Qt::CTRL+Qt::Key_3), this, SLOT(twoNextOnionSkin()), m_actionManager, "next_two");
	m_viewNextGroup->addAction(twoNext);
	
	twoNext->setCheckable( true );
	twoNext->setStatusTip(tr("Shows the next 2 onion skins"));
	
	DAction *threeNext = new DAction( QPixmap(THEME_DIR+"/icons/next3.png" ), tr( "Next Three" ), QKeySequence(Qt::CTRL+Qt::Key_4), this, SLOT(threeNextOnionSkin()), m_actionManager, "next_three");
	m_viewNextGroup->addAction(threeNext);
	
	threeNext->setCheckable(true );
	threeNext->setStatusTip(tr("Shows the next 3 onion skins"));
}


void KTViewDocument::createTools()
{
	m_toolbar = new QToolBar(tr("Toolbar"), this);
	m_toolbar->setIconSize( QSize(22,22) );
	addToolBar ( Qt::LeftToolBarArea, m_toolbar );
	
	connect(m_toolbar, SIGNAL(actionTriggered(QAction *)), this, SLOT(selectToolFromMenu(QAction *)));
	
	// Brushes menu
	m_brushesMenu = new QMenu(tr("Brushes"), m_toolbar);
	m_brushesMenu->setIcon( QPixmap(THEME_DIR+"/icons/brush.png") );
	connect( m_brushesMenu, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	
// 	connect(m_brushesMenu->menuAction(), SIGNAL(triggered()), this, SLOT(selectToolFromMenu()));
	
	m_toolbar->addAction(m_brushesMenu->menuAction());
	
	// Selection menu
	
	m_selectionMenu = new QMenu( tr("selection"), m_toolbar );
	m_selectionMenu->setIcon(QPixmap(THEME_DIR+"/icons/selection.png"));
	connect( m_selectionMenu, SIGNAL(triggered ()), this, SLOT(selectTool()));
	
	m_toolbar->addAction(m_selectionMenu->menuAction());
	
	// Fill menu
	m_fillMenu = new QMenu(tr("Fill"), m_toolbar);
	m_fillMenu->setIcon(QPixmap(THEME_DIR+"/icons/fill.png"));
	connect(m_fillMenu, SIGNAL(triggered(QAction *)), this, SLOT(changeTool( QAction* )));
	
	m_toolbar->addAction(m_fillMenu->menuAction());
	
#if 0
	m_toolsSelection->addAction(QPixmap(THEME_DIR+"/icons/nodes.png"), tr( "Con&tour Selection" ), m_paintAreaContainer->drawArea(), SLOT( slotContourSelection()), tr("T") );
	
	m_toolsDraw = new QMenu( m_toolbar );
	connect( m_toolsDraw, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	
	m_toolsDraw->setIcon(QPixmap(THEME_DIR+"/icons/brush.png"));
	m_toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/brush.png"), tr( "&Brush" ), m_paintAreaContainer->drawArea(), SLOT(slotBrush()), tr("B"));
	
	m_toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/pencil.png"), tr( "&Pencil" ), m_paintAreaContainer->drawArea(), SLOT( slotPencil()), tr("P"));
	
	m_toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/bezier.png"), tr( "&Pen" ), m_paintAreaContainer->drawArea(), SLOT( slotPen()), tr("N"));
	
	m_toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/line.png"), tr( "&line" ), m_paintAreaContainer->drawArea(), SLOT( slotLine()),tr("L"));
	
	m_toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/square.png"), tr( "&Rectangle" ), m_paintAreaContainer->drawArea(), SLOT( slotRectangle()), tr("R"));
	
	m_toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/ellipse.png"), tr( "&Ellipse" ), m_paintAreaContainer->drawArea(), SLOT(slotEllipse()),  tr("E"));
	
	m_toolsFills = new QMenu( "fills", m_toolbar );
	m_toolsFills->setIcon(QPixmap(THEME_DIR+"/icons/fill.png"));
	connect( m_toolsFills, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	
	m_toolsFills->addAction(QPixmap(THEME_DIR+"/icons/fill.png"), tr( "&Fill" ), m_paintAreaContainer->drawArea(), SLOT( slotFill()), tr("F"));
	
	m_toolsFills->addAction(QPixmap(THEME_DIR+"/icons/removefill.png"), tr( "&Remove Fill" ), m_paintAreaContainer->drawArea(), SLOT( slotRemoveFill()), tr("Shift+F"));
	
	m_toolsFills->addAction(QPixmap(THEME_DIR+"/icons/contour.png"), tr( "&Contour Fill" ), m_paintAreaContainer->drawArea(), SLOT( slotContourFill()), tr("Ctrl+F"));
 
	m_toolsFills->addAction(QPixmap(THEME_DIR+"/icons/dropper.png"), tr( "&Dropper"), m_paintAreaContainer->drawArea(), SLOT(slotDropper()), tr("D"));

	m_toolsErasers = new QMenu(tr( "Eraser" ), m_toolbar );
	m_toolsErasers->setIcon(QPixmap(THEME_DIR+"/icons/eraser.png"));
	connect( m_toolsErasers, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	
	m_toolsErasers->addAction(QPixmap(THEME_DIR+"/icons/eraser.png"), tr( "&Eraser"), m_paintAreaContainer->drawArea(), SLOT( slotEraser()), Qt::SHIFT+Qt::Key_Delete);
	
	m_toolsErasers->addAction(QPixmap(THEME_DIR+"/icons/dropper.png"), tr( "&Slicer" ), m_paintAreaContainer->drawArea(), SLOT( slotSlicer()),  Qt::CTRL+Qt::Key_Delete);
	
	m_toolsView = new QMenu(tr( "View" ), m_toolbar );
	m_toolsView->setIcon(QPixmap(THEME_DIR+"/icons/magnifying.png"));
	connect( m_toolsView, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	
	m_toolsView->addAction(QPixmap(THEME_DIR+"/icons/magnifying.png"), tr("&Magnifying Glass" ), m_paintAreaContainer->drawArea(), SLOT( slotMagnifyingGlass()), tr("M"));
	
	m_toolsView->addAction(QPixmap(THEME_DIR+"/icons/hand.png"), tr( "&Hand" ), m_paintAreaContainer->drawArea(),  SLOT( slotHand()), tr("H"));
	
	m_toolsOrder = new QMenu(tr("Order"), m_toolbar);
	connect( m_toolsOrder, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	m_toolsOrder->setIcon(QPixmap(THEME_DIR+"/icons/group.png"));
	m_toolsOrder->addAction(QPixmap(THEME_DIR+"/icons/group.png"), tr( "&Group" ), m_paintAreaContainer->drawArea(), SLOT( slotGroup()));
	m_toolsOrder->addAction(QPixmap(THEME_DIR+"/icons/ungroup.png"), tr( "&Ungroup" ), m_paintAreaContainer->drawArea(), SLOT( slotUngroup()));
	

// 	a->setStatusTip(tr("Group the selected objects into a single one"));
// 	a->setStatusTip(tr("Ungroups the selected object"));
	
	

	m_toolsAlign = new QMenu(tr( "Align"), this );
	connect( m_toolsAlign, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	m_toolsAlign->setIcon(QPixmap(THEME_DIR+"/icons/align_l.png"));
	
	m_toolsAlign->addAction( QPixmap(THEME_DIR+"/icons/align_l.png"), tr("&Left" ), m_paintAreaContainer->drawArea(), SLOT( slotAlignLeft()));
	
	m_toolsAlign->addAction( QPixmap(THEME_DIR+"/icons/align_cv.png"), tr( "&Center Vertically" ),  m_paintAreaContainer->drawArea(), SLOT(slotCenterVertically()));
	
	m_toolsAlign->addAction( QPixmap(THEME_DIR+"/icons/align_r.png"), tr("&Right" ), m_paintAreaContainer->drawArea(), SLOT(slotAlignRight()));
	m_toolsAlign->addSeparator();
	m_toolsAlign->addAction( QPixmap(THEME_DIR+"/icons/align_t.png"), tr( "&Top" ), m_paintAreaContainer->drawArea(), SLOT(slotAlignTop()));
	m_toolsAlign->addAction(QPixmap(THEME_DIR+"/icons/align_ch.png"), tr("Center &Horizontally" ), m_paintAreaContainer->drawArea(),  SLOT( slotCenterHorizontally()));
	m_toolsAlign->addAction(QPixmap(THEME_DIR+"/icons/align_b.png"), tr( "&Bottom" ), m_paintAreaContainer->drawArea(), SLOT( slotAlignBottom()));
	
	tools_left->setStatusTip(tr("Aligns the selected object to the left"));
	tools_center_vertically->setStatusTip(tr("Centers vertically the selected object"));
	tools_right->setStatusTip(tr("Aligns the selected object to the right"));
	tools_top->setStatusTip(tr("Aligns the selected object to the top"));
	tools_center_horizontally->setStatusTip(tr("Centers horizontally the selected object"));
	tools_bottom->setStatusTip(tr("Aligns the selected object to the bottom"));

	m_toolsTransform = new QMenu( tr( "Transform " ), this);
	connect( m_toolsTransform, SIGNAL(triggered ( QAction * )), this, SLOT(changeTool( QAction*)));
	m_toolsTransform->setIcon(QPixmap(HOME+"/images/icons/align_l.png"));

	m_toolsTransform->addAction(tr( "Flip &Horizontally" ), m_paintAreaContainer->drawArea(), SLOT(slotFlipHorizontally()));
	m_toolsTransform->addAction(tr( "Flip &Vertically" ), m_paintAreaContainer->drawArea(), SLOT(slotFlipVertically()));
	m_toolsTransform->addSeparator();
	m_toolsTransform->addAction(tr( "&Rotate 90 CW" ), m_paintAreaContainer->drawArea(), SLOT( slotRotateCW90()));
	m_toolsTransform->addAction(tr( "&Rotate 90 CCW" ), m_paintAreaContainer->drawArea(), SLOT( slotRotateCCW90()));
	m_toolsTransform->addSeparator();
	m_toolsTransform->addAction(tr( "&Rotate &180" ),m_paintAreaContainer->drawArea(),SLOT( slotRotate180()));
	m_toolsTransform->addAction( QPixmap(THEME_DIR+"/icons/perspective.png"),tr( "&Perspective" ) ,m_paintAreaContainer->drawArea(), SLOT( slotRotate180()));
	
	
	tools_flip_horizontally->setStatusTip(tr("Flips the selected object horizontally"));
	tools_flip_vertically->setStatusTip(tr("Flips the selected object vertically"));
	tools_rotate_cw90->setStatusTip(tr("Rotates the selected object 90 degrees clockwise"));
	tools_rotate_ccw90->setStatusTip(tr("Rotates the selected object 90 degrees counterclockwise"));
	tools_rotate180->setStatusTip(tr("Rotates the selected object 180 degrees"));
	tools_perspective->setStatusTip(tr("Activates the perspective tool"));
	
	m_toolbar->addAction(m_toolsSelection->menuAction());
	m_toolbar->addAction(m_toolsDraw->menuAction());
	m_toolbar->addAction(m_toolsFills->menuAction());
	m_toolbar->addAction(m_toolsErasers->menuAction());
	m_toolbar->addAction(m_toolsView->menuAction());
	m_toolbar->addAction(m_toolsOrder->menuAction());
	m_toolbar->addAction(m_toolsAlign->menuAction());
	m_toolbar->addAction(m_toolsTransform->menuAction());
#endif
}

void KTViewDocument::loadPlugins()
{
	m_pluginDirectory = QDir(HOME+"/plugins/");

	foreach (QString fileName, m_pluginDirectory.entryList(QDir::Files))
	{
		QPluginLoader loader(m_pluginDirectory.absoluteFilePath(fileName));
		KTToolPluginObject *plugin = qobject_cast<KTToolPluginObject*>(loader.instance());
		
		dDebug() << "******FILE: " << fileName;
		
		if (plugin)
		{
// 			AFilterInterface *aFilter = qobject_cast<AFilterInterface *>(plugin);
			AToolInterface *aTool = qobject_cast<AToolInterface *>(plugin);
			
/*			if ( aFilter )
			{
				QStringList::iterator it;
				QStringList keys = aFilter->keys();
				
								
				for (it = keys.begin(); it != keys.end(); ++it)
				{
					dDebug() << "*******Filter Loaded: " << *it;
					
					QAction *act = aFilter->actions()[*it];
					if ( act )
					{
						connect(act, SIGNAL(triggered()), this, SLOT(applyFilter()));
						m_filterMenu->addAction(act);
					}
				}
			}
			else */
			if (aTool)
			{
				QStringList::iterator it;
				QStringList keys = aTool->keys();
				
				
				for (it = keys.begin(); it != keys.end(); ++it)
				{
					dDebug() << "*******Tool Loaded: " << *it;
					
					QAction *act = aTool->actions()[*it];
					if ( act )
					{
						connect(act, SIGNAL(triggered()), this, SLOT(selectTool()));
						
						switch( aTool->type() )
						{
							case AToolInterface::Brush:
							{
								m_brushesMenu->addAction(act);
								if ( !m_brushesMenu->activeAction() )
								{
									act->trigger();
								}
							}
							break;
							case AToolInterface::Selection:
							{
								m_selectionMenu->addAction(act);
								if ( !m_selectionMenu->activeAction() )
								{
									act->trigger();
								}
							}
							break;
							case AToolInterface::Fill:
							{
								m_fillMenu->addAction(act);
								if ( !m_fillMenu->activeAction() )
								{
									act->trigger();
								}
							}
							break;
							default:
							{
							}
							break;
						}
						m_paintAreaContainer->drawArea()->setTool(aTool, *it);
					}
				}				
				connect(plugin, SIGNAL(toDrawGhostGraphic(const QPainterPath &)), m_paintAreaContainer->drawArea(), SLOT(drawGhostGraphic(const QPainterPath &)));
				connect(plugin, SIGNAL(requestRedraw()), m_paintAreaContainer->drawArea(), SLOT(redrawAll()));
			}
		}
		else
		{
// 			dError() << tr("Not load, please try to rebuild from the scratch");
		}
	}
}

void KTViewDocument::selectTool()
{
	D_FUNCINFO;
	DAction *action = qobject_cast<DAction *>(sender());
	
	if ( action )
	{
		AToolInterface *aTool = qobject_cast<AToolInterface *>(action->parent());
		QString tool = action->text();

		switch(aTool->type())
		{
			case AToolInterface::Brush:
			{
				m_brushesMenu->setDefaultAction(action);
				m_brushesMenu->setActiveAction(action);
				if ( !action->icon().isNull() )
				{
					m_brushesMenu->menuAction()->setIcon(action->icon());
				}
			}
			break;
			case AToolInterface::Fill:
			{
				m_fillMenu->setDefaultAction(action);
				m_fillMenu->setActiveAction(action);
				if ( !action->icon().isNull() )
				{
					m_fillMenu->menuAction()->setIcon(action->icon());
				}
			}
			break;
			case AToolInterface::Selection:
			{
				m_selectionMenu->setDefaultAction(action);
				m_selectionMenu->setActiveAction(action);
				if ( !action->icon().isNull() )
				{
					m_selectionMenu->menuAction()->setIcon(action->icon());
				}
			}
			break;
		}
		
		QWidget *toolConfigurator = aTool->configurator();
		
		if ( toolConfigurator )
		{
			m_configurationArea->setConfigurator( toolConfigurator);
			toolConfigurator->show();
			
			if ( !m_configurationArea->isVisible() )
			{
				m_configurationArea->show();
			}
		}
		
		m_paintAreaContainer->drawArea()->setTool(aTool, tool);
		m_paintAreaContainer->drawArea()->setCursor(action->cursor());
	}
}

void KTViewDocument::selectToolFromMenu(QAction *action)
{
	D_FUNCINFO;
	
	QMenu *menu = qobject_cast<QMenu *>(action->parent());
	if (menu )
	{
		DAction *tool = qobject_cast<DAction *>(menu->activeAction());
		
		if ( tool )
		{
			tool->trigger(); // this call selectTool()
		}
	}
}


void KTViewDocument::applyFilter()
{
// 	QAction *action = qobject_cast<QAction *>(sender());
// 	
// 	if ( action )
// 	{
// 		AFilterInterface *aFilter = qobject_cast<AFilterInterface *>(action->parent());
// 		QString filter = action->text();
// 		
// 		QImage image = aFilter->filter(action->text(), m_paintAreaContainer->drawArea()->paintDevice(), this);
// 		
// 		m_paintAreaContainer->drawArea()->setPaintDevice(image);
// 	}
}

void KTViewDocument::createToolbar()
{
	m_barGrid = new QToolBar(tr("Bar Actions"), this);
	m_barGrid->setIconSize( QSize(22,22) );
	addToolBar(m_barGrid);
	m_barGrid->addSeparator();
	m_barGrid->addAction(m_actionManager->find("undo"));
	m_barGrid->addAction(m_actionManager->find("redo"));
	m_barGrid->addSeparator();
	m_barGrid->addActions(m_editGroup->actions());
	m_barGrid->addSeparator();
// 	m_barGrid->addAction(m_aNtsc);
// 	m_barGrid->addAction(m_aLightTable);
	m_barGrid->addSeparator();
	m_barGrid->addActions(m_viewPreviousGroup->actions());
	m_barGrid->addSeparator();
	m_barGrid->addActions(m_viewNextGroup->actions());
	
}

void KTViewDocument::createMenu()
{
	//tools menu
	m_toolsMenu = new QMenu(tr( "&Tools" ), this);
	menuBar()->addMenu( m_toolsMenu );
	m_toolsMenu->addAction(m_brushesMenu->menuAction ());
	m_toolsMenu->addAction(m_selectionMenu->menuAction ());
	m_toolsMenu->addAction(m_fillMenu->menuAction ());
	m_toolsMenu->addSeparator();
	m_toolsMenu->addAction(m_actionManager->find("group"));
	m_toolsMenu->addAction(m_actionManager->find("ungroup"));
	m_toolsMenu->addSeparator();
	
	m_orderMenu = new QMenu(tr( "&Order" ), this);
	m_orderMenu->addAction(m_actionManager->find("bringToFront"));
	m_orderMenu->addAction(m_actionManager->find("sendToBack"));
	m_orderMenu->addAction(m_actionManager->find("oneStepForward"));
	m_orderMenu->addAction(m_actionManager->find("oneStepBackward"));
	m_toolsMenu->addAction(m_orderMenu->menuAction ());
	
	m_editMenu = new QMenu(tr( "&Edit" ), this);
	
	menuBar()->addMenu( m_editMenu );
	
	m_editMenu->addAction(m_actionManager->find("undo"));
	m_editMenu->addAction(m_actionManager->find("redo"));
	m_editMenu->addSeparator();
	
	m_editMenu->addAction(m_actionManager->find("cut"));
	m_editMenu->addAction(m_actionManager->find("copy"));
	m_editMenu->addAction(m_actionManager->find("paste"));
	
	m_editMenu->addAction(m_actionManager->find("delete"));
	
	m_editMenu->addSeparator();
	m_editMenu->addAction(m_actionManager->find("selectAll"));
	m_editMenu->addSeparator();
	
	m_editMenu->addAction(m_actionManager->find("properties"));
	
	
	m_viewMenu = new QMenu(tr( "&View" ), this);
	m_viewMenu->addActions(m_viewPreviousGroup->actions());
	m_viewMenu->addSeparator();
	m_viewMenu->addActions(m_viewNextGroup->actions());
	menuBar()->addMenu( m_viewMenu );
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


void KTViewDocument::disablePreviousOnionSkin()
{
	m_paintAreaContainer->drawArea()->setPreviousFrames( 0 );
}

void KTViewDocument::onePreviousOnionSkin()
{
	m_paintAreaContainer->drawArea()->setPreviousFrames( 1 );
}

void KTViewDocument::twoPreviousOnionSkin()
{
	m_paintAreaContainer->drawArea()->setPreviousFrames( 2 );
}

void KTViewDocument::threePreviousOnionSkin()
{
	m_paintAreaContainer->drawArea()->setPreviousFrames( 3 );
}
// NEXT
void KTViewDocument::disableNextOnionSkin()
{
	m_paintAreaContainer->drawArea()->setNextFrames( 0 );
}

void KTViewDocument::oneNextOnionSkin()
{
	m_paintAreaContainer->drawArea()->setNextFrames( 1 );
}

void KTViewDocument::twoNextOnionSkin()
{
	m_paintAreaContainer->drawArea()->setNextFrames( 2 );
}

void KTViewDocument::threeNextOnionSkin()
{
	m_paintAreaContainer->drawArea()->setNextFrames( 3 );
}

void KTViewDocument::setScene(KTScene* scene)
{
	setWindowTitle( m_title + " - " + scene->sceneName() );
	m_paintAreaContainer->drawArea()->setScene(  scene );
}

void KTViewDocument::configure()
{
	FUNC_NOT_IMPLEMENTED;
}

