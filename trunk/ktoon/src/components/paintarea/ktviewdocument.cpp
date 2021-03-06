/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "ktapplication.h"
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
#include "ktgraphicsscene.h"

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
    KTToolPlugin *currentTool;
    KTPaintAreaStatus *status;

    QTimer *timer;
};

KTViewDocument::KTViewDocument(KTProject *project, QWidget *parent) : QMainWindow(parent), k(new Private)
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    setWindowIcon(QPixmap(THEME_DIR + "icons/illustration_mode.png"));

    k->currentTool = 0;
    k->actionManager = new KActionManager(this);

    QFrame *frame = new QFrame(this, Qt::FramelessWindowHint);
    QGridLayout *layout = new QGridLayout(frame);

    k->paintArea = new KTPaintArea(project, frame);
    connect(k->paintArea, SIGNAL(scaled(double)), this, SLOT(scaleRuler(double)));

    setCentralWidget(frame);

    layout->addWidget(k->paintArea, 1, 1);
    k->horizontalRuler = new KTDocumentRuler(Qt::Horizontal);
    k->verticalRuler = new KTDocumentRuler(Qt::Vertical);

    layout->addWidget(k->horizontalRuler, 0, 1);
    layout->addWidget(k->verticalRuler, 1, 0);

    KToon::RenderType renderType = KToon::RenderType(KCONFIG->value("RenderType").toInt()); 

    switch (renderType) {
            case KToon::OpenGL:
                 k->paintArea->setUseOpenGL(true);
                 break;

            case KToon::Native:
                 k->paintArea->setUseOpenGL(false);
                 break;

            default:
                 #ifdef K_DEBUG
                        kWarning() << "Unsopported render, switching to native!";
                 #endif
                 k->paintArea->setUseOpenGL(false);
            break;
    }
    
    connect(k->paintArea, SIGNAL(cursorPosition(const QPointF &)),  this,  SLOT(showPos(const QPointF &)));
    
    connect(k->paintArea, SIGNAL(cursorPosition(const QPointF &)), k->verticalRuler, 
                          SLOT(movePointers(const QPointF&)));
    
    connect(k->paintArea, SIGNAL(cursorPosition(const QPointF &)), k->horizontalRuler, 
                          SLOT(movePointers(const QPointF&)));

    connect(k->paintArea, SIGNAL(changedZero(const QPointF&)), this, SLOT(changeRulerOrigin(const QPointF&)));

    connect(k->paintArea, SIGNAL(requestTriggered(const KTProjectRequest *)), this, 
                          SIGNAL(requestTriggered(const KTProjectRequest *)));

    setupDrawActions();

    //k->configurationArea = new KTConfigurationArea(this);
    //addDockWidget(Qt::RightDockWidgetArea, k->configurationArea);
    //k->configurationArea->close();
    
    createToolBar();
    createTools();
    
    k->status = new KTPaintAreaStatus(this);
    setStatusBar(k->status);

    // SQA: Verify if this code is doing something
    //connect(k->paintArea->brushManager(), SIGNAL(brushChanged(const QBrush&)), k->status, 
    //        SLOT(setBrush(const QBrush &)));

    connect(k->paintArea->brushManager(), SIGNAL(penChanged(const QPen&)), k->status, 
            SLOT(setPen(const QPen &)));

    QTimer::singleShot(1000, this, SLOT(loadPlugins()));

    updateTimer();
}

KTViewDocument::~KTViewDocument()
{
    if (k->currentTool)
        k->currentTool->saveConfig();

    delete k->configurationArea;
    delete k->timer;
    delete k;
}

void KTViewDocument::setAntialiasing(bool useIt)
{
    k->paintArea->setAntialiasing(useIt);
}

void KTViewDocument::setOpenGL(bool useIt)
{
    k->paintArea->setUseOpenGL(useIt);
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
    QString message =  "X: " +  QString::number(p.x()) + " Y: " + QString::number(p.y());
    emit sendToStatus(message) ;
}

