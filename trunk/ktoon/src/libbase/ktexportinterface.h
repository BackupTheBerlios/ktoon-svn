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

#ifndef KTEXPORTINTERFACE_H
#define KTEXPORTINTERFACE_H

#include <QStringList>
#include <QDir>

#include "ktexportpluginobject.h"

#include "ktscene.h"

#include "qplugin.h" // Q_EXPORT_PLUGIN

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTOON_EXPORT KTExportInterface
{
    public:
        enum Format 
             {
               NONE = 0,
               OGV  = 1 << 0,
               SWF  = 1 << 1,
               MPEG = 1 << 2,
               AVI  = 1 << 3,
               RM   = 1 << 4,
               ASF  = 1 << 5,
               MOV  = 1 << 6,
               GIF  = 1 << 7,
               PNG  = 1 << 8,
               JPEG = 1 << 9,
               XPM  = 1 << 10,
               SMIL = 1 << 11
             };

        Q_DECLARE_FLAGS(Formats, Format);

        virtual ~KTExportInterface() {};
        virtual QString key() const = 0;
        virtual Formats availableFormats() = 0;
        virtual bool exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, Format format, const QSize &size, int fps ) = 0;
        virtual const char* getExceptionMsg() = 0;
};

        Q_DECLARE_OPERATORS_FOR_FLAGS(KTExportInterface::Formats);
        Q_DECLARE_INTERFACE(KTExportInterface, "com.toonka.ktoon.ExportInterface/0.1");

#endif
