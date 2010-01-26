/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrosx@toonka.com>            *
 *   Copyright (C) 2006 by David Cuadrado <krawek@toonka.com>              *
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

#include "ktpaintareabase.h"

#include <QGraphicsScene>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsRectItem>
#include <QPolygon>
#include <QApplication>
#include <QTimer>
#include <QStyleOptionGraphicsItem>
#include <QClipboard>

#include "ktbrushmanager.h"
#include "ktinputdeviceinformation.h"
#include "kttextitem.h"
#include "ktpaintarearotator.h"

#include "ktgraphicsscene.h"
#include "ktgraphicalgorithm.h"

// KOM
#include <kcore/kconfig.h>
#include <kcore/kdebug.h>
#include <kgui/kapplication.h>
#include <kgui/kosd.h>

#include <cmath>

#ifdef QT_OPENGL_LIB

#include <QGLWidget>
#include <QGLFramebufferObject>

/**
 * This class defines the global paint area behavoir in the Ilustration Mode.
 * Here is where general events about the paint area are processed.
 * @author David Cuadrado & Jorge Cuadrado
*/

class GLDevice : public QGLWidget
{
    public:
        GLDevice() : QGLWidget()
                {
                     makeCurrent();
                }
        ~GLDevice() {};

    protected:
        void initializeGL()
                {
                     glDisable(GL_DEPTH_TEST);
                }
};

#endif

struct KTPaintAreaBase::Private
{
    QGraphicsRectItem *grid;
    QRectF drawingRect;

    bool drawGrid;
    double angle;

    KTPaintAreaRotator *rotator;
    QStringList copiesXml;
    KTGraphicsScene *scene;
};

KTPaintAreaBase::KTPaintAreaBase(QWidget * parent) : QGraphicsView(parent), k(new Private)
{
    k->scene = new KTGraphicsScene();
    k->grid = 0;

    k->drawGrid = false;
    k->angle = 0;

    k->rotator = new KTPaintAreaRotator(this, this);
    k->drawingRect = QRectF(QPointF(0,0), QSizeF(500, 400)); // FIXME: configurable

    k->scene->setSceneRect(k->drawingRect);
    setScene(k->scene);
    centerDrawingArea();
    setUseOpenGL(false);
    setInteractive(true);

    restoreState();
}

void KTPaintAreaBase::saveState()
{
    KConfig *config = kApp->config("PaintArea");
    config->setValue("RenderHints", int(renderHints()));
}

void KTPaintAreaBase::restoreState()
{
    KConfig *config = kApp->config("PaintArea");

    int renderHints = config->value("RenderHints", int(this->renderHints())).toInt();
    setRenderHints(QPainter::RenderHints(renderHints));
}

KTPaintAreaBase::~KTPaintAreaBase()
{
    saveState();
    delete k;
}

void KTPaintAreaBase::setAntialiasing(bool use)
{
#ifdef QT_OPENGL_LIB
    if (QGLWidget *gl = dynamic_cast<QGLWidget *>(viewport())) {
        gl->setUpdatesEnabled(false); // works better
        // gl->setFormat(QGLFormat(QGL::SampleBuffers | QGL::HasOverlay /*| QGL::DirectRendering 
        // | QGL::AccumBuffer | QGL::Rgba */));
        gl->setFormat(QGLFormat(QGL::SampleBuffers | QGL::HasOverlay));
        gl->setUpdatesEnabled(true);
    }
#endif

    setRenderHint(QPainter::Antialiasing, use);
    setRenderHint(QPainter::TextAntialiasing, use);
}

void KTPaintAreaBase::setUseOpenGL(bool opengl)
{
    K_FUNCINFO << opengl;

    QCursor cursor(Qt::ArrowCursor);
    if (viewport())
        cursor = viewport()->cursor();
	
#ifdef QT_OPENGL_LIB
        if (opengl) {
            setViewport(new GLDevice());
        } else {
                // setViewport(new KTImageDevice());
        }
#else
        Q_UNUSED(opengl);
        kWarning() << tr("OpenGL isn't supported");
#endif

    // to restore the cursor.
    if (viewport()) {
        viewport()->setCursor(cursor);
        viewport()->setAcceptDrops(true);
    }
}

void KTPaintAreaBase::setDrawGrid(bool draw)
{
    k->drawGrid = draw;
    // resetCachedContent();
    viewport()->update();
}

void KTPaintAreaBase::setTool(KTToolPlugin *tool )
{
    if (!scene()) 
        return;

    if (tool)
        disconnect(tool,SIGNAL(requested(const KTProjectRequest *)), this, 
                   SIGNAL(requestTriggered(const KTProjectRequest* )));

    k->scene->setTool(tool);
    connect(tool,SIGNAL(requested(const KTProjectRequest *)), this, 
            SIGNAL(requestTriggered( const KTProjectRequest* )));
}

bool KTPaintAreaBase::drawGrid() const
{
    return k->drawGrid;
}

void KTPaintAreaBase::mousePressEvent ( QMouseEvent * event )
{
    if (!canPaint()) 
        return;

    k->scene->aboutToMousePress();
    QGraphicsView::mousePressEvent(event);
}

