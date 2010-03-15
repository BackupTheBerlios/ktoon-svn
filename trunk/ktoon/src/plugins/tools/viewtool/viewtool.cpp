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

    stop = false;
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
    KAction *action1 = new KAction(QIcon(THEME_DIR + "icons/magnifying.png"), tr("Zoom"), this);
    action1->setShortcut(QKeySequence(tr("")));
    action1->setCursor(QCursor(THEME_DIR + "cursors/magnifying.png"));
    
    m_actions.insert(tr("Zoom"), action1);
    
    KAction *action2 = new KAction(QIcon(THEME_DIR + "icons/hand.png"), tr("Hand"), this);
    m_actions.insert(tr("Hand"), action2);
}

void ViewTool::press(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(brushManager);
    m_rect = new QGraphicsRectItem(QRectF(input->pos(), QSize(0,0)));
    m_rect->setPen(QPen(Qt::red, 1, Qt::SolidLine));
    scene->addItem(m_rect);

    foreach (QGraphicsView * view, scene->views()) {
             if (currentTool() == tr("Zoom")) {
                 if (!m_configurator->zoomIn()) {
                     QPointF point1(view->mapToScene(QPoint(0,0)));
                     QPointF point2(view->mapToScene(QPoint(view->width(),view->height())));
                     int width = point2.x() - point1.x();
                     if (width < 888)
                         //view->scale(1/1.5, 1/1.5);
                         view->scale(m_configurator->getFactor(), m_configurator->getFactor());
                     if (width > 50)
                         stop = false;
                 }
             }
    }
}

void ViewTool::move(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(brushManager);
    foreach (QGraphicsView * view, scene->views()) {
             if (currentTool() == tr("Zoom")) {
                 view->setDragMode(QGraphicsView::NoDrag);
             } else if (currentTool() == tr("Hand")) {
                        view->setDragMode(QGraphicsView::ScrollHandDrag);
             }
    }
    
    if (currentTool() == tr("Zoom")) {

        if (m_configurator->zoomIn()) {
            if (!stop) {
                QRectF rect = m_rect->rect();
                rect.setBottomLeft(input->pos());
                m_rect->setRect(rect);
                rect = rect.normalized(); 

                if (rect.height() > 10 && rect.width() > 10)
                    m_rect->setPen(QPen(Qt::gray, 0.5, Qt::SolidLine));
                else
                    m_rect->setPen(QPen(Qt::red, 1, Qt::SolidLine));
            }
        } 

    } else if (currentTool() == tr("Hand")) {
        m_scene = scene;
    }
}

void ViewTool::release(const KTInputDeviceInformation *input, KTBrushManager *brushManager, KTGraphicsScene *scene)
{
    Q_UNUSED(brushManager);
    
    foreach (QGraphicsView * view, scene->views()) {
             if (currentTool() == tr("Zoom")) {
                 if (m_configurator->zoomIn()) {
                     if (!stop) {
                         QRectF rect = m_rect->rect();
                         if (input->button() == Qt::LeftButton) {
                             if (rect.normalized().height() > 10 && rect.normalized().width() > 10) {
                                 view->fitInView(rect, Qt::KeepAspectRatio);
                                 QPointF point1(view->mapToScene(QPoint(0,0)));
                                 QPointF point2(view->mapToScene(QPoint(view->width(),view->height())));
                                 int width = point2.x() - point1.x(); 
                                 if (width < 50)
                                     stop = true;
                             }
                         } 
                     }
                 } 
             }
             delete m_rect;
             m_rect = 0;
    }
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
        KCONFIG->beginGroup("Zoom tool");
        KCONFIG->setValue("zoomoutfactor", m_configurator->getFactor());
    }
}

Q_EXPORT_PLUGIN2(kt_view, ViewTool)