void KTViewDocument::setupDrawActions()
{
    KAction *showGrid = new KAction(QPixmap(THEME_DIR + "icons/subgrid.png"), 
                                    tr("Show grid"), QKeySequence(tr("#")),
                                    this, SLOT(toggleShowGrid()), k->actionManager, "show_grid");
    showGrid->setCheckable(true);

    KAction *copy = new KAction(QPixmap(THEME_DIR + "icons/copy.png"), 
                                tr("Copy"), QKeySequence(tr("Ctrl+C")),
                                k->paintArea, SLOT(copyItems()), k->actionManager, "copy");
    copy->setStatusTip(tr("Copies the selection and puts it onto the clipboard"));

    KAction *paste = new KAction(QPixmap(THEME_DIR + "icons/paste.png"), 
                                 tr("Paste"), QKeySequence(tr("Ctrl+V")),
                                 k->paintArea, SLOT(pasteItems()), k->actionManager, "paste");
    paste->setStatusTip(tr("Pastes the clipboard into the current document"));

    KAction *cut = new KAction(QPixmap(THEME_DIR + "icons/cut.png"), 
                               tr("Cut"), QKeySequence(tr("Ctrl+X")),
                               k->paintArea, SLOT(cutItems()),k->actionManager, "cut");
    cut->setStatusTip(tr("Cuts the selected items"));

    KAction *del = new KAction(QPixmap(THEME_DIR + "icons/delete.png"), tr("Delete"), 
                               QKeySequence(Qt::Key_Delete), k->paintArea, SLOT(deleteItems()), 
                               k->actionManager, "delete");
    
    del->setStatusTip(tr("Deletes the selected object"));

    KAction *group = new KAction(QPixmap(THEME_DIR + "icons/group.png"), tr("&Group"),   
                                 QKeySequence(tr("Ctrl+G")), k->paintArea, SLOT(groupItems()), 
                                 k->actionManager, "group");

    group->setStatusTip(tr("Group the selected objects into a single one"));
    group->setDisabled(true);

    KAction *ungroup = new KAction(QPixmap(THEME_DIR + "icons/ungroup.png"), tr("&Ungroup"), 
                                    QKeySequence(tr("Ctrl+Shift+G")) , k->paintArea, SLOT(ungroupItems()), 
                                    k->actionManager, "ungroup");
    ungroup->setDisabled(true);

    ungroup->setStatusTip(tr("Ungroups the selected object"));
}

void KTViewDocument::createTools()
{
    k->toolbar = new QToolBar(tr("Draw tools"), this);
    k->toolbar->setIconSize(QSize(16,16));
    addToolBar(Qt::LeftToolBarArea, k->toolbar);

    connect(k->toolbar, SIGNAL(actionTriggered(QAction *)), this, SLOT(selectToolFromMenu(QAction *)));

    // Brushes menu
    k->brushesMenu = new QMenu(tr("Brushes"), k->toolbar);
    k->brushesMenu->setIcon(QPixmap(THEME_DIR + "icons/brush.png"));
    connect(k->brushesMenu, SIGNAL(triggered (QAction *)), this, SLOT(selectToolFromMenu(QAction*)));

    k->toolbar->addAction(k->brushesMenu->menuAction());

    // Selection menu
    k->selectionMenu = new QMenu(tr("Selection"), k->toolbar);
    k->selectionMenu->setIcon(QPixmap(THEME_DIR + "icons/selection.png"));
    connect(k->selectionMenu, SIGNAL(triggered(QAction*)), this, SLOT(selectToolFromMenu(QAction*)));

    k->toolbar->addAction(k->selectionMenu->menuAction());

    // Fill menu
    k->fillMenu = new QMenu(tr("Fill"), k->toolbar);
    k->fillMenu->setIcon(QPixmap(THEME_DIR + "icons/fillcolor.png"));
    connect(k->fillMenu, SIGNAL(triggered(QAction *)), this, SLOT(selectToolFromMenu(QAction*)));

    k->toolbar->addAction(k->fillMenu->menuAction());

    // View menu
    k->viewToolMenu = new QMenu(tr("View"), k->toolbar);
    k->viewToolMenu->setIcon(QPixmap(THEME_DIR + "icons/magnifying.png"));
    connect(k->fillMenu, SIGNAL(triggered(QAction *)), this, SLOT(selectToolFromMenu(QAction*)));

    k->toolbar->addAction(k->viewToolMenu->menuAction());
}

