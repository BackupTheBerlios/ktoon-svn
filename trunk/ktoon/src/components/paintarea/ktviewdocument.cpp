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

#include <dcore/ddebug.h>

#include <QLayout>
#include <QStatusBar>
#include <QMenuBar>
#include <QPixmap>
#include <QActionGroup>
#include <QDockWidget>
#include <QTimer>
#include <QApplication>
#include <QCursor>
#include <QAction>
#include <QActionGroup>
#include <QToolBar>
#include <QMenu>
#include <QDir>
#include <QPluginLoader>
#include <QSpinBox>
#include <QFrame>
#include <QGridLayout>

#include <dcore/dconfig.h>

#include "ktpaintareaproperties.h"
#include "ktpluginmanager.h"
#include "ktpaintarea.h"
#include "ktprojectresponse.h"
#include "ktpaintareaevent.h"
#include "ktpaintareacommand.h"

#include "ktbrushmanager.h"

#include "ktproject.h"

#include "ktpaintareastatus.h"


struct KTViewDocument::Private
{
	QActionGroup *gridGroup, *editGroup, *viewNextGroup, *viewZoomGroup, *viewPreviousGroup;
	QMenu *brushesMenu, *selectionMenu, *fillMenu, *filterMenu, *viewToolMenu;
	QMenu *toolsMenu, *editMenu, *viewMenu, *orderMenu;
	QAction *aUndo, *aRedo, *aClose;
	QToolBar *barGrid, *toolbar;
	QDoubleSpinBox *zoomFactorSpin;
	
	KTPaintArea *paintArea;
	
	KTDocumentRuler *verticalRuler, *horizontalRuler;
	DActionManager *actionManager;
	KTConfigurationArea *configurationArea;
};

KTViewDocument::KTViewDocument(KTProject *project, QWidget *parent ) : QMainWindow(parent), d(new Private)
{
	setWindowIcon(QPixmap(THEME_DIR+"/icons/layer_pic.png") ); // FIXME: new image for documents
	
	d->actionManager = new DActionManager(this);
	
	QFrame *frame = new QFrame(this);
	QGridLayout *layout = new QGridLayout(frame);
	
	d->paintArea = new KTPaintArea(project, frame);
	connect(d->paintArea, SIGNAL(scaled(double)), this, SLOT(scaleRuler(double)));
	
	setCentralWidget( frame );
	
	layout->addWidget(d->paintArea, 1,1);
	d->horizontalRuler = new KTDocumentRuler(Qt::Horizontal);
	d->verticalRuler = new KTDocumentRuler(Qt::Vertical);
	
	layout->addWidget(d->horizontalRuler, 0, 1);
	layout->addWidget(d->verticalRuler, 1, 0);
	
	KToon::RenderType renderType = KToon::RenderType(DCONFIG->value("RenderType").toInt()); 
	switch(renderType)
	{
		case KToon::OpenGL:
		{
			d->paintArea->setUseOpenGL( true );
		}
		break;
		case KToon::Native:
		{
			d->paintArea->setUseOpenGL( false );
		}
		break;
		default:
		{
			dWarning() << "Unsopported render, switching to native!";
			d->paintArea->setUseOpenGL( false );
		}
		break;
	}
	
	connect(d->paintArea, SIGNAL(cursorPosition(const QPointF &)),  this,  SLOT(showPos(const QPointF &)) );
	
	connect(d->paintArea, SIGNAL(cursorPosition(const QPointF &)), d->verticalRuler, SLOT(movePointers(const QPointF&)));
	
	connect(d->paintArea, SIGNAL(cursorPosition(const QPointF &)), d->horizontalRuler, SLOT(movePointers(const QPointF&)));
	
	connect(d->paintArea, SIGNAL(changedZero(const QPointF&)), this, SLOT(changeRulerOrigin(const QPointF&)));
	
	connect(d->paintArea, SIGNAL(requestTriggered(const KTProjectRequest* )), this, SIGNAL(requestTriggered(const KTProjectRequest *)));
	
	createActions();
	setupEditActions();
	setupViewActions();
	setupOrderActions();
	
	d->configurationArea = new KTConfigurationArea;
	addDockWidget(Qt::RightDockWidgetArea, d->configurationArea);
	
	createToolBar();
	createTools();
	
	KTPaintAreaStatus *status = new KTPaintAreaStatus(this);
	setStatusBar(status);
	
	connect(d->paintArea->brushManager(), SIGNAL(brushChanged( const QBrush& )), status, SLOT(setBrush(const QBrush &)));
	
	connect(d->paintArea->brushManager(), SIGNAL(penChanged( const QPen& )), status, SLOT(setPen(const QPen &)));
	
	QTimer::singleShot(1000, this, SLOT(loadPlugins()));
}

KTViewDocument::~KTViewDocument()
{
	delete d->configurationArea;
	delete d;
}

void KTViewDocument::setAntialiasing(bool useIt )
{
	d->paintArea->setAntialiasing(useIt);
}

void KTViewDocument::setOpenGL(bool useIt)
{
	d->paintArea->setUseOpenGL( useIt );
}

void KTViewDocument::setDrawGrid(bool draw)
{
	d->paintArea->setDrawGrid(draw);
}

QPainter::RenderHints KTViewDocument::renderHints() const
{
	return d->paintArea->renderHints();
}

void KTViewDocument::setRotationAngle(int angle)
{
	d->paintArea->setRotationAngle(angle);
}

