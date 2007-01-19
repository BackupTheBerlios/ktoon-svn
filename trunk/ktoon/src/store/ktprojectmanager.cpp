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

#include "ktprojectmanager.h"

#include <QUndoStack>

#include "ktproject.h"
#include "ktprojectrequest.h"
#include "ktprojectcommand.h"
#include "ktcommandexecutor.h"

#include "ktprojectmanagerparams.h"

#include "ktabstractprojectmanagerhandler.h"

#include "ktprojectresponse.h"
#include "ktrequestbuilder.h"
#include "ktrequestparser.h"

#include <ddebug.h>

KTProjectManager::KTProjectManager(QObject *parent) : QObject(parent), m_isOpen(false), m_isModified(false), m_handler(0)
{
	DINIT;
	m_project = new KTProject(this);
	m_undoStack = new QUndoStack(this);
	
	m_commandExecutor = new KTCommandExecutor(m_project);
	
	connect(m_commandExecutor, SIGNAL(responsed( KTProjectResponse* )), this, SLOT(emitResponse( KTProjectResponse *))); // FIXME: cuando carga hay problemas
	
	connect(m_project, SIGNAL(responsed(KTProjectResponse*)), this, SIGNAL(responsed(KTProjectResponse *)));
}


KTProjectManager::~KTProjectManager()
{
	DEND;
}

void KTProjectManager::setHandler(KTAbstractProjectHandler *handler)
{
	if ( m_handler )
	{
		disconnect(m_handler, SIGNAL(sendCommand(const KTProjectRequest *, bool)), this, SLOT(createCommand(const KTProjectRequest *, bool)));
		
		delete m_handler;
		m_handler = 0;
	}
	
	m_handler = handler;
	m_handler->setParent(this);
	
	connect(m_handler, SIGNAL(sendCommand(const KTProjectRequest *, bool)), this, SLOT(createCommand(const KTProjectRequest *, bool)));
}

void KTProjectManager::setupNewProject(KTProjectManagerParams *params)
{
	D_FUNCINFO;
	
	if ( !m_handler )
	{
		qDebug("ERROR: HANDLER!");
		return;
	}
	
	closeProject();
	
	m_project->setProjectName( params->projectName() );
	
	if ( ! m_handler->setupNewProject(params) )
	{
		qDebug("ERROR WHILE SETUP PROJECT");
		return;
	}
	
	m_isOpen = true;
	
	KTProjectRequest request = KTRequestBuilder::createSceneRequest(0, KTProjectRequest::Add, QString());
	
	handleProjectRequest(&request);
	
	request = KTRequestBuilder::createLayerRequest(0, 0, KTProjectRequest::Add, QString());
	
	handleProjectRequest(&request);
	
	request = KTRequestBuilder::createFrameRequest(0, 0, 0, KTProjectRequest::Add, QString());
	handleProjectRequest(&request);
}


void KTProjectManager::closeProject()
{
	if ( !m_handler ) return;
	
	if ( m_isOpen )
	{
		if ( ! m_handler->closeProject() )
		{
			qDebug("ERROR: WHILE CLOSING THE PROJECT");
			return;
		}
		
		m_project->clear();
	}
	
	m_isOpen = false;
	m_isModified = false;
	
	m_undoStack->clear();
}

bool KTProjectManager::saveProject(const QString &fileName)
{
	bool result = m_handler->saveProject(fileName, m_project);
	
	m_isModified = !result;
	
	return result;
}

bool KTProjectManager::loadProject(const QString &fileName)
{
	if ( ! m_handler )
	{
		dFatal() << "NO HANDLER!";
		return false;
	}
	
	bool ok = m_handler->loadProject(fileName, m_project);
	
	if ( ok )
	{
		m_isOpen = true;
		m_isModified = false;
	}
	
	return ok;
}

/**
 * Retorna verdadero si el proyecto esta abierto
 */
bool KTProjectManager::isOpen() const
{
	return m_isOpen;
}

bool KTProjectManager::isModified() const
{
	return m_isModified;
}


/**
 * Esta función es ejecutada cuando un evento es disparado por el proyecto.
 * Debe reimplementarse si se quiere dar un trato distinto al evento, como por ejemplo enviarlo por la red.
 * Por defecto, envia el evento por medio del signal commandExecuted
 * @param event 
 */
void KTProjectManager::handleProjectRequest(const KTProjectRequest *request)
{
	D_FUNCINFO;
	
	dWarning() << request->xml();
	
	// TODO: el handler debe decir cuando construir el comando
	
	if ( m_handler )
	{
		m_handler->handleProjectRequest( request );
	}
	else
	{
		qDebug("ERROR: NO HANDLER");
	}
}


/**
 * Se crea un comando para realizar una accion, por ejemplo añadir frame, este mismo comando tiene la información necesaria para revertir su efecto.
 * Normalmente este comando debe ser añadido a una pila de comandos.
 * El comando creado no es borrado por esta clase, la responsabilidad de borrarlo esta en el usuario.
 * @param event 
 * @return 
 */
void KTProjectManager::createCommand(const KTProjectRequest *request, bool addToStack)
{
	D_FUNCINFO;
	
	if ( request->isValid() )
	{
		KTProjectCommand *command = new KTProjectCommand(m_commandExecutor, request);
		
		if ( addToStack )
		{
			m_undoStack->push(command);
		}
		else
		{
			command->redo();
		}
	}
	else
	{
		dWarning() << "invalid request";
	}
}


KTProject *KTProjectManager::project() const
{
	return m_project;
}


QUndoStack *KTProjectManager::undoHistory() const
{
	return m_undoStack;
}


void KTProjectManager::emitResponse( KTProjectResponse *response)
{
	D_FUNCINFO << response->action();
	
	if( response->action() != KTProjectRequest::Select )
	{
		m_isModified = true;
	}
	
	if ( !m_handler )
	{
		emit responsed( response );
	}
	else if ( m_handler->commandExecuted(response ) )
	{
		emit responsed( response );
	}
}


