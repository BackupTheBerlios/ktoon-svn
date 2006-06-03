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

#ifndef FFMPEGPLUGIN_H
#define FFMPEGPLUGIN_H

#include <exportinterface.h>

#include "ffmpegmanager.h"

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class FFMpegPlugin : public KTExportPluginObject, public ExportInterface
{
	Q_OBJECT;
	Q_INTERFACES(ExportInterface);
	public:
		FFMpegPlugin();
		virtual ~FFMpegPlugin();
		virtual QString key() const;
		ExportInterface::Formats availableFormats();
		
		virtual void exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, Format format,  const QSize &size, float sx= 1, float sy = 1);
		
	private:
		QStringList createImages(const QList<KTScene *> &scenes, const QDir &dir,float sx = 1, float sy = 1, const char *format = "PNG");
		QSize m_size;
};

#endif