void KTViewDocument::showPos(const QPointF &p)
{
	QString message =  "X: " +  QString::number(p.x()) + " Y: " + QString::number(p.y() );
	emit sendToStatus ( message ) ;
}

void KTViewDocument::createActions()
{
// 	DAction *undo = d->history->undoAction();
// 	undo->setIcon( QPixmap(THEME_DIR+"/icons/undo.png" ));
	
// // 	d->actionManager->insert(undo);
	
// 	DAction *redo = d->history->redoAction();
// 	redo->setIcon(QPixmap(THEME_DIR+"/icons/redo.png" ));
// 	d->actionManager->insert(redo);
	
}

void KTViewDocument::setupEditActions()
{
#if 0
	d->editGroup = new QActionGroup( parent() );
	DAction *a = new DAction( QPixmap(THEME_DIR+"/icons/cut.png" ), tr( "&Cut" ),  QKeySequence(tr("Ctrl+X")), d->paintArea, SLOT(cut()),d->actionManager, "cut" );
	a->setShortcutContext ( Qt::ApplicationShortcut );
	d->editGroup->addAction(a);

	
	a = new DAction( QPixmap(THEME_DIR+"/icons/copy.png" ), tr( "C&opy" ),  QKeySequence(tr("Ctrl+C")), d->paintArea, SLOT(copy()), d->actionManager, "copy");
	
	a->setShortcutContext ( Qt::ApplicationShortcut );
	d->editGroup->addAction(a);
	
	a->setStatusTip(tr("Copies the selection and puts it onto the clipboard"));
	
	a = new DAction( QPixmap(THEME_DIR+"/icons/paste.png" ), tr( "&Paste" ),   QKeySequence(tr("Ctrl+V")), d->paintArea, SLOT(paste()), d->actionManager, "paste");
	
	a->setShortcutContext ( Qt::ApplicationShortcut );
	d->editGroup->addAction(a);
	a->setStatusTip(tr("Pastes the clipboard into the current document"));
	
	a = new DAction( QPixmap(THEME_DIR+"/icons/delete.png" ), tr( "&Delete" ), QKeySequence( Qt::Key_Delete ), d->paintArea, SLOT(removeSelectsGraphics()), d->actionManager, "delete");
// 	d->editGroup->addAction(a);
	a->setShortcutContext ( Qt::ApplicationShortcut );
	a->setStatusTip(tr("Deletes the selected object"));
	
	
	a = new DAction( QPixmap(THEME_DIR+"/icons/group.png" ), tr( "&Group" ),   QKeySequence(  ), d->paintArea, SLOT(group()), d->actionManager, "group");
	a->setShortcut ( QKeySequence(tr("Ctrl+G")) );

	d->editGroup->addAction(a);
	a->setStatusTip(tr("Group the selected objects into a single one"));
	
	a = new DAction( QPixmap(THEME_DIR+"/icons/ungroup.png" ), tr( "&Ungroup" ),   QKeySequence(  ), d->paintArea, SLOT(ungroup()), d->actionManager, "ungroup");
	a->setShortcut ( QKeySequence(tr("Ctrl+Shift+G")) );
	d->editGroup->addAction(a);
	a->setStatusTip(tr("Ungroups the selected object"));
	
	a = new DAction( QPixmap(THEME_DIR+"/icons/" ), tr( "Select &All" ),   QKeySequence(tr("Ctrl+A")), d->paintArea, SLOT(selectAll()), d->actionManager, "selectAll");
	a->setStatusTip(tr("selected all object"));
	
	DAction *flipV = new DAction(  QPixmap(THEME_DIR+"/icons/flip-vertical.png" ), tr("Flip Vertical"), QKeySequence(tr(" Ctrl + Alt + V")), d->paintArea, SLOT(flipVCurrentElement()), d->actionManager, "flipv");
	
	DAction *flipH = new DAction( QPixmap(THEME_DIR+"/icons/flip-horizontal.png" ),  tr("Flip Horizontal"), QKeySequence(tr(" Ctrl + Alt + H")), d->paintArea, SLOT(flipHCurrentElement()), d->actionManager, "fliph");
	
	a = new DAction( tr("Properties..."), QKeySequence(), this, SLOT(configure()), d->actionManager, "properties");
	a->setStatusTip(tr("Configure the paint area"));
#endif
}

void KTViewDocument::setupOrderActions()
{
#if 0
	DAction *bringtoFront = new DAction(QPixmap(THEME_DIR+"/icons/bring_to_front.png" ), tr( "&Bring to Front" ),  QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_Up), d->paintArea, SLOT(bringToFromSelected()), d->actionManager, "bringToFront" );
	
	bringtoFront->setShortcutContext ( Qt::ApplicationShortcut );
	bringtoFront->setStatusTip(tr("Brings the selected object to the front"));
	
	
	DAction *sendToBack = new DAction(QPixmap(THEME_DIR+"/icons/send_to_back.png" ), tr( "&Send to Back" ),  QKeySequence(Qt::CTRL+Qt::SHIFT+Qt::Key_Down), d->paintArea, SLOT(sendToBackSelected()), d->actionManager, "sendToBack" );
	
	sendToBack->setShortcutContext ( Qt::ApplicationShortcut );
	sendToBack->setStatusTip(tr("Sends the selected objects to the back"));
	

	
	DAction *oneStepForward = new DAction(QPixmap(THEME_DIR+"/icons/one_forward.png" ), tr( "One Step &Forward" ),  QKeySequence(Qt::CTRL+Qt::Key_Up), d->paintArea, SLOT(oneStepForwardSelected()), d->actionManager, "oneStepForward" );
	
	oneStepForward->setShortcutContext ( Qt::ApplicationShortcut );
	oneStepForward->setStatusTip(tr("Moves the selected object one step forward"));
	
	
	DAction *oneStepBackward = new DAction(QPixmap(THEME_DIR+"/icons/one_backward.png" ), tr( "One Step B&ackward" ),  QKeySequence(Qt::CTRL+Qt::Key_Down), d->paintArea, SLOT(oneStepBackwardSelected()), d->actionManager, "oneStepBackward" );
	
	oneStepBackward->setShortcutContext ( Qt::ApplicationShortcut );
	oneStepBackward->setStatusTip(tr("Moves the selected object one step backward"));
