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
#ifndef KTNETPROJECTMANAGER_H
#define KTNETPROJECTMANAGER_H

#include <QDomDocument>
#include <ktabstractprojectmanagerhandler.h>

class KTProjectCommand;
class KTNetSocket;
class KTNetProjectManagerParams;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTNetProjectManagerHandler : public KTAbstractProjectHandler
{
	Q_OBJECT;
	public:
		KTNetProjectManagerHandler(QObject *parent = 0);
		~KTNetProjectManagerHandler();
		
		virtual bool initialize(KTProjectManagerParams *params);
		virtual bool setupNewProject(KTProjectManagerParams *params);
		virtual bool closeProject();
		
		virtual void handleProjectRequest(const KTProjectRequest* event);
		virtual bool commandExecuted(KTProjectResponse *response);
		
		virtual bool saveProject(const QString &fileName, const KTProject *project);
		
		virtual bool loadProject(const QString &fileName, KTProject *project);
		
		bool loadProjectFromServer(const QString &name);
		
		void emitRequest(KTProjectRequest *request);
		void handlePackage(const QString &root, const QString &package);
		
		virtual bool isValid() const;
		
		void sendPackage(const QDomDocument &doc);
		void setProject(KTProject *project);
		
	private:
		KTNetSocket *m_socket;
		QString m_projectName, m_author;
		
		KTNetProjectManagerParams *m_params;
		KTProject *m_project;
		
	signals:
		void openNewArea(const QString &name);
		
};

#endif
