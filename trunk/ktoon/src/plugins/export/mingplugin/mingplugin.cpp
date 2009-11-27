/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "mingplugin.h"
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