void KTViewDocument::loadPlugins()
{
    QList<KAction*> brushTools;

    foreach (QObject *plugin, KTPluginManager::instance()->tools()) {

             KTToolPlugin *tool = qobject_cast<KTToolPlugin *>(plugin);

             QStringList::iterator it;
             QStringList keys = tool->keys();

             for (it = keys.begin(); it != keys.end(); ++it) {
                  #ifdef K_DEBUG
                         kDebug("plugins") << "*** Tool Loaded: " << *it;
                  #endif

                  KAction *action = tool->actions()[*it];
                  action->setIconVisibleInMenu(true);

                  if (action) {
                      connect(action, SIGNAL(triggered()), this, SLOT(selectTool()));
                      action->setParent(plugin);
                      QString toolStr = action->text();

                      switch (tool->toolType()) {
                              case KTToolInterface::Brush:
                                 {

                                   if (toolStr.compare(tr("Pencil")) == 0)
                                       brushTools.insert(0, action);
                                       k->brushesMenu->setDefaultAction(action);

                                   if (toolStr.compare(tr("Eraser")) == 0) {
                                       action->setDisabled(true);
                                       brushTools.insert(1, action);
                                   }

                                   if (toolStr.compare(tr("Polyline")) == 0)
                                       brushTools.insert(2, action);

                                   if (toolStr.compare(tr("Line")) == 0)
                                       brushTools.insert(3, action);

                                   if (toolStr.compare(tr("Rectangle")) == 0)
                                       brushTools.insert(4, action);

                                   if (toolStr.compare(tr("Ellipse")) == 0)
                                       brushTools.insert(5, action);

                                   if (toolStr.compare(tr("Text")) == 0)
                                       brushTools.insert(6, action);

                                   if (toolStr.compare(tr("Motion Tween")) == 0) {
                                       action->setDisabled(true);
                                       brushTools.insert(7, action);
                                   }

                                 }
                                 break;
                              case KTToolInterface::Selection:
                                 {
                                   k->selectionMenu->addAction(action);
                                   if (toolStr.compare(tr("Object Selection")) == 0)
                                       k->selectionMenu->setDefaultAction(action);
                                 }
                                 break;
                              case KTToolInterface::Fill:
                                 {
                                   k->fillMenu->addAction(action);
                                   if (toolStr.compare(tr("Internal fill")) == 0)
                                      k->fillMenu->setDefaultAction(action);
                                 }
                                 break;
                               case KTToolInterface::View:
                                 {
                                   k->viewToolMenu->addAction(action);
                                   if (toolStr.compare(tr("Zoom")) == 0)
                                       k->viewToolMenu->setDefaultAction(action);

                                 }
                                 break;
                               default:
                                 break;
                      }
                  }
             }
    } // end foreach

    for (int i = 0; i < brushTools.size(); ++i) 
         k->brushesMenu->addAction(brushTools.at(i));

    foreach (QObject *plugin, KTPluginManager::instance()->filters()) {
             AFilterInterface *filter = qobject_cast<AFilterInterface *>(plugin);
             QStringList::iterator it;
             QStringList keys = filter->keys();

             for (it = keys.begin(); it != keys.end(); ++it) {
                  #ifdef K_DEBUG
                         kDebug("plugins") << "*** Filter Loaded: " << *it;
                  #endif

                  KAction *act = filter->actions()[*it];
                  if (act) {
                      connect(act, SIGNAL(triggered()), this, SLOT(applyFilter()));
                      k->filterMenu->addAction(act);
                  }
             }
    }

    KAction *pencil = brushTools.at(0);
    pencil->trigger();
    brushTools.clear();
}

