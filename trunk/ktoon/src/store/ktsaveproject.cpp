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

#include "ktsaveproject.h"

#include <QDir>

#include "ktproject.h"
#include "ktscene.h"
#include "ktpackagehandler.h"

#include "ddebug.h"

KTSaveProject::KTSaveProject() : QObject()
{
}


KTSaveProject::~KTSaveProject()
{
}

void KTSaveProject::save(const QString &fileName, const KTProject *project)
{
// 	Crear un directorio en QDir::tempPath(), guardar todo ahi, y comprimir con KTPackageHandler
	
	QDir projectDir = QDir::temp();
	
	if ( ! projectDir.mkdir(project->projectName()) )
	{
		qFatal("Can't create");
	}
	
	projectDir.cd(project->projectName());
	
	int index = 0;
	foreach ( KTScene *scene, project->scenes() )
	{
		QDomDocument doc;
		doc.appendChild(scene->toXml(doc));
		
		
		QFile scn(projectDir.path()+"/scene"+QString::number(index));
		
		if ( scn.open(QIODevice::WriteOnly | QIODevice::Text) )
		{
			QTextStream st(&scn);
			
			st << doc.toString();
			
			index += 1;
			
			scn.close();
		}
	}
	
	KTPackageHandler packageHandler;
	
	bool ok = packageHandler.makePackage(projectDir.path(), fileName);
	
	
	if ( ok )
	{
		dWarning() << tr("Project saved in %1!").arg(fileName);
	}
}



