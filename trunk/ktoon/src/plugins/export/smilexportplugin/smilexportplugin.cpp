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

#include <dcore/dglobal.h>
#include <dcore/ddebug.h>

#include <QImage>
#include <QPainter>

#include "ktlayer.h"

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
	return KTExportInterface::SMIL;
}

void SmilExportPlugin::exportToFormat(const QString &filePath, const QList<KTScene *> &scenes, KTExportInterface::Format format, const QSize &size, int fps)
{
	QFileInfo fileInfo(filePath);
	
	QDir dir = fileInfo.dir();
	if ( !dir.exists() )
	{
		dir.mkdir(dir.path());
	}
	
	m_baseName = fileInfo.baseName();
	
	dir.mkdir("data");
	
	initSmil();
	
// 	createImages(scenes, dir, 1, sy);
	
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

