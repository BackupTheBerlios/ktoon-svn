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

#include "ktprojectmanagerparams.h"

#include "ktabstractprojectmanagerhandler.h"

#include <ddebug.h>

KTProjectManager::KTProjectManager(QObject *parent) : QObject(parent), m_isOpen(false), m_handler(0)
{
	DINIT;
	m_project = new KTProject(this);
	
	m_undoStack = new QUndoStack(this);
	
	connect(m_project,SIGNAL(commandExecuted( KTProjectRequest* )), this, SLOT(handleProjectRequest( KTProjectRequest *)));
}


KTProjectManager::~KTProjectManager()
{
	DEND;
}

void KTProjectManager::setHandler(KTAbstractProjectHandler *handler)
{
	if ( m_handler )
	{
		disconnect(m_handler, SIGNAL(sendRequestToClients(KTProjectRequest *)), this, SIGNAL(commandExecuted( KTProjectRequest *)));
		
		delete m_handler;
		m_handler = 0;
	}
	
	m_handler = handler;
	m_handler->setParent(this);
	
	connect(m_handler, SIGNAL(sendRequestToClients(KTProjectRequest *)), this, SIGNAL(commandExecuted(KTProjectRequest *)));
	
}

void KTProjectManager::executeRequest(KTProjectRequest *request)
{
	KTProjectCommand command(m_project, request);
	command.redo();
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
	
// 	// Add by default a scene, layer, frame
	
	// Hacer mediante comandos!
	m_project->createScene(0);
	m_project->createLayer(0,0);
	m_project->createFrame(0,0,0);
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
	
	m_undoStack->clear();
}

/**
 * Retorna verdadero si el proyecto esta abierto
 */
bool KTProjectManager::isOpen() const
{
	return m_isOpen;
}


/**
 * Esta función es ejecutada cuando un evento es disparado por el proyecto.
 * Debe reimplementarse si se quiere dar un trato distinto al evento, como por ejemplo enviarlo por la red.
 * Por defecto, envia el evento por medio del signal commandExecuted
 * @param event 
 */
void KTProjectManager::handleProjectRequest(KTProjectRequest *request)
{
	D_FUNCINFO;
	
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
void KTProjectManager::createCommand(const KTProjectRequest *request)
{
	D_FUNCINFO;
	
	if ( request->isValid() )
	{
		KTProjectCommand *command = m_handler->createCommand(m_project, request);
		
		if ( command )
		{
			qDebug("ADDING");
			m_undoStack->push(command);
		}
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


