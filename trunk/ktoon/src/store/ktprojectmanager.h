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
#include "ktglobal_store.h"

class KTProject;
class KTProjectRequest;
class KTProjectCommand;
class KTProjectManagerParams;
class KTAbstractProjectHandler;
class QUndoStack;

/**
 * Clase para tratar eventos del proyecto
 * @author David Cuadrado \<krawek@gmail.com\>
*/
class STORE_EXPORT KTProjectManager : public QObject
{
	Q_OBJECT;
	public:
		KTProjectManager(QObject *parent = 0);
		virtual ~KTProjectManager();
		
		virtual void setupNewProject(KTProjectManagerParams *params );
		virtual void closeProject();
		
		bool isOpen() const;
		
		KTProject *project() const;
		
		void setHandler(KTAbstractProjectHandler *handler);
		
		QUndoStack *undoHistory() const;
		
		
	public slots:
		virtual void createCommand(const KTProjectRequest *event);
		
	protected slots:
		virtual void handleProjectRequest(KTProjectRequest *event);
		
	private slots:
		void executeRequest(KTProjectRequest *request);
		
	signals:
		void commandExecuted(KTProjectRequest *event);
		
	private:
		KTProject *m_project;
		bool m_isOpen;
		
		KTAbstractProjectHandler *m_handler;
		
		QUndoStack *m_undoStack;
};

#endif