void KTPaintAreaBase::mouseMoveEvent( QMouseEvent * event )
{
    if (!canPaint()) 
        return;

    // Rotate
    if (!k->scene->isDrawing() && event->buttons() == Qt::LeftButton 
        && (event->modifiers () == (Qt::ShiftModifier | Qt::ControlModifier))) {
        setUpdatesEnabled(false);
        setDragMode(QGraphicsView::NoDrag);
	QPointF p1 = event->pos();
        QPointF p2 = k->drawingRect.center();
        k->rotator->rotateTo( (int)(-(180*KTGraphicalAlgorithm::angleForPos(p1,p2))/M_PI) );
        setUpdatesEnabled(true);
    } else {
        QGraphicsView::mouseMoveEvent(event);
        if (!k->scene->mouseGrabberItem() && k->scene->isDrawing()) { // HACK
            QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseMove);
            mouseEvent.setWidget(viewport());
            mouseEvent.setScenePos(mapToScene(event->pos()));
            mouseEvent.setScreenPos(event->globalPos());
            mouseEvent.setButtons(event->buttons());
            mouseEvent.setButton(event->button());
            mouseEvent.setModifiers(event->modifiers());
            mouseEvent.setAccepted(false);
            // QApplication::sendEvent(k->scene, &mouseEvent);
            k->scene->mouseMoved(&mouseEvent);
         }
    }

    emit cursorPosition(mapToScene(event->pos()));
}

void KTPaintAreaBase::mouseReleaseEvent(QMouseEvent *event)
{
    QGraphicsView::mouseReleaseEvent(event);

    if (! k->scene->mouseGrabberItem() && k->scene->isDrawing()) { // HACK
        QGraphicsSceneMouseEvent mouseEvent(QEvent::GraphicsSceneMouseRelease);
        mouseEvent.setWidget(viewport());
        mouseEvent.setScenePos(mapToScene(event->pos()));
        mouseEvent.setScreenPos(event->globalPos());
        mouseEvent.setButtons(event->buttons());
        mouseEvent.setButton(event->button());
        mouseEvent.setModifiers(event->modifiers());
        mouseEvent.setAccepted(false);
        // QApplication::sendEvent(k->scene, &mouseEvent);
        k->scene->mouseReleased(&mouseEvent);
    }
}

void KTPaintAreaBase::tabletEvent(QTabletEvent * event)
{
    QGraphicsView::tabletEvent(event);
}

void KTPaintAreaBase::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsView::drawBackground(painter, rect);
    painter->save();

    bool hasAntialiasing = painter->renderHints() & QPainter::Antialiasing;

    painter->setRenderHint(QPainter::Antialiasing, false);
    painter->setPen(QPen(QColor(0,0,0,180), 2));
    painter->fillRect(k->drawingRect, Qt::white);
    painter->drawRect(k->drawingRect);

    emit changedZero(painter->matrix().map(QPointF(0,0)));

    // if enabled draw grid
    if (k->drawGrid) {
        int sx = (int)painter->matrix().m11();
        int sy = (int)painter->matrix().m22();
        painter->resetMatrix();
        painter->scale(sx, sy);
        painter->setPen(QPen(QColor(0,0,180, 50), 1));

        for (int i = 3; i < qMax(width(), height()); i+= 10) {
             painter->drawLine(i, 0, i, height());
             painter->drawLine(0, i, width(), i);
        }
    }

    painter->setRenderHint(QPainter::Antialiasing, hasAntialiasing);
    painter->restore();
}

void KTPaintAreaBase::drawForeground(QPainter *painter, const QRectF &rect)
{
    if (KTFrame *frame = k->scene->currentFrame()) {
        if (frame->isLocked()) {
            QString text = tr("Locked");
            QFont kfont(QFont("Arial", 20));
            QFontMetricsF fm(kfont);
            painter->setFont(kfont);

            painter->fillRect(rect, QColor(201,201,201, 200));
            painter->drawText(QPointF(k->scene->sceneRect().topRight().x() - fm.width(text) - 5, 
                              k->scene->sceneRect().topRight().y() + fm.height() - 5), text);
        }
    }
}

bool KTPaintAreaBase::canPaint() const
{
    if (k->scene) {
        KTFrame *frame = k->scene->currentFrame();

        if (frame)
            return !frame->isLocked();
    }

    return false;
}

void KTPaintAreaBase::centerDrawingArea()
{
    centerOn(k->drawingRect.center());
}

void KTPaintAreaBase::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier)
        scaleView(pow((double)2, -event->delta() / 240.0));
    else
        QGraphicsView::wheelEvent(event);
}

bool KTPaintAreaBase::viewportEvent(QEvent *e)
{
    bool ret = QGraphicsView::viewportEvent(e);

    if (e->type() == QEvent::Show) {
        if (k->scene->items().isEmpty()) {
            k->scene->drawCurrentPhotogram();
        }
    }

    return ret;
}

void KTPaintAreaBase::scaleView(qreal scaleFactor)
{
    qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 1, 1)).width();

    if (factor < 0.07 || factor > 100)
        return;

    scale(scaleFactor, scaleFactor);

    emit scaled(scaleFactor);
}

void KTPaintAreaBase::setRotationAngle(int angle)
{
    rotate(angle - k->angle);
    k->angle = angle;
}

KTBrushManager *KTPaintAreaBase::brushManager() const
{
    return k->scene->brushManager();
}

QRectF KTPaintAreaBase::drawingRect() const
{
    return k->drawingRect;
}

KTGraphicsScene *KTPaintAreaBase::graphicsScene() const
{
    return k->scene;
}
