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

#include "ktproject.h"
#include "ktprojectevent.h"
#include "ktprojectcommand.h"

#include <ddebug.h>

KTProjectManager::KTProjectManager(QObject *parent) : QObject(parent), m_isOpen(false)
{
	m_project = new KTProject(this);
	
	connect(m_project,SIGNAL(commandExecuted( KTProjectEvent* )), this, SLOT(handleProjectEvent( KTProjectEvent *)));
}


KTProjectManager::~KTProjectManager()
{
}

void KTProjectManager::setupNewProject(const QString &projectName)
{
	closeProject();
	
// 	m_project->createScene(true);
	m_project->setProjectName( projectName );
	
	
// 	// Add by default a scene, layer, frame
// 	m_project->createLayer( true );
// 	m_project->createFrame( true );
}

void KTProjectManager::closeProject()
{
	if ( m_isOpen )
	{
		m_project->clear();
	}
	
	m_isOpen = false;
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
void KTProjectManager::handleProjectEvent(KTProjectEvent *event)
{
	D_FUNCINFO;
	emit commandExecuted(event);
}


/**
 * Se crea un comando para realizar una accion, por ejemplo añadir frame, este mismo comando tiene la información necesaria para revertir su efecto.
 * Normalmente este comando debe ser añadido a una pila de comandos.
 * El comando creado no es borrado por esta clase, la responsabilidad de borrarlo esta en el usuario.
 * @param event 
 * @return 
 */
KTProjectCommand *KTProjectManager::createCommand(const KTProjectEvent *event)
{
	D_FUNCINFO;
	KTProjectCommand *command = new KTProjectCommand(m_project, event);
	
	return command;
}







