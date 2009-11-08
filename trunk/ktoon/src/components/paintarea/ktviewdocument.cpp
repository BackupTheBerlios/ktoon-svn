/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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

// KOM
#include <kcore/kdebug.h>
#include <kcore/kconfig.h>

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

#include "ktpaintareaproperties.h"
#include "ktpluginmanager.h"
#include "ktpaintarea.h"
#include "ktprojectresponse.h"
#include "ktpaintareaevent.h"
#include "ktpaintareacommand.h"

#include "ktbrushmanager.h"

#include "ktproject.h"

#include "ktpaintareastatus.h"

/**
 * This class defines all the environment for the Ilustration interface.
 * Here is where all the tools for Ilustration are defined.
 * @author David Cuadrado <krawek@toonka.com>
*/

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
	KActionManager *actionManager;
	KTConfigurationArea *configurationArea;
};

KTViewDocument::KTViewDocument(KTProject *project, QWidget *parent ) : QMainWindow(parent), k(new Private)
{
	setWindowIcon(QPixmap(THEME_DIR+"/icons/layer_pic.png") ); // FIXME: new image for documents
	
	k->actionManager = new KActionManager(this);
	
	QFrame *frame = new QFrame(this);
	QGridLayout *layout = new QGridLayout(frame);
	
	k->paintArea = new KTPaintArea(project, frame);
	connect(k->paintArea, SIGNAL(scaled(double)), this, SLOT(scaleRuler(double)));
	
	setCentralWidget( frame );
	
	layout->addWidget(k->paintArea, 1,1);
	k->horizontalRuler = new KTDocumentRuler(Qt::Horizontal);
	k->verticalRuler = new KTDocumentRuler(Qt::Vertical);
	
	layout->addWidget(k->horizontalRuler, 0, 1);
	layout->addWidget(k->verticalRuler, 1, 0);
	
	KToon::RenderType renderType = KToon::RenderType(KCONFIG->value("RenderType").toInt()); 
	switch(renderType)
	{
		case KToon::OpenGL:
		{
			k->paintArea->setUseOpenGL( true );
		}
		break;
		case KToon::Native:
		{
			k->paintArea->setUseOpenGL( false );
		}
		break;
		default:
		{
			#ifdef K_DEBUG
				kWarning() << "Unsopported render, switching to native!";
			#endif

			k->paintArea->setUseOpenGL( false );
		}
		break;
	}
	
	connect(k->paintArea, SIGNAL(cursorPosition(const QPointF &)),  this,  SLOT(showPos(const QPointF &)) );
	
	connect(k->paintArea, SIGNAL(cursorPosition(const QPointF &)), k->verticalRuler, 
				     SLOT(movePointers(const QPointF&)));
	
	connect(k->paintArea, SIGNAL(cursorPosition(const QPointF &)), k->horizontalRuler, 
				     SLOT(movePointers(const QPointF&)));
	
	connect(k->paintArea, SIGNAL(changedZero(const QPointF&)), this, SLOT(changeRulerOrigin(const QPointF&)));
	
	connect(k->paintArea, SIGNAL(requestTriggered(const KTProjectRequest* )), this, 
				     SIGNAL(requestTriggered(const KTProjectRequest *)));
	
	setupDrawActions();
	
	k->configurationArea = new KTConfigurationArea;
	addDockWidget(Qt::RightDockWidgetArea, k->configurationArea);
	
	createToolBar();
	createTools();
	
	KTPaintAreaStatus *status = new KTPaintAreaStatus(this);
	setStatusBar(status);
	
	connect(k->paintArea->brushManager(), SIGNAL(brushChanged( const QBrush& )), status, 
						     SLOT(setBrush(const QBrush &)));
	
	connect(k->paintArea->brushManager(), SIGNAL(penChanged( const QPen& )), status, SLOT(setPen(const QPen &)));
	
	QTimer::singleShot(1000, this, SLOT(loadPlugins()));
}

KTViewDocument::~KTViewDocument()
{
	delete k->configurationArea;
	delete k;
}

void KTViewDocument::setAntialiasing(bool useIt )
{
	k->paintArea->setAntialiasing(useIt);
}

void KTViewDocument::setOpenGL(bool useIt)
{
	k->paintArea->setUseOpenGL( useIt );
}

void KTViewDocument::setDrawGrid(bool draw)
{
	k->paintArea->setDrawGrid(draw);
}

QPainter::RenderHints KTViewDocument::renderHints() const
{
	return k->paintArea->renderHints();
}

void KTViewDocument::setRotationAngle(int angle)
{
	k->paintArea->setRotationAngle(angle);
}

void KTViewDocument::showPos(const QPointF &p)
{
	QString message =  "X: " +  QString::number(p.x()) + " Y: " + QString::number(p.y() );
	emit sendToStatus ( message ) ;
}

