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

#ifndef KTGRAPHICOBJECT_H
#define KTGRAPHICOBJECT_H

#include <QObject>
#include "ktabstractserializable.h"
#include "ktframe.h"

#include "ktglobal_store.h"

class QGraphicsItem;
class KTItemTweener;

/**
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class STORE_EXPORT KTGraphicObject : public QObject, public KTAbstractSerializable
{
    public:
        enum Transformations{ScaleX = 1, ScaleY, Rotate, TranslateX, TranslateY};
        
        KTGraphicObject(QGraphicsItem *item, KTFrame *parent);
        ~KTGraphicObject();
        
        void setItem(QGraphicsItem *item);
        QGraphicsItem *item() const;
        
        void setObjectName(const QString &name);
        QString objectName() const;
        
        void setTweener(KTItemTweener *tweener);
        KTItemTweener *tweener() const;
        
        KTFrame *frame() const;
        //int logicalIndex() const;
        int objectIndex() const;
        
    public:
        virtual void fromXml(const QString &xml);
        virtual QDomElement toXml(QDomDocument &doc)  const;
        
    private:
        void initItemData();
        
        struct Private;
        Private *const k;
};

#endif
