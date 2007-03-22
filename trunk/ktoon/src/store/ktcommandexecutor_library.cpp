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

#include "ktcommandexecutor.h"
#include "ktproject.h"
#include "ktprojectrequest.h"

#include "ktprojectresponse.h"

bool KTCommandExecutor::createSymbol(KTLibraryResponse *response)
{
	if ( m_project->createSymbol( response->symtype(), response->arg().toString(), response->data() ) )
	{
		emit responsed(response);
		
		return true;
	}
	else
	{
	}
	
	return false;
}


bool KTCommandExecutor::removeSymbol(KTLibraryResponse *response)
{
	QString xml = QString::fromLocal8Bit(response->data());
	
	if ( m_project->removeSymbol( xml ) )
	{
		emit responsed(response);
		
		return true;
	}
	else
	{
	}
	
	return false;
}

bool KTCommandExecutor::addSymbolToProject(KTLibraryResponse *response)
{
	if ( m_project->addSymbolToProject(response->arg().toString(), response->sceneIndex(), response->layerIndex(), response->frameIndex()) )
	{
		emit responsed(response);
		return true;
	}
	
	return false;
}


bool KTCommandExecutor::removeSymbolFromProject(KTLibraryResponse *response)
{
	return false;
}