void KTViewDocument::setupDrawActions()
{
	KAction *showGrid = new KAction( QPixmap(THEME_DIR+"/icons/subgrid.png" ), tr( "Show grid" ), QKeySequence(tr("Ctrl+L")), this,
					 SLOT(toggleShowGrid()), k->actionManager, "show_grid" );
	showGrid->setCheckable(true);

        KAction *undo = new KAction( QPixmap(THEME_DIR+"/icons/undo.png" ), tr( "&Undo" ),  QKeySequence(tr("Ctrl+U")), this,
                                     SLOT(undo()), k->actionManager, "undo" );
        undo->setStatusTip(tr("Undo last operation"));

        KAction *redo = new KAction( QPixmap(THEME_DIR+"/icons/redo.png" ), tr( "&Redo" ),  QKeySequence(tr("Ctrl+R")), this,
                                     SLOT(redo()), k->actionManager, "redo" );
        redo->setStatusTip(tr("Redo last operation"));

        KAction *copy = new KAction( QPixmap(THEME_DIR+"/icons/copy.png" ), tr( "C&opy" ),  QKeySequence(tr("Ctrl+C")),
                                     k->paintArea, SLOT(copyItems()), k->actionManager, "copy");
        copy->setStatusTip(tr("Copies the selection and puts it onto the clipboard"));

        KAction *paste = new KAction( QPixmap(THEME_DIR+"/icons/paste.png" ), tr( "&Paste" ),   QKeySequence(tr("Ctrl+V")),
                                      k->paintArea, SLOT(pasteItems()), k->actionManager, "paste");
        paste->setStatusTip(tr("Pastes the clipboard into the current document"));

        KAction *cut = new KAction( QPixmap(THEME_DIR+"/icons/cut.png" ), tr( "&Cut" ),  QKeySequence(tr("Ctrl+X")),
                                    k->paintArea, SLOT(cutItems()),k->actionManager, "cut" );
        cut->setStatusTip(tr("Cuts the selected items"));

	
	KAction *del = new KAction( QPixmap(THEME_DIR+"/icons/delete.png" ), tr( "Delete" ), 
				    QKeySequence( Qt::Key_Delete ), k->paintArea, SLOT(deleteItems()), k->actionManager, "delete" );
	
	del->setStatusTip(tr("Deletes the selected object"));
	
	KAction *group = new KAction( QPixmap(THEME_DIR+"/icons/group.png" ), tr( "&Group" ),   
				      QKeySequence(tr("Ctrl+G") ), k->paintArea, SLOT(groupItems()), k->actionManager,
				      "group");

	group->setStatusTip(tr("Group the selected objects into a single one"));
	
	KAction *ungroup = new KAction( QPixmap(THEME_DIR+"/icons/ungroup.png" ), tr( "&Ungroup" ), 
					QKeySequence(tr("Ctrl+Shift+G")) , k->paintArea, SLOT(ungroupItems()), 
					k->actionManager, "ungroup");
	ungroup->setStatusTip(tr("Ungroups the selected object"));

/*
	KAction *zoomIn = new KAction( QPixmap(THEME_DIR+"/icons/zood->in.png" ), tr( "Zoom In" ), 
				       QKeySequence(Qt::CTRL+Qt::Key_Plus), k->paintArea, SLOT(zoomIn()), k->actionManager,
				       "zood->in" );
	
	k->zoomFactorSpin = new QSpinBox();
	k->zoomFactorSpin->setMaximum ( 200 );
	k->zoomFactorSpin->setMinimum ( 26 );
	k->zoomFactorSpin->setValue(100);
	k->zoomFactorSpin->setSingleStep(5);
	
	k->zoomFactorSpin->setSuffix ( "%" );
	connect( k->zoomFactorSpin, SIGNAL( valueChanged ( int  )), this, SLOT(setZoomFactor(int )));
	
	KAction *zoomOut = new KAction( QPixmap(THEME_DIR+"/icons/zood->out.png" ), tr( "Zoom Out" ), 
					QKeySequence(Qt::CTRL+Qt::Key_Minus), k->paintArea, SLOT(zoomOut()), 
					k->actionManager, "zood->out" );
	// k->viewPreviousGroup->addAction(zoomOut);
	
*/

	k->viewPreviousGroup = new QActionGroup( this );
	k->viewPreviousGroup->setExclusive( true );
	KAction *noPrevious = new KAction( QPixmap(THEME_DIR+"/icons/no_previous.png" ), tr( "No Previous" ), 
					   QKeySequence(Qt::Key_1), this, SLOT(disablePreviousOnionSkin()), 
					   k->actionManager, "no_previous" );
	
	k->viewPreviousGroup->addAction(noPrevious);
	
	noPrevious->setCheckable ( true );
	noPrevious->setStatusTip(tr("Disables previous onion skin visualization"));
	
	noPrevious->setChecked(true);
	
	KAction *onePrevious = new KAction( QPixmap(THEME_DIR+"/icons/previous.png" ), tr( "Previous One" ), 
					    QKeySequence(Qt::Key_2), this, SLOT(onePreviousOnionSkin()), k->actionManager,
					    "previews_one");
	
	k->viewPreviousGroup->addAction(onePrevious);
	
	onePrevious->setStatusTip(tr("Shows the previous onion skin" ));
	onePrevious->setCheckable ( true );
	
	KAction *twoPrevious = new KAction( QPixmap(THEME_DIR+"/icons/previous2.png" ), tr( "Previous Two" ), 
					    QKeySequence(Qt::Key_3), this, SLOT(twoPreviousOnionSkin()), k->actionManager,
					    "previews_two");
	k->viewPreviousGroup->addAction(twoPrevious);
	twoPrevious->setStatusTip(tr("Shows the previous 2 onion skins" ));
	twoPrevious->setCheckable ( true );
	
	KAction *threePrevious = new KAction( QPixmap(THEME_DIR+"/icons/previous3.png" ), tr( "Previous Three" ),
					      QKeySequence(Qt::Key_4), this, SLOT(threePreviousOnionSkin()), 
					      k->actionManager, "previews_three");
	k->viewPreviousGroup->addAction(threePrevious);
	threePrevious->setCheckable ( true );
	threePrevious->setStatusTip(tr("Shows the previous 3 onion skins" ));
	

	// NEXT 

	k->viewNextGroup = new QActionGroup( this );
	k->viewNextGroup->setExclusive( true );
	
	KAction *noNext = new KAction( QPixmap(THEME_DIR+"/icons/no_next.png" ), tr( "No Next" ), 
				       QKeySequence(Qt::CTRL+Qt::Key_1), this, SLOT(disableNextOnionSkin()), 
				       k->actionManager, "no_next");
	k->viewNextGroup->addAction(noNext);
	
	noNext->setCheckable ( true );
	noNext->setStatusTip(tr("Disables next onion skin visualization"));
	
	KAction *oneNext = new KAction( QPixmap(THEME_DIR+"/icons/next.png" ), tr( "Next One" ), 
					QKeySequence(Qt::CTRL+Qt::Key_2), this, SLOT(oneNextOnionSkin()), k->actionManager,
					"next_one");
	k->viewNextGroup->addAction(oneNext);
	
	oneNext->setCheckable ( true );
	oneNext->setStatusTip(tr("Shows the next onion skin"));
	
	KAction *twoNext = new KAction( QPixmap(THEME_DIR+"/icons/next2.png" ), tr( "Next Two" ), 
					QKeySequence(Qt::CTRL+Qt::Key_3), this, SLOT(twoNextOnionSkin()), k->actionManager,
					"next_two");
	k->viewNextGroup->addAction(twoNext);
	
	twoNext->setCheckable( true );
	twoNext->setStatusTip(tr("Shows the next 2 onion skins"));
	
	KAction *threeNext = new KAction( QPixmap(THEME_DIR+"/icons/next3.png" ), tr( "Next Three" ), 
					  QKeySequence(Qt::CTRL+Qt::Key_4), this, SLOT(threeNextOnionSkin()), 
					  k->actionManager, "next_three");
	k->viewNextGroup->addAction(threeNext);
	
	threeNext->setCheckable(true );
	threeNext->setStatusTip(tr("Shows the next 3 onion skins"));
}


