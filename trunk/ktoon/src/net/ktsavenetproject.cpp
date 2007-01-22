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

#include "ktsavenetproject.h"

#include <QFile>
#include <QTextStream>
#include <QDomDocument>
#include <QDomElement>

#include "ktproject.h"

KTSaveNetProject::KTSaveNetProject() : KTSaveProject()
{
}

KTSaveNetProject::KTSaveNetProject(const QString &server, int port)
 : KTSaveProject(), m_server(server), m_port(port)
{
}


KTSaveNetProject::~KTSaveNetProject()
{
}

bool KTSaveNetProject::save(const QString &filename, const KTProject *project)
{
	QDomDocument doc;
	QDomElement root = doc.createElement("NetProject");
	
	QDomElement projectElement = doc.createElement("project");
	projectElement.setAttribute("name", project->projectName());
	
	root.appendChild(projectElement);
	
	QDomElement connection = doc.createElement("connection");
	
	QDomElement server = doc.createElement("server");
	QDomText serverValue = doc.createTextNode(m_server);
	
	server.appendChild(serverValue);
	
	QDomElement port = doc.createElement("port");
	QDomText portValue = doc.createTextNode(QString::number(m_port));
	
	port.appendChild(portValue);
	
	connection.appendChild(server);
	connection.appendChild(port);
	
	root.appendChild(connection);
	
	doc.appendChild(root);
	
	QFile file(filename);
	
	if ( !file.open(QIODevice::WriteOnly | QIODevice::Text))
		return false;
	
	
	QTextStream ts(&file);
	
	ts << doc.toString();
	
	file.close();
	
	
	return true;
}

bool KTSaveNetProject::load(const QString &fileName, KTProject *)
{
	
	return false;
}

