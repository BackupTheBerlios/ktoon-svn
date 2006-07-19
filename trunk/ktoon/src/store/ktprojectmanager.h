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

#ifndef KTPROJECTMANAGER_H
#define KTPROJECTMANAGER_H

#include <QObject>

class KTProject;
class KTProjectEvent;
class KTProjectCommand;

/**
 * Clase para tratar eventos del projecto
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class Q_DECL_EXPORT KTProjectManager : public QObject
{
	Q_OBJECT;
	public:
		KTProjectManager(QObject *parent = 0);
		~KTProjectManager();
		
		virtual KTProjectCommand *createCommand(const KTProjectEvent *event);
		
		void setupNewProject(const QString &projectName);
		void closeProject();
		
		bool isOpen() const;
		
		KTProject *project() const;
		
	protected slots:
		virtual void handleProjectEvent(KTProjectEvent *event);
		
	signals:
		void commandExecuted(KTProjectEvent *event);
		
	private:
		KTProject *m_project;
		bool m_isOpen;
};

#endif
