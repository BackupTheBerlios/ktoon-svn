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

#ifndef KTPAINTAREA_H
#define KTPAINTAREA_H

#include "ktpaintareabase.h"
#include "ktabstractprojectresponsehandler.h"
#include "ktgraphicsscene.h"

class QGraphicsRectItem;
class KTBrushManager;
class KTInputDeviceInformation;
class KTProject;
class KTPaintAreaRotator;

/**
 * This class provides an area to make different kind of traces
 * @author Jorge Cuadrado \<kuadrosx@toonka.com\> - David Cuadrado \<krawek@toonka.com\>
*/

class KTPaintArea : public KTPaintAreaBase, public KTAbstractProjectResponseHandler
{
    Q_OBJECT;
    public:
        enum MoveItemType { MoveBack, MoveFront, MoveBackwards, MoveForwards };

        KTPaintArea(const KTProject *project, QWidget * parent = 0);
        ~KTPaintArea();

        void setCurrentScene(int index);
        void setCurrentTool(QString tool);
        void updatePaintArea();

    public slots:
        void setNextFramesOnionSkinCount(int n);
        void setPreviousFramesOnionSkinCount(int n);

    signals:
        void requestTriggered(const KTProjectRequest *event);
        void itemAddedOnSelection(KTGraphicsScene *);

    public slots:
        void deleteItems();
        void groupItems();
        void ungroupItems();
        void copyItems();
        void pasteItems();
        void cutItems();

        void addSelectedItemsToLibrary();
        void requestMoveSelectedItems(QAction *action);

    protected:
        void mousePressEvent(QMouseEvent * event);
        void frameResponse(KTFrameResponse *event);
        void layerResponse(KTLayerResponse *event);
        void sceneResponse(KTSceneResponse *event);
        void itemResponse(KTItemResponse *event);
        void libraryResponse(KTLibraryResponse *request);
        void projectResponse(KTProjectResponse *projectResponse);

        bool canPaint() const;

    private:
        struct Private;
        Private *const k;
};

#endif
