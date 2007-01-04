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

#ifndef KTABSTRACTPROJECTHANDLER_H
#define KTABSTRACTPROJECTHANDLER_H

#include <qobject.h>
#include "ktglobal_store.h"

class KTProjectResponse;
class KTProjectRequest;
class KTProjectManagerParams;
class KTProjectCommand;
class KTProject;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTAbstractProjectHandler : public QObject
{
	Q_OBJECT;
	
	public:
		KTAbstractProjectHandler(QObject *parent = 0);
		virtual ~KTAbstractProjectHandler();
		
		virtual bool setupNewProject(KTProjectManagerParams *params);
		virtual bool closeProject();
		virtual void handleProjectRequest(const KTProjectRequest *request) = 0;
		virtual bool commandExecuted(KTProjectResponse *response, int state);
		
		virtual bool saveProject(const QString &fileName, const KTProject *project) = 0;
		virtual bool loadProject(const QString &fileName, KTProject *project) = 0;
	signals:
		void sendCommand(const KTProjectRequest *event, bool addToStack);

};


#endif


