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

#ifndef AFILTERINTERFACE_H
#define AFILTERINTERFACE_H

#include <QStringList>
#include <QRect>
#include <QPoint>
#include <QPainter>
#include <QBrush>
#include <QPen>
#include <QPainterPath>
#include <QHash>

#include <cmath> // sin,cos

#include "ktframe.h"
#include <kgui/kaction.h>

#include "qplugin.h" // Q_EXPORT_PLUGIN

class QKeySequence;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTOON_EXPORT AFilterInterface
{
    public:
        virtual ~AFilterInterface() {}
        virtual QStringList keys() const = 0;
        // virtual void filter(const QString &filter, const QList<KTGraphicComponent *> &frame) = 0;
        
        virtual QHash<QString, KAction *>actions() = 0;
};

Q_DECLARE_INTERFACE(AFilterInterface, "com.toonka.ktoon.AFilterInterface/0.1");

#endif
