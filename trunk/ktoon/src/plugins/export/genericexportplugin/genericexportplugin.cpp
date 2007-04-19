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
 
#include "genericexportplugin.h"

#include <dcore/dglobal.h>
#include <dcore/ddebug.h>

#include <QImage>
#include <QPainter>

#include "ktlayer.h"

GenericExportPlugin::GenericExportPlugin()
{
}


GenericExportPlugin::~GenericExportPlugin()
{
}


QString GenericExportPlugin::key() const
{
	return "Image Arrays";
}

KTExportInterface::Formats GenericExportPlugin::availableFormats()
{
	return KTExportInterface::PNG;
}

void GenericExportPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, KTExportInterface::Format format, const QSize &size, int fps)
{
	QFileInfo fileInfo(filePath);
	
	
	QDir dir = fileInfo.dir();
	if ( !dir.exists() )
	{
		dir.mkdir(dir.path());
	}
	
	char *f = "PNG";
	m_baseName = fileInfo.baseName();
}

Q_EXPORT_PLUGIN( GenericExportPlugin );

