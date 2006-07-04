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

#include "ddebug.h"
#include "dglobal.h"

#include <QImage>
#include <QPainter>

#include <cstdio>

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
	return SWF | MPEG | AVI | RM | ASF | MOV | GIF;
}

void FFMpegPlugin::exportToFormat(const QString &filePath, const QList<KTSceneManager *> &scenes, Format format,  const QSize &size, float sx, float sy)
{
#ifdef HAVE_FFMPEG
	
	FFMpegManager manager;
	m_size = size;
	
	QDir temp(CACHE_DIR+"/exporting");
	if ( !temp.exists() )
	{
		temp.mkdir(temp.path());
	}
	
	QStringList paths = createImages(scenes, temp, sx, sy);
	
	manager.create(filePath, format, paths, m_size,scenes[0]->fps());
	
	foreach(QString path, paths)
	{
		QFile::remove(path);
	}
	
#endif
}

QStringList FFMpegPlugin::createImages(const QList<KTSceneManager *> &scenes, const QDir &dir, float sx, float sy, const char *format)
{
	QStringList paths;
#ifdef HAVE_FFMPEG
	int nPhotogramsRenderized = 0;

	
	foreach(KTSceneManager *scene, scenes )
	{
		Layers layers = scene->layers();
		
		bool m_isRendered = false;
	
		while ( ! m_isRendered )
		{
			Layers::iterator layerIterator = layers.begin();
			bool ok = true;
			
			QImage renderized = QImage(m_size, QImage::Format_RGB32);
			renderized.fill(qRgb(255, 255, 255));
			
			QPainter painter(&renderized);
			painter.setRenderHint(QPainter::Antialiasing);
			
			painter.scale(sx,sy);
			
			while ( layerIterator != layers.end() )
			{
				ok = ok && (nPhotogramsRenderized > (*layerIterator)->frames().count());
				
				if ( *layerIterator && nPhotogramsRenderized < (*layerIterator)->frames().count() && (*layerIterator)->isVisible() )
				{
					KTFrame *frame = (*layerIterator)->frames()[nPhotogramsRenderized];
					if ( frame )
					{
// 						QList<KTGraphicComponent *> componentList = frame->components();
// 												
// 						if ( componentList.count() > 0  )
// 						{
// 							QList<KTGraphicComponent *>::iterator it = componentList.begin();
// 													
// 							while ( it != componentList.end() )
// 							{
// 								(*it)->draw(&painter);
// 								++it;
// 							}
// 						}
					}
				}
				++layerIterator;
			}
			
			
			painter.end();
			
			QString file = "";
			if ( nPhotogramsRenderized < 10 )
			{
				file = QString("000%1.png").arg(nPhotogramsRenderized);
			}
			else if ( nPhotogramsRenderized < 100 )
			{
				file = QString("00%1.png").arg(nPhotogramsRenderized);
			}
			else if( nPhotogramsRenderized < 1000 )
			{
				file = QString("0%1.png").arg(nPhotogramsRenderized);
			}
			else if( nPhotogramsRenderized < 10000 )
			{
				file = QString("%1.png").arg(nPhotogramsRenderized);
			}
			
			renderized.save(dir.path()+"/"+file, "PNG");
			
			paths << dir.path()+"/"+file;
			
			if (ok )
			{
				m_isRendered = true;
			}
			
			nPhotogramsRenderized++;
			
		}
	}
#endif
	return paths;

}



#ifdef HAVE_FFMPEG
Q_EXPORT_PLUGIN( FFMpegPlugin );
#endif

