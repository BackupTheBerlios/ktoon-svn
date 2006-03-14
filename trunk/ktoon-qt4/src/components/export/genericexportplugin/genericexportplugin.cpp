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

#include <dglobal.h>
#include <ddebug.h>

#include <QImage>
#include <QPainter>

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

ExportInterface::Formats GenericExportPlugin::availableFormats()
{
	return PNG | JPEG;
}

void GenericExportPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, Format format,  const QSize &size)
{
	QFileInfo fileInfo(filePath);
	
	
	QDir dir = fileInfo.dir();
	if ( !dir.exists() )
	{
		dir.mkdir(dir.path());
	}
	
	char *f = "PNG";
	
	switch(format)
	{
		case JPEG:
		{
			f = "JPEG";
		}
		break;
		default: break;
	}
	
	m_baseName = fileInfo.baseName();
	
	createImages(scenes, dir, f);
}

QStringList GenericExportPlugin::createImages(const QList<KTScene *> &scenes, const QDir &dir, const char *format)
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
								createImage( *it, &painter);
								++it;
							}
						}
					}
				}
				++layerIterator;
			}
			
			QString file = "";
			QString extension = QString::fromLocal8Bit(format).toLower();
			if ( nPhotogramsRenderized < 10 )
			{
				file = QString("000%1").arg(nPhotogramsRenderized);
			}
			else if ( nPhotogramsRenderized < 100 )
			{
				file = QString("00%1").arg(nPhotogramsRenderized);
			}
			else if( nPhotogramsRenderized < 1000 )
			{
				file = QString("0%1").arg(nPhotogramsRenderized);
			}
			else if( nPhotogramsRenderized < 10000 )
			{
				file = QString("%1").arg(nPhotogramsRenderized);
			}
			
			if ( !renderized.isNull() )
			{
				renderized.save(dir.path()+"/"+m_baseName+file+"."+extension, format);
	// 			emit progressStep( nPhotogramsRenderized, totalPhotograms);
				
				paths << dir.path()+"/"+m_baseName+file+"."+extension;
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

void GenericExportPlugin::createImage(AGraphicComponent *component, QPainter *painter)
{
	painter->save();
	foreach(AGraphic *graphic, component->graphics())
	{
		QPen pen = graphic->pen;
		QBrush brush = graphic->brush;
		
		painter->setPen(pen);
		painter->setBrush(brush);
		
		QList<QPolygonF> poligons = graphic->path.toSubpathPolygons();
		
		if ( poligons.count() == 1 )
		{
			painter->drawPath(graphic->path);
		}
		else
		{
			QList<QPolygonF>::const_iterator it;
			for(it = poligons.begin(); it != poligons.end(); ++it)
			{
				painter->drawPolygon(*it);
			}
		}
	}

	const QList< AGraphicComponent *> childs = component->childs();
	if(childs.count() > 0)
	{
		foreach(AGraphicComponent *child, childs)
		{
			createImage( child, painter);
		}
	}
	
	
	painter->restore();
}

Q_EXPORT_PLUGIN( GenericExportPlugin );

