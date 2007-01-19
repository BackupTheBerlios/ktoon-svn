/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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

#ifndef PROJECTCOLLECTION_H
#define PROJECTCOLLECTION_H

/**
 * @author Jorge Cuadrado \<kuadrosxx@gmail.com\>
*/
#include <QHash>
#include <ktproject.h>

class KTServerConnection ;

class ProjectCollection 
{
	public:
		ProjectCollection();
		~ProjectCollection();
		
		void createProject(const KTServerConnection *cnn);
		void openProject(const KTServerConnection *cnn);
		void handleProjectRequest(const KTServerConnection *cnn, const QString strRequest);
		QStringList projects() const;
		void closeProject(const QString & name);
		void saveProject(const QString & name);
		
		
	private:
		QHash<QString, KTProject * > m_projects;
		
};

#endif
