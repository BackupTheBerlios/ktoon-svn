/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktlocalprojectmanagerhandler.h"
#include "ktprojectrequest.h"
#include "ktprojectcommand.h"
#include "ktsaveproject.h"

#include <kcore/kdebug.h>

/**
 * This class handles all the procedures related to the projects local format for KToon.
 *
 * @author David Cuadrado <krawek@toonka.com>
*/

KTLocalProjectManagerHandler::KTLocalProjectManagerHandler(QObject *parent) : KTAbstractProjectHandler(parent)
{
}


KTLocalProjectManagerHandler::~KTLocalProjectManagerHandler()
{
}

void KTLocalProjectManagerHandler::handleProjectRequest(const KTProjectRequest *request)
{

	#ifdef K_DEBUG
		K_FUNCINFO;
	#endif
	if ( request->isValid() )
	{
		emit sendCommand( request, true );
	}
	else
	{
		#ifdef K_DEBUG
			kfDebug << "INVALID REQUEST! ID: " << request->id();
		#endif
	}
}

bool KTLocalProjectManagerHandler::saveProject(const QString &fileName, const KTProject *project)
{
	bool result = false;
	QString fn = fileName;
	
	if ( !fileName.endsWith(".ktn") )
	{
		fn+=".ktn";
	}
	
	KTSaveProject *saver = new KTSaveProject;
	result = saver->save(fn, project);
	
	delete saver;
	
	return result;
}

bool KTLocalProjectManagerHandler::loadProject(const QString &fileName, KTProject *project)
{
	bool result = false;
	KTSaveProject *loader = 0;
	
	if ( fileName.endsWith(".ktn") )
	{
		loader = new KTSaveProject;
		result = loader->load(fileName, project);
		
		delete loader;
	}
	else
	{
		result = false;
	}
	
	return result;
}

void KTLocalProjectManagerHandler::setProject(KTProject *)
{
	
}
