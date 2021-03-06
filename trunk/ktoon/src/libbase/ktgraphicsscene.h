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

#ifndef KTGRAPHICSSCENE_H
#define KTGRAPHICSSCENE_H

#include <QGraphicsScene>

#include "ktglobal.h"
#include "ktsvgitem.h"

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
        
        void drawPhotogram(int photogram);
        void addFrame(KTFrame *frame, double opacity = 1.0);
        void addGraphicObject(KTGraphicObject *object, double opacity = 1.0);
        void addSvgObject(KTSvgItem *svgItem, double opacity = 1.0);
        
        void clean();
        void removeScene();
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

        void includeObject(QGraphicsItem *object);

    protected:
        virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
        virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
        virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *mouseEvent);
        virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
        virtual void keyPressEvent(QKeyEvent *keyEvent);
       
        /* 
        virtual void dragLeaveEvent(QGraphicsSceneDragDropEvent * event);
        virtual void dragEnterEvent(QGraphicsSceneDragDropEvent * event);
        virtual void dragMoveEvent(QGraphicsSceneDragDropEvent * event);
        virtual void dropEvent(QGraphicsSceneDragDropEvent * event);
        */
        
        virtual bool event(QEvent *e);

    private:
        struct Private;
        Private *const k;
};

#endif
