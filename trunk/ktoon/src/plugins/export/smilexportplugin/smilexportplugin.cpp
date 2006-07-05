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
 
#include "smilexportplugin.h"

#include <dglobal.h>
#include <ddebug.h>

#include <QImage>
#include <QPainter>

SmilExportPlugin::SmilExportPlugin()
{
}


SmilExportPlugin::~SmilExportPlugin()
{
}


QString SmilExportPlugin::key() const
{
	return "SMIL 2.0";
}

KTExportInterface::Formats SmilExportPlugin::availableFormats()
{
	return SMIL;
}

void SmilExportPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, Format format,  const QSize &size,float sx, float sy)
{
	m_size = size;
	QFileInfo fileInfo(filePath);
	
	QDir dir = fileInfo.dir();
	if ( !dir.exists() )
	{
		dir.mkdir(dir.path());
	}
	
	m_baseName = fileInfo.baseName();
	
	dir.mkdir("data");
	
	initSmil();
	
	createImages(scenes, dir, sx, sy);
	
	m_smil.documentElement().appendChild(m_body);
	
	QFile save(filePath);
	if ( save.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&save);
		out << m_smil.toString();
	}
}

void SmilExportPlugin::initSmil()
{
	m_smil = QDomDocument("smil PUBLIC \"-//W3C//DTD SMIL 2.0//EN\" \"http://www.w3.org/2001/SMIL20/SMIL20.dtd\"");
	
	QDomElement root = m_smil.createElement("smil");
	
	root.setAttribute("xmlns", "http://www.w3.org/2001/SMIL20/Language");
	
	QDomElement head = m_smil.createElement("head");
	
	QDomElement generator = m_smil.createElement("meta");
	generator.setAttribute("name", "generator");
	generator.setAttribute("content", "KToon" );
	
	head.appendChild(generator);
	
	QDomElement layout = m_smil.createElement("layout");
	
	QDomElement rootLayout = m_smil.createElement("root-layout");
	rootLayout.setAttribute("id", "MainLayout");
	rootLayout.setAttribute("backgroundColor", "black");
	rootLayout.setAttribute("width", m_size.width());
	rootLayout.setAttribute("height", m_size.height());
	
	layout.appendChild(rootLayout);
	
// 	<region id="Images" left="18" width="220" top="6" height="240" z-index="1" fit="meet"/>
	QDomElement region = m_smil.createElement("region");
	region.setAttribute("id", "Animation");
	
	
	region.setAttribute("left", 0);
	region.setAttribute("top", 0);
	region.setAttribute("width", m_size.width());
	region.setAttribute("height", m_size.height());
	region.setAttribute("z-index", 1);
	region.setAttribute("fit", "meet");
	
	layout.appendChild(region);
	
	head.appendChild(layout);
	
	root.appendChild(head);
	
	m_body = m_smil.createElement("body");
	
	m_smil.appendChild(root);
}

QStringList SmilExportPlugin::createImages(const QList<KTScene *> &scenes, const QDir &dir, float sx, float sy, const char *format)
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
			
			QImage renderized = QImage(m_size.width(), m_size.height(), QImage::Format_RGB32);
			renderized.fill(qRgb(255, 255, 255));
			
			QPainter painter(&renderized);
			painter.setRenderHint(QPainter::Antialiasing);
			
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
// 								(*it)->draw( &painter);
// 								++it;
// 							}
// 						}
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
				QString dest = dir.path()+"/data/"+m_baseName+file+"."+extension;
				renderized.save(dest, format);
				
				paths << dest;
				
				createPar( dest, 1.0f/(float)scene->fps() );
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

void SmilExportPlugin::createPar(const QString &filePath, double duration)
{
	QFileInfo finfo(filePath);
	QString relative = "data/"+finfo.baseName()+"."+finfo.completeSuffix();
	
	QDomElement par = m_smil.createElement("par");
	
// 	<img id="TitleA" region="Images" dur="8s" fill="transition" src="NYCdata/TitleA.gif"/>
	QDomElement img = m_smil.createElement("img");
	img.setAttribute("id", finfo.baseName());
	img.setAttribute("region", "Animation");
	img.setAttribute("dur", QString("%1s").arg(duration));
	img.setAttribute("fill", "transition");
	img.setAttribute("src", relative);
	
	par.appendChild(img);
	
	m_body.appendChild(par);
}

Q_EXPORT_PLUGIN( SmilExportPlugin );

