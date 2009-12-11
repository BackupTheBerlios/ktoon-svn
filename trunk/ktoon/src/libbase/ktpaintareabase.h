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

#ifndef KTPAINTAREABASE_H
#define KTPAINTAREABASE_H

#include <QGraphicsView>
#include <kttoolplugin.h>

#include "ktglobal.h"

class QGraphicsRectItem;
class KTBrushManager;
class KTInputDeviceInformation;
class KTProject;
class KTPaintAreaRotator;

/**
 * This class provides an area to draw with some kind of brushes
 * @author Jorge Cuadrado \<kuadrosx@toonka.com\> - David Cuadrado \<krawek@toonka.com\>
*/
class KTOON_EXPORT KTPaintAreaBase : public QGraphicsView
{
    Q_OBJECT;
    public:
        KTPaintAreaBase(QWidget * parent = 0);
        ~KTPaintAreaBase();

        void setAntialiasing(bool use);
        void setUseOpenGL(bool opengl);
        void setDrawGrid(bool draw);
        void setTool(KTToolPlugin *tool);

        bool drawGrid() const;

        void scaleView(qreal scaleFactor);
        void setRotationAngle(int angle);

        KTBrushManager *brushManager() const;

        QRectF drawingRect() const;
        KTGraphicsScene *graphicsScene() const;

    private:
        virtual void saveState();
        virtual void restoreState();

    protected:
        virtual void mousePressEvent(QMouseEvent * event);
        virtual void mouseMoveEvent(QMouseEvent * event);
        virtual void mouseReleaseEvent(QMouseEvent *event);
        virtual void tabletEvent(QTabletEvent * event);
        virtual void wheelEvent(QWheelEvent *event);
        virtual bool viewportEvent(QEvent *e);

    signals:
        void cursorPosition(const QPointF &pos);
        void requestTriggered(const KTProjectRequest *event);
        void changedZero(const QPointF &zero);
        void scaled(double factor);

    public slots:
        void centerDrawingArea();

    protected:
        virtual void drawBackground(QPainter *painter, const QRectF &rect);
        virtual void drawForeground(QPainter *painter, const QRectF &rect);
        virtual bool canPaint() const;

    private:
        struct Private;
        Private *const k;
};

#endif