#endif
	
}

void KTViewDocument::setupViewActions()
{
	DAction *showGrid = new DAction( QPixmap(THEME_DIR+"/icons/subgrid.png" ), tr( "Show grid" ), QKeySequence(), this, SLOT(toggleShowGrid()), d->actionManager, "show_grid" );
	showGrid->setCheckable(true);
	
	DAction *del = new DAction( QPixmap(THEME_DIR+"/icons/delete.png" ), tr( "Delete" ), QKeySequence( Qt::Key_Delete ), d->paintArea, SLOT(deleteItems()), d->actionManager, "delete" );
	
// 	del->setShortcutContext(Qt::WidgetShortcut);
// 	d->paintArea->addAction(del);
	
	del->setStatusTip(tr("Deletes the selected object"));
	
	DAction *group = new DAction( QPixmap(THEME_DIR+"/icons/group.png" ), tr( "&Group" ),   QKeySequence(tr("Ctrl+G") ), d->paintArea, SLOT(groupItems()), d->actionManager, "group");
	group->setStatusTip(tr("Group the selected objects into a single one"));
	
	DAction *ungroup = new DAction( QPixmap(THEME_DIR+"/icons/ungroup.png" ), tr( "&Ungroup" ), QKeySequence(tr("Ctrl+Shift+G")) , d->paintArea, SLOT(ungroupItems()), d->actionManager, "ungroup");
	ungroup->setStatusTip(tr("Ungroups the selected object"));
	
	DAction *copy = new DAction( QPixmap(THEME_DIR+"/icons/copy.png" ), tr( "C&opy" ),  QKeySequence(tr("Ctrl+C")), d->paintArea, SLOT(copyItems()), d->actionManager, "copy");
	copy->setStatusTip(tr("Copies the selection and puts it onto the clipboard"));
	
	
	DAction *paste = new DAction( QPixmap(THEME_DIR+"/icons/paste.png" ), tr( "&Paste" ),   QKeySequence(tr("Ctrl+V")), d->paintArea, SLOT(pasteItems()), d->actionManager, "paste");
	paste->setStatusTip(tr("Pastes the clipboard into the current document"));
	
	
	DAction *cut = new DAction( QPixmap(THEME_DIR+"/icons/cut.png" ), tr( "&Cut" ),  QKeySequence(tr("Ctrl+X")), d->paintArea, SLOT(cutItems()),d->actionManager, "cut" );
	cut->setStatusTip(tr("Cuts the selected items"));
	
#if 0
	DAction *zoomIn = new DAction( QPixmap(THEME_DIR+"/icons/zood->in.png" ), tr( "Zoom In" ), QKeySequence(Qt::CTRL+Qt::Key_Plus), d->paintArea, SLOT(zoomIn()), d->actionManager, "zood->in" );
	
	d->zoomFactorSpin = new QSpinBox();
	d->zoomFactorSpin->setMaximum ( 200 );
	d->zoomFactorSpin->setMinimum ( 26 );
	d->zoomFactorSpin->setValue(100);
	d->zoomFactorSpin->setSingleStep(5);
	
	d->zoomFactorSpin->setSuffix ( "%" );
	connect( d->zoomFactorSpin, SIGNAL( valueChanged ( int  )), this, SLOT(setZoomFactor(int )));
	
	DAction *zoomOut = new DAction( QPixmap(THEME_DIR+"/icons/zood->out.png" ), tr( "Zoom Out" ), QKeySequence(Qt::CTRL+Qt::Key_Minus), d->paintArea, SLOT(zoomOut()), d->actionManager, "zood->out" );
// 	d->viewPreviousGroup->addAction(zoomOut);
	
#endif
	d->viewPreviousGroup = new QActionGroup( this );
	d->viewPreviousGroup->setExclusive( true );
	DAction *noPrevious = new DAction( QPixmap(THEME_DIR+"/icons/no_previous.png" ), tr( "No Previous" ), QKeySequence(Qt::Key_1), this, SLOT(disablePreviousOnionSkin()), d->actionManager, "no_previous" );
	
	d->viewPreviousGroup->addAction(noPrevious);
	
	noPrevious->setCheckable ( true );
	noPrevious->setStatusTip(tr("Disables previous onion skin visualization"));
	
	noPrevious->setChecked(true);
	
	DAction *onePrevious = new DAction( QPixmap(THEME_DIR+"/icons/previous.png" ), tr( "Previous One" ), QKeySequence(Qt::Key_2), this, SLOT(onePreviousOnionSkin()), d->actionManager, "previews_one");
	
	d->viewPreviousGroup->addAction(onePrevious);
	
	onePrevious->setStatusTip(tr("Shows the previous onion skin" ));
	onePrevious->setCheckable ( true );
	
	DAction *twoPrevious = new DAction( QPixmap(THEME_DIR+"/icons/previous2.png" ), tr( "Previous Two" ), QKeySequence(Qt::Key_3), this, SLOT(twoPreviousOnionSkin()), d->actionManager, "previews_two");
	d->viewPreviousGroup->addAction(twoPrevious);
	twoPrevious->setStatusTip(tr("Shows the previous 2 onion skins" ));
	twoPrevious->setCheckable ( true );
	
	DAction *threePrevious = new DAction( QPixmap(THEME_DIR+"/icons/previous3.png" ), tr( "Previous Three" ), QKeySequence(Qt::Key_4), this, SLOT(threePreviousOnionSkin()), d->actionManager, "previews_three");
	d->viewPreviousGroup->addAction(threePrevious);
	threePrevious->setCheckable ( true );
	threePrevious->setStatusTip(tr("Shows the previous 3 onion skins" ));
	
	
// 	// NEXT 

	d->viewNextGroup = new QActionGroup( this );
	d->viewNextGroup->setExclusive( true );
	
	DAction *noNext = new DAction( QPixmap(THEME_DIR+"/icons/no_next.png" ), tr( "No Next" ), QKeySequence(Qt::CTRL+Qt::Key_1), this, SLOT(disableNextOnionSkin()), d->actionManager, "no_next");
	d->viewNextGroup->addAction(noNext);
	
	
	noNext->setCheckable ( true );
	noNext->setStatusTip(tr("Disables next onion skin visualization" ));
	
	DAction *oneNext = new DAction( QPixmap(THEME_DIR+"/icons/next.png" ), tr( "Next One" ), QKeySequence(Qt::CTRL+Qt::Key_2), this, SLOT(oneNextOnionSkin()), d->actionManager, "next_one");
	d->viewNextGroup->addAction(oneNext);
	
	oneNext->setCheckable ( true );
	oneNext->setStatusTip(tr("Shows the next onion skin"));
	
	DAction *twoNext = new DAction( QPixmap(THEME_DIR+"/icons/next2.png" ), tr( "Next Two" ), QKeySequence(Qt::CTRL+Qt::Key_3), this, SLOT(twoNextOnionSkin()), d->actionManager, "next_two");
	d->viewNextGroup->addAction(twoNext);
	
	twoNext->setCheckable( true );
	twoNext->setStatusTip(tr("Shows the next 2 onion skins"));
	
	DAction *threeNext = new DAction( QPixmap(THEME_DIR+"/icons/next3.png" ), tr( "Next Three" ), QKeySequence(Qt::CTRL+Qt::Key_4), this, SLOT(threeNextOnionSkin()), d->actionManager, "next_three");
	d->viewNextGroup->addAction(threeNext);
	
	threeNext->setCheckable(true );
	threeNext->setStatusTip(tr("Shows the next 3 onion skins"));
}


