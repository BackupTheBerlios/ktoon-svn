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
#include <kcore/kglobal.h>
#include <kcore/kdebug.h>

#include <QImage>
#include <QPainter>

#include <cstdio>

#include "ktlayer.h"

#include <mingpp.h>

MingPlugin::MingPlugin()
{
}

MingPlugin::~MingPlugin()
{
}


QString MingPlugin::key() const
{
	return "Ming";
}

KTExportInterface::Formats MingPlugin::availableFormats()
{
	return KTExportInterface::SWF;
}

bool MingPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, KTExportInterface::Format format, const QSize &size, int fps)
{
    return true;
}

Q_EXPORT_PLUGIN( MingPlugin );