void KTViewDocument::createTools()
{
	k->toolbar = new QToolBar(tr("Draw tools"), this);
	k->toolbar->setIconSize( QSize(16,16) );
	addToolBar ( Qt::LeftToolBarArea, k->toolbar );
	
	connect(k->toolbar, SIGNAL(actionTriggered(QAction *)), this, SLOT(selectToolFromMenu(QAction *)));
	
	// Brushes menu
	k->brushesMenu = new QMenu(tr("Brushes"), k->toolbar);
	k->brushesMenu->setIcon( QPixmap(THEME_DIR+"/icons/brush.png") );
	connect( k->brushesMenu, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	
	k->toolbar->addAction(k->brushesMenu->menuAction());
	
	// Selection menu
	
	k->selectionMenu = new QMenu( tr("selection"), k->toolbar );
	k->selectionMenu->setIcon(QPixmap(THEME_DIR+"/icons/selection.png"));
	connect( k->selectionMenu, SIGNAL(triggered (QAction*)), this, SLOT(selectToolFromMenu( QAction*)));
	
	k->toolbar->addAction(k->selectionMenu->menuAction());
	
	// Fill menu
	k->fillMenu = new QMenu(tr("Fill"), k->toolbar);
	k->fillMenu->setIcon(QPixmap(THEME_DIR+"/icons/fill.png"));
	connect(k->fillMenu, SIGNAL(triggered(QAction *)), this, SLOT(selectToolFromMenu( QAction* )));
	
	k->toolbar->addAction(k->fillMenu->menuAction());
	
	// View menu
	k->viewToolMenu = new QMenu(tr("View"), k->toolbar);
	k->viewToolMenu->setIcon(QPixmap(THEME_DIR+"/icons/magnifying.png"));
	connect(k->fillMenu, SIGNAL(triggered(QAction *)), this, SLOT(selectToolFromMenu( QAction* )));
	
	k->toolbar->addAction(k->viewToolMenu->menuAction());
	
/*
	k->toolsSelection->addAction(QPixmap(THEME_DIR+"/icons/nodes.png"), tr( "Con&tour Selection" ), k->paintArea, 
				     SLOT( slotContourSelection()), tr("T") );
	
	k->toolsDraw = new QMenu( k->toolbar );
	connect( k->toolsDraw, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	
	k->toolsDraw->setIcon(QPixmap(THEME_DIR+"/icons/brush.png"));
	k->toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/brush.png"), tr( "&Brush" ), k->paintArea, SLOT(slotBrush()), 
				tr("B"));
	
	k->toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/pencil.png"), tr( "&Pencil" ), k->paintArea, SLOT( slotPencil()),
				tr("P"));
	
	k->toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/bezier.png"), tr( "&Pen" ), k->paintArea, SLOT( slotPen()), 
			        tr("N"));
	
	k->toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/line.png"), tr( "&line" ), k->paintArea, SLOT( slotLine()),
				tr("L"));
	
	k->toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/square.png"), tr( "&Rectangle" ), k->paintArea, 
				SLOT( slotRectangle()), tr("R"));
	
	k->toolsDraw->addAction(QPixmap(THEME_DIR+"/icons/ellipse.png"), tr( "&Ellipse" ), k->paintArea, 
				SLOT(slotEllipse()),  tr("E"));
	
	k->toolsFills = new QMenu( "fills", k->toolbar );
	k->toolsFills->setIcon(QPixmap(THEME_DIR+"/icons/fill.png"));
	connect( k->toolsFills, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	
	k->toolsFills->addAction(QPixmap(THEME_DIR+"/icons/fill.png"), tr( "&Fill" ), k->paintArea, SLOT( slotFill()), 
				 tr("F"));
	
	k->toolsFills->addAction(QPixmap(THEME_DIR+"/icons/removefill.png"), tr( "&Remove Fill" ), k->paintArea, 
				 SLOT( slotRemoveFill()), tr("Shift+F"));
	
	k->toolsFills->addAction(QPixmap(THEME_DIR+"/icons/contour.png"), tr( "&Contour Fill" ), k->paintArea, 
				 SLOT( slotContourFill()), tr("Ctrl+F"));
 
	k->toolsFills->addAction(QPixmap(THEME_DIR+"/icons/dropper.png"), tr( "&Dropper"), k->paintArea, 
				 SLOT(slotDropper()), tr("D"));

	k->toolsErasers = new QMenu(tr( "Eraser" ), k->toolbar );
	k->toolsErasers->setIcon(QPixmap(THEME_DIR+"/icons/eraser.png"));
	connect( k->toolsErasers, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	
	k->toolsErasers->addAction(QPixmap(THEME_DIR+"/icons/eraser.png"), tr( "&Eraser"), k->paintArea, 
				   SLOT( slotEraser()), Qt::SHIFT+Qt::Key_Delete);
	
	k->toolsErasers->addAction(QPixmap(THEME_DIR+"/icons/dropper.png"), tr( "&Slicer" ), k->paintArea, 
				   SLOT( slotSlicer()),  Qt::CTRL+Qt::Key_Delete);
	
	k->toolsView = new QMenu(tr( "View" ), k->toolbar );
	k->toolsView->setIcon(QPixmap(THEME_DIR+"/icons/magnifying.png"));
	connect( k->toolsView, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	
	k->toolsView->addAction(QPixmap(THEME_DIR+"/icons/magnifying.png"), tr("&Magnifying Glass" ), k->paintArea, 
				        SLOT( slotMagnifyingGlass()), tr("M"));
	
	k->toolsView->addAction(QPixmap(THEME_DIR+"/icons/hand.png"), tr( "&Hand" ), k->paintArea,  SLOT( slotHand()), 
					tr("H"));
	
	k->toolsOrder = new QMenu(tr("Order"), k->toolbar);
	connect( k->toolsOrder, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	k->toolsOrder->setIcon(QPixmap(THEME_DIR+"/icons/group.png"));
	k->toolsOrder->addAction(QPixmap(THEME_DIR+"/icons/group.png"), tr( "&Group" ), k->paintArea, SLOT( slotGroup()));
	k->toolsOrder->addAction(QPixmap(THEME_DIR+"/icons/ungroup.png"), tr( "&Ungroup" ), k->paintArea, 
				 SLOT( slotUngroup()));
	

	// a->setStatusTip(tr("Group the selected objects into a single one"));
	// a->setStatusTip(tr("Ungroups the selected object"));
	
	k->toolsAlign = new QMenu(tr( "Align"), this );
	connect( k->toolsAlign, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	k->toolsAlign->setIcon(QPixmap(THEME_DIR+"/icons/align_l.png"));
	
	k->toolsAlign->addAction( QPixmap(THEME_DIR+"/icons/align_l.png"), tr("&Left" ), k->paintArea, 
				  SLOT( slotAlignLeft()));
	
	k->toolsAlign->addAction( QPixmap(THEME_DIR+"/icons/align_cv.png"), tr( "&Center Vertically" ),  k->paintArea, 
				  SLOT(slotCenterVertically()));
	
	k->toolsAlign->addAction( QPixmap(THEME_DIR+"/icons/align_r.png"), tr("&Right" ), k->paintArea, 
				  SLOT(slotAlignRight()));
	k->toolsAlign->addSeparator();
	k->toolsAlign->addAction( QPixmap(THEME_DIR+"/icons/align_t.png"), tr( "&Top" ), k->paintArea, 
				  SLOT(slotAlignTop()));
	k->toolsAlign->addAction(QPixmap(THEME_DIR+"/icons/align_ch.png"), tr("Center &Horizontally" ), k->paintArea,  
				  SLOT( slotCenterHorizontally()));
	k->toolsAlign->addAction(QPixmap(THEME_DIR+"/icons/align_b.png"), tr( "&Bottom" ), k->paintArea, 
				  SLOT( slotAlignBottom()));
	
	tools_left->setStatusTip(tr("Aligns the selected object to the left"));
	tools_center_vertically->setStatusTip(tr("Centers vertically the selected object"));
	tools_right->setStatusTip(tr("Aligns the selected object to the right"));
	tools_top->setStatusTip(tr("Aligns the selected object to the top"));
	tools_center_horizontally->setStatusTip(tr("Centers horizontally the selected object"));
	tools_bottom->setStatusTip(tr("Aligns the selected object to the bottom"));

	k->toolsTransform = new QMenu( tr( "Transform " ), this);
	connect( k->toolsTransform, SIGNAL(triggered ( QAction * )), this, SLOT(selectToolFromMenu( QAction*)));
	k->toolsTransform->setIcon(QPixmap(HOME_DIR+"/images/icons/align_l.png"));

	k->toolsTransform->addAction(tr( "Flip &Horizontally" ), k->paintArea, SLOT(slotFlipHorizontally()));
	k->toolsTransform->addAction(tr( "Flip &Vertically" ), k->paintArea, SLOT(slotFlipVertically()));
	k->toolsTransform->addSeparator();
	k->toolsTransform->addAction(tr( "&Rotate 90 CW" ), k->paintArea, SLOT( slotRotateCW90()));
	k->toolsTransform->addAction(tr( "&Rotate 90 CCW" ), k->paintArea, SLOT( slotRotateCCW90()));
	k->toolsTransform->addSeparator();
	k->toolsTransform->addAction(tr( "&Rotate &180" ),k->paintArea,SLOT( slotRotate180()));
	k->toolsTransform->addAction( QPixmap(THEME_DIR+"/icons/perspective.png"),tr( "&Perspective" ) ,k->paintArea, 
				      SLOT( slotRotate180()));
	
	tools_flip_horizontally->setStatusTip(tr("Flips the selected object horizontally"));
	tools_flip_vertically->setStatusTip(tr("Flips the selected object vertically"));
	tools_rotate_cw90->setStatusTip(tr("Rotates the selected object 90 degrees clockwise"));
	tools_rotate_ccw90->setStatusTip(tr("Rotates the selected object 90 degrees counterclockwise"));
	tools_rotate180->setStatusTip(tr("Rotates the selected object 180 degrees"));
	tools_perspective->setStatusTip(tr("Activates the perspective tool"));
	
	k->toolbar->addAction(k->toolsSelection->menuAction());
	k->toolbar->addAction(k->toolsDraw->menuAction());
	k->toolbar->addAction(k->toolsFills->menuAction());
	k->toolbar->addAction(k->toolsErasers->menuAction());
	k->toolbar->addAction(k->toolsView->menuAction());
	k->toolbar->addAction(k->toolsOrder->menuAction());
	k->toolbar->addAction(k->toolsAlign->menuAction());
	k->toolbar->addAction(k->toolsTransform->menuAction());
*/
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
			#ifdef K_DEBUG
				K_DEBUG("plugins") << "*******Tool Loaded: " << *it;
			#endif

			KAction *act = tool->actions()[*it];
			if ( act )
			{
				connect(act, SIGNAL(triggered()), this, SLOT(selectTool()));
				
				act->setParent(plugin);
				
				switch( tool->toolType() )
				{
					case KTToolInterface::Brush:
					{
						k->brushesMenu->addAction(act);
						if ( !k->brushesMenu->activeAction() )
						{
							act->trigger();
						}
					}
					break;
					case KTToolInterface::Selection:
					{
						k->selectionMenu->addAction(act);
						if ( !k->selectionMenu->activeAction() )
						{
							act->trigger();
						}
					}
					break;
					case KTToolInterface::Fill:
					{
						k->fillMenu->addAction(act);
						if ( !k->fillMenu->activeAction() )
						{
							act->trigger();
						}
					}
					break;
					case KTToolInterface::View:
					{
						k->viewToolMenu->addAction(act);
						if ( !k->viewToolMenu->activeAction() )
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
				// k->paintArea->setTool(tool, *it);
			}
		}
		
		// connect(plugin, SIGNAL(toDrawGhostGraphic(const QPainterPath &)), k->paintArea, 
		//  			  SLOT(drawGhostGraphic(const QPainterPath &)));
		// connect(plugin, SIGNAL(requestRedraw()), k->paintArea, SLOT(redrawAll()));
	}
	
	foreach(QObject *plugin, KTPluginManager::instance()->filters() )
	{
		AFilterInterface *filter = qobject_cast<AFilterInterface *>(plugin);
		QStringList::iterator it;
		QStringList keys = filter->keys();
				
		for (it = keys.begin(); it != keys.end(); ++it)
		{
			#ifdef K_DEBUG
				K_DEBUG("plugins") << "*******Filter Loaded: " << *it;
			#endif
					
			KAction *act = filter->actions()[*it];
			if ( act )
			{
				connect(act, SIGNAL(triggered()), this, SLOT(applyFilter()));
				k->filterMenu->addAction(act);
			}
		}
	}
}

void KTViewDocument::selectTool()
{
	#ifdef K_DEBUG
		K_FUNCINFO;
	#endif
	KAction *action = qobject_cast<KAction *>(sender());
	
	if ( action )
	{
		KTToolPlugin *tool = qobject_cast<KTToolPlugin *>(action->parent());
		QString toolStr = action->text();
		
		switch(tool->toolType())
		{
			case KTToolInterface::Brush:
			{
				k->brushesMenu->setDefaultAction(action);
				k->brushesMenu->setActiveAction(action);
				if ( !action->icon().isNull() )
				{
					k->brushesMenu->menuAction()->setIcon(action->icon());
				}
			}
			break;
			case KTToolInterface::Fill:
			{
				k->fillMenu->setDefaultAction(action);
				k->fillMenu->setActiveAction(action);
				if ( !action->icon().isNull() )
				{
					k->fillMenu->menuAction()->setIcon(action->icon());
				}
			}
			break;
		
			case KTToolInterface::Selection:
			{
				k->selectionMenu->setDefaultAction(action);
				k->selectionMenu->setActiveAction(action);
				if ( !action->icon().isNull() )
				{
					k->selectionMenu->menuAction()->setIcon(action->icon());
				}
			}
			break;
			
			case KTToolInterface::View:
			{
				k->viewToolMenu->setDefaultAction(action);
				k->viewToolMenu->setActiveAction(action);
				if ( !action->icon().isNull() )
				{
					k->viewToolMenu->menuAction()->setIcon(action->icon());
				}
			}
			break;
		}
		
		QWidget *toolConfigurator = tool->configurator();
		
		if ( toolConfigurator)
		{
			k->configurationArea->setConfigurator( toolConfigurator );
			toolConfigurator->show();
			if ( !k->configurationArea->isVisible() )
			{
				k->configurationArea->show();
			}
		}
		else
		{
			if ( k->configurationArea->isVisible() )
			{
				k->configurationArea->close();
			}
		}
		
		tool->setCurrentTool( toolStr );
		k->paintArea->setTool(tool);
		
		k->paintArea->viewport()->setCursor(action->cursor());
	}
}

void KTViewDocument::selectToolFromMenu(QAction *action)
{
	#ifdef K_DEBUG
		K_FUNCINFO;
	#endif
	
	QMenu *menu = qobject_cast<QMenu *>(action->parent());
	if (menu )
	{
		KAction *tool = qobject_cast<KAction *>(menu->activeAction());
		
		if ( tool )
		{
			tool->trigger(); // this call selectTool()
		}
	}
}

bool KTViewDocument::handleProjectResponse(KTProjectResponse *event)
{
	return k->paintArea->handleResponse(event);
}

void KTViewDocument::applyFilter()
{
	QAction *action = qobject_cast<QAction *>(sender());
	
	if ( action )
	{
		AFilterInterface *aFilter = qobject_cast<AFilterInterface *>(action->parent());
		QString filter = action->text();
		
		// KTFrame *frame = k->paintArea->currentFrame();
		// 	
		// if( frame)
		// {
		// 	aFilter->filter(action->text(), frame->components() );
		// 	k->paintArea->redrawAll();
		// }
	}
}

void KTViewDocument::updateZoomFactor(double f)
{
	// k->paintArea->setZoomFactor( f );
	k->zoomFactorSpin->blockSignals(true);
	k->zoomFactorSpin->setValue(f*100);
	k->zoomFactorSpin->blockSignals(false);
}

void KTViewDocument::createToolBar()
{
	k->barGrid = new QToolBar(tr("Paint area actions"), this);
	k->barGrid->setIconSize( QSize(16,16) );
	
	addToolBar(k->barGrid);
	
	k->barGrid->addAction(k->actionManager->find("show_grid"));
        k->barGrid->addSeparator();

        k->barGrid->addAction(k->actionManager->find("undo"));
        k->barGrid->addAction(k->actionManager->find("redo"));
        k->barGrid->addSeparator();

        k->barGrid->addAction(k->actionManager->find("copy"));
        k->barGrid->addAction(k->actionManager->find("paste"));
        k->barGrid->addAction(k->actionManager->find("cut"));
	k->barGrid->addAction(k->actionManager->find("delete"));
        k->barGrid->addSeparator();

	k->barGrid->addAction(k->actionManager->find("group"));
	k->barGrid->addAction(k->actionManager->find("ungroup"));
	
	// k->barGrid->addSeparator();
	// k->barGrid->addAction(d->actionManager->find("undo"));
	// k->barGrid->addAction(d->actionManager->find("redo"));
	// k->barGrid->addSeparator();
	// k->barGrid->addAction(d->actionManager->find("zood->in"));
	// k->barGrid->addWidget(d->zoomFactorSpin);
	// k->barGrid->addAction(d->actionManager->find("zood->out"));
	// k->barGrid->addSeparator();
	
	// k->barGrid->addActions(d->editGroup->actions());
	// k->barGrid->addSeparator();
	k->barGrid->addSeparator();
	k->barGrid->addActions(k->viewPreviousGroup->actions());
	
	QSpinBox *prevOnionSkinSpin = new QSpinBox(this);
	connect(prevOnionSkinSpin, SIGNAL(valueChanged ( int)), this, SLOT(setPreviousOnionSkin(int)));
	
	k->barGrid->addWidget(prevOnionSkinSpin);
	
	k->barGrid->addSeparator();
	k->barGrid->addActions(k->viewNextGroup->actions());
	
	QSpinBox *nextOnionSkinSpin = new QSpinBox(this);
	connect(nextOnionSkinSpin, SIGNAL(valueChanged ( int)), this, SLOT(setNextOnionSkin(int)));
	
	k->barGrid->addWidget(nextOnionSkinSpin);
}

void KTViewDocument::createMenu()
{
	//tools menu
	k->toolsMenu = new QMenu(tr( "&Tools" ), this);
	menuBar()->addMenu( k->toolsMenu );
	k->toolsMenu->addAction(k->brushesMenu->menuAction ());
	k->toolsMenu->addAction(k->selectionMenu->menuAction ());
	k->toolsMenu->addAction(k->fillMenu->menuAction ());
	k->toolsMenu->addSeparator();
	k->toolsMenu->addAction(k->actionManager->find("group"));
	k->toolsMenu->addAction(k->actionManager->find("ungroup"));
	k->toolsMenu->addSeparator();
	
	k->orderMenu = new QMenu(tr( "&Order" ), this);
	k->orderMenu->addAction(k->actionManager->find("bringToFront"));
	k->orderMenu->addAction(k->actionManager->find("sendToBack"));
	k->orderMenu->addAction(k->actionManager->find("oneStepForward"));
	k->orderMenu->addAction(k->actionManager->find("oneStepBackward"));
	k->toolsMenu->addAction(k->orderMenu->menuAction ());
	
	k->editMenu = new QMenu(tr( "&Edit" ), this);
	
	menuBar()->addMenu( k->editMenu );
	
	k->editMenu->addAction(k->actionManager->find("undo"));
	k->editMenu->addAction(k->actionManager->find("redo"));
	k->editMenu->addSeparator();
	
	k->editMenu->addAction(k->actionManager->find("cut"));
	k->editMenu->addAction(k->actionManager->find("copy"));
	k->editMenu->addAction(k->actionManager->find("paste"));
	
	k->editMenu->addAction(k->actionManager->find("delete"));
	
	k->editMenu->addSeparator();
	k->editMenu->addAction(k->actionManager->find("selectAll"));
	k->editMenu->addSeparator();
	// k->editMenu->addAction(d->actionManager->find("localflipv"));
	// k->editMenu->addAction(d->actionManager->find("localfliph"));
	// k->editMenu->addSeparator();
	k->editMenu->addAction(k->actionManager->find("properties"));
	
	k->viewMenu = new QMenu(tr( "&View" ), this);
	k->viewMenu->addActions(k->viewPreviousGroup->actions());
	k->viewMenu->addSeparator();
	k->viewMenu->addActions(k->viewNextGroup->actions());
	menuBar()->addMenu( k->viewMenu );
	
	//Filters
	
	k->filterMenu = new QMenu(tr("Filters"), this);
	menuBar()->addMenu(k->filterMenu);
}

void KTViewDocument::closeArea()
{
	k->paintArea->setScene(0);
	close();
}

void KTViewDocument::setCursor(const QCursor &)
{
	// k->paintArea->setCursor(c);
}

void KTViewDocument::disablePreviousOnionSkin()
{
	k->paintArea->setPreviousFramesOnionSkinCount( 0 );
}

void KTViewDocument::onePreviousOnionSkin()
{
	k->paintArea->setPreviousFramesOnionSkinCount( 1 );
}

void KTViewDocument::twoPreviousOnionSkin()
{
	k->paintArea->setPreviousFramesOnionSkinCount( 2 );
}

void KTViewDocument::threePreviousOnionSkin()
{
	k->paintArea->setPreviousFramesOnionSkinCount( 3 );
}

void KTViewDocument::setPreviousOnionSkin(int n)
{
	k->paintArea->setPreviousFramesOnionSkinCount(n);
}

// NEXT
void KTViewDocument::disableNextOnionSkin()
{
	k->paintArea->setNextFramesOnionSkinCount( 0 );
}

void KTViewDocument::oneNextOnionSkin()
{
	k->paintArea->setNextFramesOnionSkinCount( 1 );
}

void KTViewDocument::twoNextOnionSkin()
{
	k->paintArea->setNextFramesOnionSkinCount( 2 );
}

void KTViewDocument::threeNextOnionSkin()
{
	k->paintArea->setNextFramesOnionSkinCount( 3 );
}


void KTViewDocument::setNextOnionSkin(int n)
{
	k->paintArea->setNextFramesOnionSkinCount( n );
}

void KTViewDocument::toggleShowGrid()
{
	k->paintArea->setDrawGrid( !k->paintArea->drawGrid() );
}

// void KTViewDocument::setScene(KTScene* scene)
// {
// 	setWindowTitle( d->title + " - " + scene->sceneName() );
// 	k->paintArea->setScene(  scene );
// }

void KTViewDocument::setZoomFactor(int /*percent*/)
{
	k->zoomFactorSpin->blockSignals(true);
	// k->paintArea->setZoomFactor((float) porcent/100);
	k->zoomFactorSpin->blockSignals(false);
}

void KTViewDocument::scaleRuler(double factor)
{
#if 0
	double sep = factor * k->verticalRuler->scaleFactor();
	k->verticalRuler->scale(sep);
	k->horizontalRuler->scale(sep);
#endif
}

void KTViewDocument::changeRulerOrigin(const QPointF &zero)
{
	k->verticalRuler->setZeroAt(zero);
	k->horizontalRuler->setZeroAt(zero);
}

/*
 void KTViewDocument::configure()
 {
 	KTPaintAreaConfig properties;
 	
 	if ( properties.exec() != QDialog::Rejected )
 	{
 		KTPaintAreaProperties areaProperties;
 		
 		areaProperties.gridColor = properties.gridColor();
 		areaProperties.backgroundColor = properties.backgroundColor();
 		areaProperties.onionSkinColor = properties.onionSkinColor();
 		areaProperties.onionSkinBackground = properties.onionSkinBackground();
 		areaProperties.gridSeparation = properties.gridSeparation();
 		
		k->paintArea->setProperties(areaProperties);
 	}
 }

 void KTViewDocument::closeEvent(QCloseEvent *e)
 {
 	
 }
*/

QSize KTViewDocument::sizeHint() const
{
	QSize size(parentWidget()->size());
	return size.expandedTo(QApplication::globalStrut());
}


KTBrushManager *KTViewDocument::brushManager() const
{
	return k->paintArea->brushManager();
}


KTPaintAreaCommand *KTViewDocument::createCommand(const KTPaintAreaEvent *event)
{
	KTPaintAreaCommand *command = new KTPaintAreaCommand(k->paintArea, event);
	
	return command;
}
