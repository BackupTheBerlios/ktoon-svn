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

#ifndef KTSVGITEM_H
#define KTSVGITEM_H

#include <QGraphicsSvgItem>
#include "ktabstractserializable.h"
#include "ktglobal_store.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class STORE_EXPORT KTSvgItem : public QGraphicsSvgItem, public KTAbstractSerializable
{
    public:
        KTSvgItem(QGraphicsItem * parent = 0);
        KTSvgItem(QString &file);
        ~KTSvgItem();
        QString itemPath() const;
        void rendering();
        virtual void fromXml(const QString &xml);
        virtual QDomElement toXml(QDomDocument &doc) const;

    private:
        QString path;
        QString data;

};

#endif