void KTViewDocument::selectTool()
{
    #ifdef K_DEBUG
           K_FUNCINFO;
    #endif

    if (k->currentTool) {
        k->currentTool->saveConfig();
        QWidget *toolConfigurator = k->currentTool->configurator();
        if (toolConfigurator)
            k->configurationArea->close();
    }

    KAction *action = qobject_cast<KAction *>(sender());

    if (action) {
        KTToolPlugin *tool = qobject_cast<KTToolPlugin *>(action->parent());
        k->currentTool = tool; 
        QString toolStr = tr("%1").arg(action->text());
        k->paintArea->setCurrentTool(toolStr);

        if (!action->icon().isNull())
            k->status->updateTool(toolStr, action->icon().pixmap(15, 15));

        int minWidth = 0;

        switch (tool->toolType()) {

                case KTToolInterface::Brush: 
                     if (toolStr.compare(tr("Pencil"))==0) {
                         minWidth = 130;
                     } else if (toolStr.compare(tr("Text"))==0) {
                                minWidth = 350;
                     } else if (toolStr.compare(tr("Motion Tween"))==0) {
                                minWidth = 140;
                     } 

                     k->brushesMenu->setDefaultAction(action);
                     k->brushesMenu->setActiveAction(action);
                     if (!action->icon().isNull())
                         k->brushesMenu->menuAction()->setIcon(action->icon());
                     break;

                case KTToolInterface::Fill:
                     k->fillMenu->setDefaultAction(action);
                     k->fillMenu->setActiveAction(action);
                     if (!action->icon().isNull())
                         k->fillMenu->menuAction()->setIcon(action->icon());
                     break;

                case KTToolInterface::Selection:
                     k->selectionMenu->setDefaultAction(action);
                     k->selectionMenu->setActiveAction(action);
                     if (!action->icon().isNull())
                         k->selectionMenu->menuAction()->setIcon(action->icon());
                     if (toolStr.compare(tr("Object Selection"))==0) {
                         minWidth = 130;
                         connect(k->paintArea, SIGNAL(itemAddedOnSelection(KTGraphicsScene *)), 
                                 tool, SLOT(updateItems(KTGraphicsScene *)));
                     } 
                     break;

                case KTToolInterface::View:
                     k->viewToolMenu->setDefaultAction(action);
                     k->viewToolMenu->setActiveAction(action);
                     if (!action->icon().isNull())
                         k->viewToolMenu->menuAction()->setIcon(action->icon());
                     if (toolStr.compare(tr("Zoom"))==0)
                         minWidth = 130;
                     break;
        }

        QWidget *toolConfigurator = tool->configurator();

        if (toolConfigurator) {
            k->configurationArea = new KTConfigurationArea(this);
            k->configurationArea->setConfigurator(toolConfigurator, minWidth);
            addDockWidget(Qt::RightDockWidgetArea, k->configurationArea);
            toolConfigurator->show();
            if (!k->configurationArea->isVisible())
                k->configurationArea->show();
        } else {
            if (k->configurationArea->isVisible())
                k->configurationArea->close();
        }

        tool->setCurrentTool(toolStr);
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

    if (menu) {
        KAction *tool = qobject_cast<KAction *>(menu->activeAction());

        if (tool) {
            tool->trigger(); // this call selectTool()
        } else {
            tool = qobject_cast<KAction *>(menu->defaultAction());
            if (tool)
                tool->trigger();
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

    if (action) {
        /*
        SQA issue: Code to check
        AFilterInterface *aFilter = qobject_cast<AFilterInterface *>(action->parent());
        QString filter = action->text();
        
        KTFrame *frame = k->paintArea->currentFrame();
        if (frame) {
            aFilter->filter(action->text(), frame->components());
            k->paintArea->redrawAll();
        }
        */
    }
}

void KTViewDocument::updateZoomFactor(double f)
{
    k->zoomFactorSpin->blockSignals(true);
    k->zoomFactorSpin->setValue(f*100);
    k->zoomFactorSpin->blockSignals(false);
}

void KTViewDocument::createToolBar()
{
    k->barGrid = new QToolBar(tr("Paint area actions"), this);
    k->barGrid->setIconSize(QSize(16,16));

    addToolBar(k->barGrid);

    k->barGrid->addAction(kApp->findGlobalAction("undo"));
    k->barGrid->addAction(kApp->findGlobalAction("redo"));

    k->barGrid->addSeparator();

    k->barGrid->addAction(k->actionManager->find("copy"));
    k->barGrid->addAction(k->actionManager->find("paste"));
    k->barGrid->addAction(k->actionManager->find("cut"));
    k->barGrid->addAction(k->actionManager->find("delete"));
    k->barGrid->addSeparator();

    k->barGrid->addAction(k->actionManager->find("show_grid"));
    k->barGrid->addSeparator();

    k->barGrid->addAction(k->actionManager->find("group"));
    k->barGrid->addAction(k->actionManager->find("ungroup"));

    k->barGrid->addSeparator();

    KCONFIG->beginGroup("OnionParameters");
    int preview = KCONFIG->value("PreviewFrames", -1).toInt();
    int next = KCONFIG->value("NextFrames", -1).toInt();

    QSpinBox *prevOnionSkinSpin = new QSpinBox(this);
    prevOnionSkinSpin->setToolTip(tr("Preview Frames"));
    connect(prevOnionSkinSpin, SIGNAL(valueChanged(int)), this, SLOT(setPreviousOnionSkin(int)));

    if (preview > 0)
        prevOnionSkinSpin->setValue(preview);
    else
        prevOnionSkinSpin->setValue(1);

    k->barGrid->addWidget(prevOnionSkinSpin);

    QLabel *layers = new QLabel("");
    QPixmap pix(THEME_DIR + "icons/layer.png");
    layers->setToolTip(tr("Onion Skin"));
    layers->setPixmap(pix);
    layers->setMaximumSize(20, 20);
    k->barGrid->addWidget(layers);

    QSpinBox *nextOnionSkinSpin = new QSpinBox(this);
    nextOnionSkinSpin->setToolTip(tr("Next Frames"));
    connect(nextOnionSkinSpin, SIGNAL(valueChanged (int)), this, SLOT(setNextOnionSkin(int)));

    if (next > 0)
        nextOnionSkinSpin->setValue(next);
    else
        nextOnionSkinSpin->setValue(1);

    k->barGrid->addWidget(nextOnionSkinSpin);
}

void KTViewDocument::closeArea()
{
    if (k->configurationArea->isVisible())
        k->configurationArea->close();

    k->paintArea->setScene(0);
    close();
}

void KTViewDocument::undo()
{
    puts("Adding undo support");
}

void KTViewDocument::redo()
{
    puts("Adding redo support");
}

void KTViewDocument::setCursor(const QCursor &)
{
 /*
    k->paintArea->setCursor(c);
 */
}

void KTViewDocument::setPreviousOnionSkin(int level)
{
    KCONFIG->beginGroup("OnionParameters");
    KCONFIG->setValue("PreviewFrames", level);

    k->paintArea->setPreviousFramesOnionSkinCount(level);
}

void KTViewDocument::setNextOnionSkin(int level)
{
    KCONFIG->beginGroup("OnionParameters");
    KCONFIG->setValue("NextFrames", level);

    k->paintArea->setNextFramesOnionSkinCount(level);
}

void KTViewDocument::toggleShowGrid()
{
    k->paintArea->setDrawGrid(!k->paintArea->drawGrid());
}

void KTViewDocument::setZoomFactor(int /*percent*/)
{
    k->zoomFactorSpin->blockSignals(true);
    k->zoomFactorSpin->blockSignals(false);
}

void KTViewDocument::scaleRuler(double factor)
{
    Q_UNUSED(factor);
    /*
    double sep = factor * k->verticalRuler->scaleFactor();
    k->verticalRuler->scale(sep);
    k->horizontalRuler->scale(sep);
    */
}

void KTViewDocument::changeRulerOrigin(const QPointF &zero)
{
    k->verticalRuler->setZeroAt(zero);
    k->horizontalRuler->setZeroAt(zero);
}

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

void KTViewDocument::updatePaintArea()
{
    k->paintArea->updatePaintArea(); 
}

void KTViewDocument::callAutoSave()
{
    emit autoSave();
}

void KTViewDocument::updateTimer()
{
    KCONFIG->beginGroup("General");
    int autoSave = KCONFIG->value("AutoSave").toInt();

    k->timer = new QTimer(this);

    if (autoSave > 0) {
        autoSave = autoSave*60000;
        connect(k->timer, SIGNAL(timeout()), this, SLOT(callAutoSave()));
        k->timer->start(autoSave);
    }
}

