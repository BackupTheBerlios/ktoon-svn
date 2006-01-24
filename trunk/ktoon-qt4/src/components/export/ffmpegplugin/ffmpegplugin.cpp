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

#include "ffmpegplugin.h"

#ifdef HAVE_FFMPEG
#include <ffmpeg/avcodec.h>
#endif

FFMpegPlugin::FFMpegPlugin()
{
}


FFMpegPlugin::~FFMpegPlugin()
{
}

QString FFMpegPlugin::key() const
{
	return "ffmpeg";
}

ExportInterface::Formats FFMpegPlugin::availableFormats()
{
	return SWF | MPEG | AVI;
}

void FFMpegPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, Format format, int fps)
{
}

#ifdef HAVE_FFMPEG
Q_EXPORT_PLUGIN( FFMpegPlugin );
#endif

