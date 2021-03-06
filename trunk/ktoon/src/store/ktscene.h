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

#ifndef KTSCENEMANAGER_H
#define KTSCENEMANAGER_H

#include "ktabstractserializable.h"
#include "ktproject.h"

#include <QDomDocument>
#include <QDomElement>
#include <QGraphicsScene>
#include <QMap>

#include "ktinthash.h"
#include "ktglobal_store.h"

class QGraphicsItem;
class QPainter;
class QStyleOptionGraphicsItem;
class KTLayer;
class KTSoundLayer;
class KTGraphicObject;

typedef KTIntHash<KTLayer *> Layers;
typedef KTIntHash<KTSoundLayer *> SoundLayers;

/**
 * @brief This class represents the scene data structure
 * @author David Cuadrado \<krawek@toonka.com\>
*/

class STORE_EXPORT KTScene : public QObject, public KTAbstractSerializable
{
    Q_OBJECT
    public:
        /**
          * Default Constructor
          */
        KTScene(KTProject *parent);

        /**
          * Destructor
          */
        ~KTScene();

        /**
          * Sets scene name
          */
        void setSceneName(const QString &name);

        /**
          * Locks the scene
          */
        void setLocked(bool isLocked);

        /**
          * Returns scene name
          */
        QString sceneName() const;

        /**
          * Returns true if scene is locked
          */
        bool isLocked() const;
        void setVisible(bool isVisible);
        bool isVisible() const;

        /**
          * Returns layers list
          */
        Layers layers() const;
        int layersTotal() const;
        SoundLayers soundLayers() const;

        KTLayer *layer(int position) const;
        KTSoundLayer *soundLayer(int position) const;

        void setLayers(const Layers &);

        /**
          * Removes the layer at index
          */
        bool removeLayer(int index);

        /**
          * Adds a layer, if addToEnd is true, the layer will be set at the end, otherwise next to the current layer
          */
        KTLayer *createLayer(int position, bool loaded = false);

        KTSoundLayer *createSoundLayer(int position, bool loaded = false);

        /**
          * Moves the layer to the index
          */
        bool moveLayer(int from, int to);

        //int logicalIndex() const;
        int objectIndex() const;
        int visualIndexOf(KTLayer *layer) const;
        //int logicalIndexOf(KTLayer *layer) const;

        KTProject *project() const;
        void addTweeningObject(KTGraphicObject *object);
        void removeTweeningObject(KTGraphicObject *object);
        QList<KTGraphicObject *> tweeningObjects() const;
        int framesTotal();

        QList<int> layerIndexes();

        virtual void fromXml(const QString &xml);
        virtual QDomElement toXml(QDomDocument &doc) const;

    private:
        struct Private;
        Private *const k;
};

#endif
