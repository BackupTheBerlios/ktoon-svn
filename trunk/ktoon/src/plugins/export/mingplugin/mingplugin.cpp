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
#include <dglobal.h>
#include <ddebug.h>

#include <QImage>
#include <QPainter>

#include <cstdio>

#ifdef HAVE_MING
#include <mingpp.h>
#endif

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
	return SWF;
}

void MingPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, Format format,  const QSize &size, float sx, float sy)
{
#ifdef HAVE_MING
	QDir temp(CACHE_DIR+"/exporting");
	if ( !temp.exists() )
	{
		temp.mkdir(temp.path());
	}
	
	Ming_init();
	
	SWFMovie movie;
	
	// 	movie.setRate(scenes[0]->fps() ); // FIXME
	movie.setDimension(size.width(), size.height() );
	movie.setBackground(0xff, 0xff, 0xff );

	SWFShape shape;
	shape.addSolidFill(255,255,255,255);

	SWFDisplayItem *frame = movie.add( &shape);
	
	QStringList paths = createImages( scenes, temp );

	foreach(QString image, paths)
	{
		SWFBitmap bitmap(image.toLatin1().data());
		
		SWFShape p_shape;
		p_shape.addBitmapFill(&bitmap);
		frame = movie.add( &p_shape);
		movie.nextFrame();
	}
	
	movie.save(filePath.toLatin1().data());
	
	foreach(QString path, paths)
	{
		QFile::remove(path);
	}
	
// 	Ming_cleanup();
// 	Ming_collectGarbage();
#endif
}

QStringList MingPlugin::createImages(const QList<KTScene *> &scenes, const QDir &dir, float sx, float sy, const char *format)
{
	QStringList paths;
	
	int nPhotogramsRenderized = 0;
	
	foreach(KTScene *scene, scenes )
	{
		Layers layers = scene->layers();
		
		bool m_isRendered = false;
	
		while ( ! m_isRendered )
		{
			Layers::iterator layerIterator = layers.begin();
			bool ok = true;
			
			QImage renderized = QImage(520, 340, QImage::Format_RGB32);
			renderized.fill(qRgb(255, 255, 255));
			
			QPainter painter(&renderized);
			painter.setRenderHint(QPainter::Antialiasing);
			
			painter.scale(sx, sy);
			
			while ( layerIterator != layers.end() )
			{
				ok = ok && (nPhotogramsRenderized > (*layerIterator)->frames().count());
				
				if ( *layerIterator && nPhotogramsRenderized < (*layerIterator)->frames().count() && (*layerIterator)->isVisible() )
				{
// 					KTFrame *frame = (*layerIterator)->frames()[nPhotogramsRenderized];
// 					if ( frame )
// 					{
// 						QList<KTGraphicComponent *> componentList = frame->components();
// 												
// 						if ( componentList.count() > 0  )
// 						{
// 							QList<KTGraphicComponent *>::iterator it = componentList.begin();
// 													
// 							while ( it != componentList.end() )
// 							{
// 								(*it)->draw( &painter);
// 								++it;
// 							}
// 						}
// 					}
				}
				++layerIterator;
			}
			
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
			
			if ( !renderized.isNull() )
			{
				renderized.save(dir.path()+"/"+file, "PNG");
	// 			emit progressStep( nPhotogramsRenderized, totalPhotograms);
				
				paths << dir.path()+"/"+file;
			}
			
			if (ok )
			{
				m_isRendered = true;
			}
			
			nPhotogramsRenderized++;
		}
	}
	
	return paths;
}

#ifdef HAVE_MING
Q_EXPORT_PLUGIN( MingPlugin );
#endif

