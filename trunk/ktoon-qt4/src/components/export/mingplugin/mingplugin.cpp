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
#include <ktapplication.h>

#include <QImage>
#include <QPainter>

#include <cstdio>

#ifdef HAVE_MING
#include <ming.h>
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

ExportInterface::Formats MingPlugin::availableFormats()
{
	return SWF;
}

void MingPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, Format format,  const QSize &size)
{
#ifdef HAVE_MING
	QDir temp(KTOON_TEMP_DIR+"/exporting");
	if ( !temp.exists() )
	{
		temp.mkdir(temp.path());
	}
	
	SWFMovie movie = newSWFMovie();
	
	SWFMovie_setRate(movie, scenes[0]->fps() );
	SWFMovie_setDimension(movie,  size.width(), size.height() );
	SWFMovie_setBackground(movie,  0xff, 0xff, 0xff );

	SWFMatrix matrix = SWFFillStyle_getMatrix(newSWFSolidFillStyle(255,255,255,255));
	
	SWFDisplayItem frame = SWFMovie_add( movie, matrix);
	
	QStringList paths = createImages( scenes, temp );

	foreach(QString image, paths)
	{
		FILE *fd = fopen(image.toLatin1(), "r");
		SWFBitmap bitmap = newSWFBitmap_fromInput(newSWFInput_file(fd) ); //( image.toLatin1() );
		
		fclose(fd);
		
		int width  = SWFBitmap_getWidth(bitmap);
		int height = SWFBitmap_getHeight(bitmap);

		SWFShape p_shape = newSWFShape();
		SWFShape_addBitmapFillStyle(p_shape, bitmap, 0);

		frame = SWFMovie_add(movie, p_shape);
		
		SWFMovie_nextFrame(movie);
	}
	
	SWFMovie_save(movie, filePath.toLatin1());
	
// 	FILE *f = fopen(filePath.toLatin1(), "wb");
// 	int count;
// 
// 	if(f == 0)
// 	{
// 		qDebug("WHAAAT???");
// 	}
// 	
// 	count = SWFMovie_output(movie, fileOutputMethod, f);
// 	fclose(f);
	
	destroySWFMovie(movie);

	foreach(QString path, paths)
	{
		QFile::remove(path);
	}
#endif
}

QStringList MingPlugin::createImages(const QList<KTScene *> &scenes, const QDir &dir)
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
			
			while ( layerIterator != layers.end() )
			{
				ok = ok && (nPhotogramsRenderized > (*layerIterator)->frames().count());
				
				if ( *layerIterator && nPhotogramsRenderized < (*layerIterator)->frames().count() && (*layerIterator)->isVisible() )
				{
					KTKeyFrame *frame = (*layerIterator)->frames()[nPhotogramsRenderized];
					if ( frame )
					{
						QList<AGraphicComponent *> componentList = frame->components();
												
						if ( componentList.count() > 0  )
						{
							QList<AGraphicComponent *>::iterator it = componentList.begin();
													
							while ( it != componentList.end() )
							{
								foreach(AGraphic *graphic, (*it)->graphics() )
								{
									painter.save();
														
									painter.setPen(graphic->pen);
									painter.setBrush(graphic->brush);
									
									QList<QPolygonF> poligons = graphic->path.toSubpathPolygons();
									if ( poligons.count() == 1 )
									{
										painter.drawPath(graphic->path);
									}
									else
									{
										QList<QPolygonF>::const_iterator it;
										for(it = poligons.begin(); it != poligons.end(); ++it)
										{
										painter.drawPolygon(*it);
										}
									}
								}
								++it;
							}
						}
					}
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
			
			renderized.save(dir.path()+"/"+file, "PNG");
// 			emit progressStep( nPhotogramsRenderized, totalPhotograms);
			
			paths << dir.path()+"/"+file;
			
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