void KTViewDocument::createTools()
{
	d->toolbar = new QToolBar(tr("Draw tools"), this);
	d->toolbar->setIconSize( QSize(16,16) );
	addToolBar ( Qt::LeftToolBarArea, d->toolbar );
	
	connect(d->toolbar, SIGNAL(actionTriggered(QAction *)), this, SLOT(selectToolFromMenu(QAction *)));
	
	// Brushes menu
	d->brushesMenu = new QMenu(tr("Brushes"), d->toolbar);
	d->brushesMenu->setIcon( QPixmap(THEME_DIR+"/icons/brush.png") );
	connect( d->brushesMenu, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	
	d->toolbar->addAction(d->brushesMenu->menuAction());
	
	// Selection menu
	
	d->selectionMenu = new QMenu( tr("selection"), d->toolbar );
	d->selectionMenu->setIcon(QPixmap(THEME_DIR+"/icons/selection.png"));
	connect( d->selectionMenu, SIGNAL(triggered (QAction*)), this, SLOT(selectToolFromMenu( QAction*)));
	
	d->toolbar->addAction(d->selectionMenu->menuAction());
	
	// Fill menu
	d->fillMenu = new QMenu(tr("Fill"), d->toolbar);
	d->fillMenu->setIcon(QPixmap(THEME_DIR+"/icons/fill.png"));
	connect(d->fillMenu, SIGNAL(triggered(QAction *)), this, SLOT(selectToolFromMenu( QAction* )));
	
	d->toolbar->addAction(d->fillMenu->menuAction());
	
	// View menu
	d->viewToolMenu = new QMenu(tr("View"), d->toolbar);
	d->viewToolMenu->setIcon(QPixmap(THEME_DIR+"/icons/magnifying.png"));
	connect(d->fillMenu, SIGNAL(triggered(QAction *)), this, SLOT(selectToolFromMenu( QAction* )));
	
	d->toolbar->addAction(d->viewToolMenu->menuAction());
	
#if 0
	d->toolsSelection->addAction(QPixmap(THEME_DIR+"/icons/nodes.png"), tr( "Con&tour Selection" ), d->paintArea, SLOT( slotContourSelection()), tr("T") );
	
	d->toolsDraw = new QMenu( d->toolbar );
	connect( d->toolsDraw, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	
	d->toolsDraw->setIcon(QPixmap(THEME_DIR+"/icons/brush.png"));
	d->toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/brush.png"), tr( "&Brush" ), d->paintArea, SLOT(slotBrush()), tr("B"));
	
	d->toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/pencil.png"), tr( "&Pencil" ), d->paintArea, SLOT( slotPencil()), tr("P"));
	
	d->toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/bezier.png"), tr( "&Pen" ), d->paintArea, SLOT( slotPen()), tr("N"));
	
	d->toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/line.png"), tr( "&line" ), d->paintArea, SLOT( slotLine()),tr("L"));
	
	d->toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/square.png"), tr( "&Rectangle" ), d->paintArea, SLOT( slotRectangle()), tr("R"));
	
	d->toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/ellipse.png"), tr( "&Ellipse" ), d->paintArea, SLOT(slotEllipse()),  tr("E"));
	
	d->toolsFills = new QMenu( "fills", d->toolbar );
	d->toolsFills->setIcon(QPixmap(THEME_DIR+"/icons/fill.png"));
	connect( d->toolsFills, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	
	d->toolsFills->addAction(QPixmap(THEME_DIR+"/icons/fill.png"), tr( "&Fill" ), d->paintArea, SLOT( slotFill()), tr("F"));
	
	d->toolsFills->addAction(QPixmap(THEME_DIR+"/icons/removefill.png"), tr( "&Remove Fill" ), d->paintArea, SLOT( slotRemoveFill()), tr("Shift+F"));
	
	d->toolsFills->addAction(QPixmap(THEME_DIR+"/icons/contour.png"), tr( "&Contour Fill" ), d->paintArea, SLOT( slotContourFill()), tr("Ctrl+F"));
 
	d->toolsFills->addAction(QPixmap(THEME_DIR+"/icons/dropper.png"), tr( "&Dropper"), d->paintArea, SLOT(slotDropper()), tr("D"));

	d->toolsErasers = new QMenu(tr( "Eraser" ), d->toolbar );
	d->toolsErasers->setIcon(QPixmap(THEME_DIR+"/icons/eraser.png"));
	connect( d->toolsErasers, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	
	d->toolsErasers->addAction(QPixmap(THEME_DIR+"/icons/eraser.png"), tr( "&Eraser"), d->paintArea, SLOT( slotEraser()), Qt::SHIFT+Qt::Key_Delete);
	
	d->toolsErasers->addAction(QPixmap(THEME_DIR+"/icons/dropper.png"), tr( "&Slicer" ), d->paintArea, SLOT( slotSlicer()),  Qt::CTRL+Qt::Key_Delete);
	
	d->toolsView = new QMenu(tr( "View" ), d->toolbar );
	d->toolsView->setIcon(QPixmap(THEME_DIR+"/icons/magnifying.png"));
	connect( d->toolsView, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	
	d->toolsView->addAction(QPixmap(THEME_DIR+"/icons/magnifying.png"), tr("&Magnifying Glass" ), d->paintArea, SLOT( slotMagnifyingGlass()), tr("M"));
	
	d->toolsView->addAction(QPixmap(THEME_DIR+"/icons/hand.png"), tr( "&Hand" ), d->paintArea,  SLOT( slotHand()), tr("H"));
	
	d->toolsOrder = new QMenu(tr("Order"), d->toolbar);
	connect( d->toolsOrder, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	d->toolsOrder->setIcon(QPixmap(THEME_DIR+"/icons/group.png"));
	d->toolsOrder->addAction(QPixmap(THEME_DIR+"/icons/group.png"), tr( "&Group" ), d->paintArea, SLOT( slotGroup()));
	d->toolsOrder->addAction(QPixmap(THEME_DIR+"/icons/ungroup.png"), tr( "&Ungroup" ), d->paintArea, SLOT( slotUngroup()));
	

// 	a->setStatusTip(tr("Group the selected objects into a single one"));
// 	a->setStatusTip(tr("Ungroups the selected object"));
	
	

	d->toolsAlign = new QMenu(tr( "Align"), this );
	connect( d->toolsAlign, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	d->toolsAlign->setIcon(QPixmap(THEME_DIR+"/icons/align_l.png"));
	
	d->toolsAlign->addAction( QPixmap(THEME_DIR+"/icons/align_l.png"), tr("&Left" ), d->paintArea, SLOT( slotAlignLeft()));
	
	d->toolsAlign->addAction( QPixmap(THEME_DIR+"/icons/align_cv.png"), tr( "&Center Vertically" ),  d->paintArea, SLOT(slotCenterVertically()));
	
	d->toolsAlign->addAction( QPixmap(THEME_DIR+"/icons/align_r.png"), tr("&Right" ), d->paintArea, SLOT(slotAlignRight()));
	d->toolsAlign->addSeparator();
	d->toolsAlign->addAction( QPixmap(THEME_DIR+"/icons/align_t.png"), tr( "&Top" ), d->paintArea, SLOT(slotAlignTop()));
	d->toolsAlign->addAction(QPixmap(THEME_DIR+"/icons/align_ch.png"), tr("Center &Horizontally" ), d->paintArea,  SLOT( slotCenterHorizontally()));
	d->toolsAlign->addAction(QPixmap(THEME_DIR+"/icons/align_b.png"), tr( "&Bottom" ), d->paintArea, SLOT( slotAlignBottom()));
	
	tools_left->setStatusTip(tr("Aligns the selected object to the left"));
	tools_center_vertically->setStatusTip(tr("Centers vertically the selected object"));
	tools_right->setStatusTip(tr("Aligns the selected object to the right"));
	tools_top->setStatusTip(tr("Aligns the selected object to the top"));
	tools_center_horizontally->setStatusTip(tr("Centers horizontally the selected object"));
	tools_bottom->setStatusTip(tr("Aligns the selected object to the bottom"));

	d->toolsTransform = new QMenu( tr( "Transform " ), this);
	connect( d->toolsTransform, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	d->toolsTransform->setIcon(QPixmap(HOME_DIR+"/images/icons/align_l.png"));

	d->toolsTransform->addAction(tr( "Flip &Horizontally" ), d->paintArea, SLOT(slotFlipHorizontally()));
	d->toolsTransform->addAction(tr( "Flip &Vertically" ), d->paintArea, SLOT(slotFlipVertically()));
	d->toolsTransform->addSeparator();
	d->toolsTransform->addAction(tr( "&Rotate 90 CW" ), d->paintArea, SLOT( slotRotateCW90()));
	d->toolsTransform->addAction(tr( "&Rotate 90 CCW" ), d->paintArea, SLOT( slotRotateCCW90()));
	d->toolsTransform->addSeparator();
	d->toolsTransform->addAction(tr( "&Rotate &180" ),d->paintArea,SLOT( slotRotate180()));
	d->toolsTransform->addAction( QPixmap(THEME_DIR+"/icons/perspective.png"),tr( "&Perspective" ) ,d->paintArea, SLOT( slotRotate180()));
	
	
	tools_flip_horizontally->setStatusTip(tr("Flips the selected object horizontally"));
	tools_flip_vertically->setStatusTip(tr("Flips the selected object vertically"));
	tools_rotate_cw90->setStatusTip(tr("Rotates the selected object 90 degrees clockwise"));
	tools_rotate_ccw90->setStatusTip(tr("Rotates the selected object 90 degrees counterclockwise"));
	tools_rotate180->setStatusTip(tr("Rotates the selected object 180 degrees"));
	tools_perspective->setStatusTip(tr("Activates the perspective tool"));
	
	d->toolbar->addAction(d->toolsSelection->menuAction());
	d->toolbar->addAction(d->toolsDraw->menuAction());
	d->toolbar->addAction(d->toolsFills->menuAction());
	d->toolbar->addAction(d->toolsErasers->menuAction());
	d->toolbar->addAction(d->toolsView->menuAction());
	d->toolbar->addAction(d->toolsOrder->menuAction());
	d->toolbar->addAction(d->toolsAlign->menuAction());
	d->toolbar->addAction(d->toolsTransform->menuAction());
#endif
}

void KTViewDocument::loadPlugins()
{
	foreach(QObject *plugin, KTPluginManager::instance()->tools() )
	{
		KTToolPlugin *tool = qobject_cast<KTToolPlugin *>(plugin);
		
		QStringList::iterator it;
		QStringList keys = tool->keys();
			
		for (it = keys.begin(); it != keys.end(); ++it)
		{
			dDebug("plugins") << "*******Tool Loaded: " << *it;
			
			DAction *act = tool->actions()[*it];
			if ( act )
			{
				connect(act, SIGNAL(triggered()), this, SLOT(selectTool()));
				
				act->setParent(plugin);
				
				switch( tool->toolType() )
				{
					case KTToolInterface::Brush:
					{
						d->brushesMenu->addAction(act);
						if ( !d->brushesMenu->activeAction() )
						{
							act->trigger();
						}
					}
					break;
					case KTToolInterface::Selection:
					{
						d->selectionMenu->addAction(act);
						if ( !d->selectionMenu->activeAction() )
						{
							act->trigger();
						}
					}
					break;
					case KTToolInterface::Fill:
					{
						d->fillMenu->addAction(act);
						if ( !d->fillMenu->activeAction() )
						{
							act->trigger();
						}
					}
					break;
					case KTToolInterface::View:
					{
						d->viewToolMenu->addAction(act);
						if ( !d->viewToolMenu->activeAction() )
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
// // 				d->paintArea->setTool(tool, *it);
			}
		}
		
// 		connect(plugin, SIGNAL(toDrawGhostGraphic(const QPainterPath &)), d->paintArea, SLOT(drawGhostGraphic(const QPainterPath &)));
// 		connect(plugin, SIGNAL(requestRedraw()), d->paintArea, SLOT(redrawAll()));
	}
	
	foreach(QObject *plugin, KTPluginManager::instance()->filters() )
	{
		AFilterInterface *filter = qobject_cast<AFilterInterface *>(plugin);
		QStringList::iterator it;
		QStringList keys = filter->keys();
				
		for (it = keys.begin(); it != keys.end(); ++it)
		{
			dDebug("plugins") << "*******Filter Loaded: " << *it;
					
			DAction *act = filter->actions()[*it];
			if ( act )
			{
				connect(act, SIGNAL(triggered()), this, SLOT(applyFilter()));
				d->filterMenu->addAction(act);
			}
		}
	}
}

void KTViewDocument::selectTool()
{
	D_FUNCINFO;
	DAction *action = qobject_cast<DAction *>(sender());
	
	if ( action )
	{
		KTToolPlugin *tool = qobject_cast<KTToolPlugin *>(action->parent());
		QString toolStr = action->text();
		
		switch(tool->toolType())
		{
			case KTToolInterface::Brush:
			{
				d->brushesMenu->setDefaultAction(action);
				d->brushesMenu->setActiveAction(action);
				if ( !action->icon().isNull() )
				{
					d->brushesMenu->menuAction()->setIcon(action->icon());
				}
			}
			break;
			case KTToolInterface::Fill:
			{
				d->fillMenu->setDefaultAction(action);
				d->fillMenu->setActiveAction(action);
				if ( !action->icon().isNull() )
				{
					d->fillMenu->menuAction()->setIcon(action->icon());
				}
			}
			break;
		
			case KTToolInterface::Selection:
			{
				d->selectionMenu->setDefaultAction(action);
				d->selectionMenu->setActiveAction(action);
				if ( !action->icon().isNull() )
				{
					d->selectionMenu->menuAction()->setIcon(action->icon());
				}
			}
			break;
			
			case KTToolInterface::View:
			{
				d->viewToolMenu->setDefaultAction(action);
				d->viewToolMenu->setActiveAction(action);
				if ( !action->icon().isNull() )
				{
					d->viewToolMenu->menuAction()->setIcon(action->icon());
				}
			}
			break;
		}
		
		QWidget *toolConfigurator = tool->configurator();
		
		if ( toolConfigurator)
		{
			d->configurationArea->setConfigurator( toolConfigurator );
			toolConfigurator->show();
			if ( !d->configurationArea->isVisible() )
			{
				d->configurationArea->show();
			}
		}
		else
		{
			if ( d->configurationArea->isVisible() )
			{
				d->configurationArea->close();
			}
		}
		
		tool->setCurrentTool( toolStr );
		d->paintArea->setTool(tool);
		
		d->paintArea->viewport()->setCursor(action->cursor());
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

bool KTViewDocument::handleProjectResponse(KTProjectResponse *event)
{
	return d->paintArea->handleResponse(event);
}

void KTViewDocument::applyFilter()
{
	QAction *action = qobject_cast<QAction *>(sender());
	
	if ( action )
	{
		AFilterInterface *aFilter = qobject_cast<AFilterInterface *>(action->parent());
		QString filter = action->text();
		
// 		KTFrame *frame = d->paintArea->currentFrame();
// 		
// 		if( frame)
// 		{
// 			aFilter->filter(action->text(), frame->components() );
// 			d->paintArea->redrawAll();
// 		}
	}
}

void KTViewDocument::updateZoomFactor(double f)
{
// 	d->paintArea->setZoomFactor( f );
	d->zoomFactorSpin->blockSignals(true);
	d->zoomFactorSpin->setValue(f*100);
	d->zoomFactorSpin->blockSignals(false);
}

void KTViewDocument::createToolBar()
{
	d->barGrid = new QToolBar(tr("Paint area actions"), this);
	d->barGrid->setIconSize( QSize(16,16) );
	
	addToolBar(d->barGrid);
	
	d->barGrid->addAction(d->actionManager->find("show_grid"));
	d->barGrid->addAction(d->actionManager->find("delete"));
	d->barGrid->addAction(d->actionManager->find("group"));
	d->barGrid->addAction(d->actionManager->find("ungroup"));
	d->barGrid->addAction(d->actionManager->find("copy"));
	d->barGrid->addAction(d->actionManager->find("paste"));
	
	d->barGrid->addAction(d->actionManager->find("cut"));
	
// 	d->barGrid->addSeparator();
// 	d->barGrid->addAction(d->actionManager->find("undo"));
// 	d->barGrid->addAction(d->actionManager->find("redo"));
// 	d->barGrid->addSeparator();
// 	d->barGrid->addAction(d->actionManager->find("zood->in"));
// 	d->barGrid->addWidget(d->zoomFactorSpin);
// 	d->barGrid->addAction(d->actionManager->find("zood->out"));
// 	d->barGrid->addSeparator();
	
// 	d->barGrid->addActions(d->editGroup->actions());
// 	d->barGrid->addSeparator();
	d->barGrid->addSeparator();
	d->barGrid->addActions(d->viewPreviousGroup->actions());
	
	QSpinBox *prevOnionSkinSpin = new QSpinBox(this);
	connect(prevOnionSkinSpin, SIGNAL(valueChanged ( int)), this, SLOT(setPreviousOnionSkin(int)));
	
	d->barGrid->addWidget(prevOnionSkinSpin);
	
	d->barGrid->addSeparator();
	d->barGrid->addActions(d->viewNextGroup->actions());
	
	QSpinBox *nextOnionSkinSpin = new QSpinBox(this);
	connect(nextOnionSkinSpin, SIGNAL(valueChanged ( int)), this, SLOT(setNextOnionSkin(int)));
	
	d->barGrid->addWidget(nextOnionSkinSpin);
}

void KTViewDocument::createMenu()
{
	//tools menu
	d->toolsMenu = new QMenu(tr( "&Tools" ), this);
	menuBar()->addMenu( d->toolsMenu );
	d->toolsMenu->addAction(d->brushesMenu->menuAction ());
	d->toolsMenu->addAction(d->selectionMenu->menuAction ());
	d->toolsMenu->addAction(d->fillMenu->menuAction ());
	d->toolsMenu->addSeparator();
	d->toolsMenu->addAction(d->actionManager->find("group"));
	d->toolsMenu->addAction(d->actionManager->find("ungroup"));
	d->toolsMenu->addSeparator();
	
	d->orderMenu = new QMenu(tr( "&Order" ), this);
	d->orderMenu->addAction(d->actionManager->find("bringToFront"));
	d->orderMenu->addAction(d->actionManager->find("sendToBack"));
	d->orderMenu->addAction(d->actionManager->find("oneStepForward"));
	d->orderMenu->addAction(d->actionManager->find("oneStepBackward"));
	d->toolsMenu->addAction(d->orderMenu->menuAction ());
	
	d->editMenu = new QMenu(tr( "&Edit" ), this);
	
	menuBar()->addMenu( d->editMenu );
	
	d->editMenu->addAction(d->actionManager->find("undo"));
	d->editMenu->addAction(d->actionManager->find("redo"));
	d->editMenu->addSeparator();
	
	d->editMenu->addAction(d->actionManager->find("cut"));
	d->editMenu->addAction(d->actionManager->find("copy"));
	d->editMenu->addAction(d->actionManager->find("paste"));
	
	d->editMenu->addAction(d->actionManager->find("delete"));
	
	d->editMenu->addSeparator();
	d->editMenu->addAction(d->actionManager->find("selectAll"));
	d->editMenu->addSeparator();
// 	d->editMenu->addAction(d->actionManager->find("localflipv"));
// 	d->editMenu->addAction(d->actionManager->find("localfliph"));
// 	d->editMenu->addSeparator();
	d->editMenu->addAction(d->actionManager->find("properties"));
	
	
	d->viewMenu = new QMenu(tr( "&View" ), this);
	d->viewMenu->addActions(d->viewPreviousGroup->actions());
	d->viewMenu->addSeparator();
	d->viewMenu->addActions(d->viewNextGroup->actions());
	menuBar()->addMenu( d->viewMenu );
	
	//Filters
	
	d->filterMenu = new QMenu(tr("Filters"), this);
	menuBar()->addMenu(d->filterMenu);
}

void KTViewDocument::closeArea()
{
	d->paintArea->setScene(0);
	close();
}

void KTViewDocument::setCursor(const QCursor &)
{
// 	d->paintArea->setCursor(c);
}


void KTViewDocument::disablePreviousOnionSkin()
{
	d->paintArea->setPreviousFramesOnionSkinCount( 0 );
}

void KTViewDocument::onePreviousOnionSkin()
{
	d->paintArea->setPreviousFramesOnionSkinCount( 1 );
}

void KTViewDocument::twoPreviousOnionSkin()
{
	d->paintArea->setPreviousFramesOnionSkinCount( 2 );
}

void KTViewDocument::threePreviousOnionSkin()
{
	d->paintArea->setPreviousFramesOnionSkinCount( 3 );
}

void KTViewDocument::setPreviousOnionSkin(int n)
{
	d->paintArea->setPreviousFramesOnionSkinCount(n);
}

// NEXT
void KTViewDocument::disableNextOnionSkin()
{
	d->paintArea->setNextFramesOnionSkinCount( 0 );
}

void KTViewDocument::oneNextOnionSkin()
{
	d->paintArea->setNextFramesOnionSkinCount( 1 );
}

void KTViewDocument::twoNextOnionSkin()
{
	d->paintArea->setNextFramesOnionSkinCount( 2 );
}

void KTViewDocument::threeNextOnionSkin()
{
	d->paintArea->setNextFramesOnionSkinCount( 3 );
}


void KTViewDocument::setNextOnionSkin(int n)
{
	d->paintArea->setNextFramesOnionSkinCount( n );
}

void KTViewDocument::toggleShowGrid()
{
	d->paintArea->setDrawGrid( !d->paintArea->drawGrid() );
}

// void KTViewDocument::setScene(KTScene* scene)
// {
// 	setWindowTitle( d->title + " - " + scene->sceneName() );
// 	d->paintArea->setScene(  scene );
// }

void KTViewDocument::setZoomFactor(int /*percent*/)
{
	d->zoomFactorSpin->blockSignals(true);
// 	d->paintArea->setZoomFactor((float) porcent/100);
	d->zoomFactorSpin->blockSignals(false);
}

void KTViewDocument::scaleRuler(double factor)
{
#if 0
	double sep = factor * d->verticalRuler->scaleFactor();
	d->verticalRuler->scale(sep);
	d->horizontalRuler->scale(sep);
#endif
}

void KTViewDocument::changeRulerOrigin(const QPointF &zero)
{
	d->verticalRuler->setZeroAt(zero);
	d->horizontalRuler->setZeroAt(zero);
}

// void KTViewDocument::configure()
// {
// 	KTPaintAreaConfig properties;
// 	
// 	if ( properties.exec() != QDialog::Rejected )
// 	{
// 		KTPaintAreaProperties areaProperties;
// 		
// 		areaProperties.gridColor = properties.gridColor();
// 		areaProperties.backgroundColor = properties.backgroundColor();
// 		areaProperties.onionSkinColor = properties.onionSkinColor();
// 		areaProperties.onionSkinBackground = properties.onionSkinBackground();
// 		areaProperties.gridSeparation = properties.gridSeparation();
// 		
// // 		d->paintArea->setProperties(areaProperties);
// 	}
// }

// void KTViewDocument::closeEvent(QCloseEvent *e)
// {
// 	
// }

QSize KTViewDocument::sizeHint() const
{
	QSize size(parentWidget()->size());
	return size.expandedTo(QApplication::globalStrut());
}


KTBrushManager *KTViewDocument::brushManager() const
{
	return d->paintArea->brushManager();
}


KTPaintAreaCommand *KTViewDocument::createCommand(const KTPaintAreaEvent *event)
{
	KTPaintAreaCommand *command = new KTPaintAreaCommand(d->paintArea, event);
	
	return command;
}



