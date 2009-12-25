/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KTGRAPHICSSCENE_H
#define KTGRAPHICSSCENE_H

#include <QGraphicsScene>

#include "ktglobal.h"

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/

class KTFrame;
class KTGraphicObject;
class KTScene;
class KTToolPlugin;
class KTBrushManager;
class QMouseEvent;
class KTItemResponse;

class KTOON_EXPORT KTGraphicsScene : public QGraphicsScene
{
    public:
        KTGraphicsScene();
        ~KTGraphicsScene();
        
        void setCurrentFrame(int layer, int frame);
        
        void setCurrentScene(KTScene *scene);
        void drawCurrentPhotogram();
        
        void drawItems(QPainter *painter, int numItems, QGraphicsItem *items[], const QStyleOptionGraphicsItem options[], QWidget *widget);
        
        void drawPhotogram(int photogram);
        void addFrame(KTFrame *frame, double opacity = 1.0);
        void addGraphicObject(KTGraphicObject *object, double opacity = 1.0);
        
        void clean();
        int currentFrameIndex() const;
        int currentLayerIndex() const;
        int currentSceneIndex() const;
        
        void setNextOnionSkinCount(int n);
        void setPreviousOnionSkinCount(int n);
        
        void setLayerVisible(int layerIndex, bool visible);
        
        KTScene *scene() const;
        
        KTFrame *currentFrame();
        
        void setTool(KTToolPlugin *tool);
        KTToolPlugin *currentTool() const;
        
        bool isDrawing() const;
        
        KTBrushManager *brushManager() const;
        
        void itemResponse(KTItemResponse *event);
        
        void mouseMoved(QGraphicsSceneMouseEvent *event);
        void mouseReleased(QGraphicsSceneMouseEvent *event);

        virtual void aboutToMousePress();
        
    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        virtual void keyPressEvent(QKeyEvent *keyEvent);
        
        virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent * event);
        virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
        virtual void dragMoveEvent(QGraphicsSceneDragDropEvent * event);
        virtual void dropEvent(QGraphicsSceneDragDropEvent * event);
        
        virtual bool event(QEvent *e);
        
        
    private:
        struct Private;
        Private *const k;
};

#endif
