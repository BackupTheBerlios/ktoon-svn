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
#include "viewtool.h"

#include <QKeySequence>
#include <QDebug>
#include <QImage>
#include <QPaintDevice>
#include <QGraphicsView>

#include <kcore/kglobal.h>
#include <kcore/kdebug.h>
#include <kcore/kconfig.h>

#include "ktrectitem.h"
#include "ktellipseitem.h"
#include "ktlineitem.h"

#include "ktscene.h"

#include "ktinputdeviceinformation.h"
#include "ktgraphicsscene.h"

ViewTool::ViewTool() : m_rect(0), m_scene(0), m_configurator(0)
{
    KINIT;

    setupActions();
}

ViewTool::~ViewTool()
{
    KEND;
}

QStringList ViewTool::keys() const
{
    return QStringList() << tr("Zoom") << tr("Hand");
}

void ViewTool::setupActions()
{
    KAction *zoomAction = new KAction(QIcon(THEME_DIR + "icons/magnifying.png"), tr("Zoom"), this);
    zoomAction->setShortcut(QKeySequence(tr("z")));
    zoomAction->setCursor(QCursor(THEME_DIR + "cursors/magnifying.png"));
    
    m_actions.insert(tr("Zoom"), zoomAction);
    
    KAction *handAction = new KAction(QIcon(THEME_DIR + "icons/hand.png"), tr("Hand"), this);
    handAction->setShortcut(QKeySequence(tr("h")));
    handAction->setCursor(QCursor(THEME_DIR + "cursors/hand.png"));

    m_actions.insert(tr("Hand"), handAction);
}

void ViewTool::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(brushManager);
}

void ViewTool::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(brushManager);

    foreach (QGraphicsView * view, scene->views()) {
             if (currentTool() == tr("Zoom"))
                 view->setDragMode(QGraphicsView::NoDrag);
             else if (currentTool() == tr("Hand"))
                      view->setDragMode(QGraphicsView::ScrollHandDrag);
    }

    /*
    SQA: Check this code and try to make rect-selection-zoom work
    
    if (currentTool() == tr("Zoom")) {

        if (input->button() == Qt::LeftButton) {
            int xMouse = input->pos().x();
            int yMouse = input->pos().y();

            QRectF rect = m_rect->rect();
            int xInit = m_rect->pos().x(); 
            int yInit = m_rect->pos().y();

            if (xMouse >= xInit) {
                if (yMouse >= yInit)
                    rect.setBottomRight(input->pos()); 
                else
                    rect.setTopRight(input->pos());
            } else {
                if (yMouse >= yInit)
                    rect.setBottomLeft(input->pos());
                else
                    rect.setTopLeft(input->pos());
            }

            m_rect->setRect(rect);
            rect = rect.normalized(); 

            if (rect.height() > 10 && rect.width() > 10)
                m_rect->setPen(QPen(Qt::gray, 0.5, Qt::SolidLine));
            else
                m_rect->setPen(QPen(Qt::red, 1, Qt::SolidLine));
        }

    } else if (currentTool() == tr("Hand")) {
               m_scene = scene;
    } 

    */
}

void ViewTool::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(brushManager);

    if (currentTool() == tr("Zoom")) { 
        foreach (QGraphicsView * view, scene->views()) {
                 if (input->button() == Qt::LeftButton) {
                     view->centerOn(input->pos());
                     view->scale(1 + m_configurator->getFactor(), 1 + m_configurator->getFactor());
                 } else {
                     if (input->button() == Qt::RightButton) {
                         view->centerOn(input->pos());
                         view->scale(1 - m_configurator->getFactor(), 1 - m_configurator->getFactor());
                     } 
                 }
        }
    }

   /*  
    if (input->button() == Qt::LeftButton) {
    
        foreach (QGraphicsView *view, scene->views()) {
                 if (currentTool() == tr("Zoom")) {
                     QRectF rect = m_rect->rect();
                     if (rect.normalized().height() > 10 && rect.normalized().width() > 10) {
                         view->fitInView(rect, Qt::KeepAspectRatio);
                         //QPointF point1(view->mapToScene(QPoint(0,0)));
                         //QPointF point2(view->mapToScene(QPoint(view->width(),view->height())));
                     }
                 }
         }
         delete m_rect;
         m_rect = 0;

    }
    */
}

QMap<QString, KAction *> ViewTool::actions() const
{
    return m_actions;
}

int ViewTool::toolType() const
{
    return View;
}

QWidget *ViewTool::configurator()
{
    if (! m_configurator)
        m_configurator = new ZoomConfigurator;

    return m_configurator;
}

void ViewTool::aboutToChangeScene(KTGraphicsScene *)
{
}

void ViewTool::aboutToChangeTool()
{
    if (currentTool() == tr("Hand")) {
       if (m_scene) {
           foreach (QGraphicsView * view, m_scene->views())
                    view->setDragMode(QGraphicsView::NoDrag);
       }
    }
}

void ViewTool::saveConfig()
{
    if (m_configurator) {
        KCONFIG->beginGroup("ZoomTool");
        KCONFIG->setValue("zoomFactor", m_configurator->getFactor());
    }
}

Q_EXPORT_PLUGIN2(kt_view, ViewTool)
