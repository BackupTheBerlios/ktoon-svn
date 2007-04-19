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

#include <dcore/ddebug.h>
#include <dcore/dglobal.h>

#include <dgui/dffmpegmoviegenerator.h>

#include <QImage>
#include <QPainter>

#include "ktlayer.h"
#include "ktanimationrenderer.h"

FFMpegPlugin::FFMpegPlugin()
{
}


FFMpegPlugin::~FFMpegPlugin()
{
}

QString FFMpegPlugin::key() const
{
	return "FFMpeg";
}

KTExportInterface::Formats FFMpegPlugin::availableFormats()
{
	return KTExportInterface::SWF | KTExportInterface::MPEG | KTExportInterface::AVI | KTExportInterface::RM | KTExportInterface::ASF | KTExportInterface::MOV | KTExportInterface::GIF;
}

void FFMpegPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, KTExportInterface::Format format, const QSize &size, int fps)
{
	DFFMpegMovieGenerator *generator = 0;
	
	switch(format)
	{
		case KTExportInterface::SWF:
		{
			generator = new DFFMpegMovieGenerator(DFFMpegMovieGenerator::SWF, size, fps);
		}
		break;
		case KTExportInterface::MPEG:
		{
			generator = new DFFMpegMovieGenerator(DFFMpegMovieGenerator::MPEG, size, fps);
		}
		break;
		case KTExportInterface::AVI:
		{
			generator = new DFFMpegMovieGenerator(DFFMpegMovieGenerator::AVI, size, fps);
		}
		break;
		case KTExportInterface::RM:
		{
			generator = new DFFMpegMovieGenerator(DFFMpegMovieGenerator::RM, size, fps);
		}
		break;
		case KTExportInterface::MOV:
		{
			generator = new DFFMpegMovieGenerator(DFFMpegMovieGenerator::MOV, size, fps);
		}
		break;
		case KTExportInterface::ASF:
		{
			generator = new DFFMpegMovieGenerator(DFFMpegMovieGenerator::ASF, size, fps);
		}
		break;
		case KTExportInterface::GIF:
		{
			generator = new DFFMpegMovieGenerator(DFFMpegMovieGenerator::GIF, size, fps);
		}
		break;
		default: return;
	}
	
	
	KTAnimationRenderer renderer;
	{
		QPainter painter(generator);
		painter.setRenderHint(QPainter::Antialiasing, true);
		
		foreach(KTScene *scene, scenes)
		{
			renderer.setScene(scene);
			
			while(renderer.nextPhotogram())
			{
				renderer.render(&painter);
				generator->nextFrame();
				generator->reset();
			}
		}
	}
	
	generator->saveMovie(filePath);
	delete generator;
}


#ifdef HAVE_FFMPEG
Q_EXPORT_PLUGIN( FFMpegPlugin );
#endif

