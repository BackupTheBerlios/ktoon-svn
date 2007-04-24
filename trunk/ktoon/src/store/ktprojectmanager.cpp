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
#include <QDir>

#include "ktproject.h"
#include "ktprojectrequest.h"
#include "ktprojectcommand.h"
#include "ktcommandexecutor.h"

#include "ktprojectmanagerparams.h"

#include "ktabstractprojectmanagerhandler.h"

#include "ktprojectresponse.h"
#include "ktrequestbuilder.h"
#include "ktrequestparser.h"

#include <dcore/dalgorithm.h>
#include <dcore/ddebug.h>


class KTProjectManager::Private
{
	public:
		Private() : handler(0), params(0)
		{
		}
		~Private()
		{
			delete handler;
			delete undoStack;
			delete commandExecutor;
			delete params;
		}
		
	public:
		KTProject *project;
		bool isModified;
		KTAbstractProjectHandler *handler;
		QUndoStack *undoStack;
		KTCommandExecutor *commandExecutor;
		
		KTProjectManagerParams *params;
};

KTProjectManager::KTProjectManager(QObject *parent) : QObject(parent), d(new Private())
{
	DINIT;
	
	d->isModified = false;
	d->handler = 0;
	
	d->project = new KTProject(this);
	d->undoStack = new QUndoStack(this);
	
	d->commandExecutor = new KTCommandExecutor(d->project);
	
	connect(d->commandExecutor, SIGNAL(responsed( KTProjectResponse* )), this, SLOT(emitResponse( KTProjectResponse *)));
	
	connect(d->project, SIGNAL(responsed(KTProjectResponse*)), this, SIGNAL(responsed(KTProjectResponse *)));
}


KTProjectManager::~KTProjectManager()
{
	DEND;
	delete d;
}

void KTProjectManager::setParams(KTProjectManagerParams *params)
{
	if ( d->params ) delete d->params;
	d->params = params;
	
	d->handler->initialize(d->params);
}

KTProjectManagerParams *KTProjectManager::params() const
{
	return d->params;
}

void KTProjectManager::setHandler(KTAbstractProjectHandler *handler)
{
	if ( d->handler )
	{
		disconnect(d->handler, SIGNAL(sendCommand(const KTProjectRequest *, bool)), this, SLOT(createCommand(const KTProjectRequest *, bool)));
		disconnect(d->handler, SIGNAL(sendLocalCommand(const KTProjectRequest *)), this, SLOT(handleLocalRequest(const KTProjectRequest *)));
		
		delete d->handler;
		d->handler = 0;
	}
	
	d->handler = handler;
	d->handler->setParent(this);
	
	d->handler->setProject(d->project);
	
	connect(d->handler, SIGNAL(sendCommand(const KTProjectRequest *, bool)), this, SLOT(createCommand(const KTProjectRequest *, bool)));
	connect(d->handler, SIGNAL(sendLocalCommand(const KTProjectRequest *)), this, SLOT(handleLocalRequest(const KTProjectRequest *)));
}

KTAbstractProjectHandler *KTProjectManager::handler() const
{
	return d->handler;
}

void KTProjectManager::setupNewProject()
{
	D_FUNCINFO;
	
	if ( !d->handler || !d->params)
	{
		qDebug("ERROR: HANDLER!");
		return;
	}
	
	closeProject();
	
	d->project->setProjectName( d->params->projectName() );
	d->project->setAuthor( d->params->author() );
	
	if ( ! d->handler->setupNewProject(d->params) )
	{
		qDebug("ERROR WHILE SETUP PROJECT");
		return;
	}
	
	d->project->setOpen(true);
	setupProjectDir();
	
	KTProjectRequest request = KTRequestBuilder::createSceneRequest(0, KTProjectRequest::Add, QString());
	
	handleProjectRequest(&request);
	
	request = KTRequestBuilder::createLayerRequest(0, 0, KTProjectRequest::Add, QString());
	
	handleProjectRequest(&request);
	
	request = KTRequestBuilder::createFrameRequest(0, 0, 0, KTProjectRequest::Add, QString());
	handleProjectRequest(&request);
}


void KTProjectManager::closeProject()
{
	if ( !d->handler ) return;
	
	if (  d->project->isOpen() )
	{
		if ( ! d->handler->closeProject() )
		{
			qDebug("ERROR: WHILE CLOSING THE PROJECT");
			return;
		}
		
		d->project->clear();
	}
	
	
	d->project->setOpen(false);
	d->isModified = false;
	
	d->undoStack->clear();
}

bool KTProjectManager::saveProject(const QString &fileName)
{
	bool result = d->handler->saveProject(fileName, d->project);
	
	d->isModified = !result;
	
	return result;
}

bool KTProjectManager::loadProject(const QString &fileName)
{
	if ( ! d->handler )
	{
		dFatal() << "NO HANDLER!";
		return false;
	}
	
	bool ok = d->handler->loadProject(fileName, d->project);
	
	if ( ok )
	{
		d->project->setOpen(true);
		d->isModified = false;
	}
	
	return ok;
}

/**
 * Retorna verdadero si el proyecto esta abierto
 */
bool KTProjectManager::isOpen() const
{
	return d->project->isOpen();
}

bool KTProjectManager::isModified() const
{
	return d->isModified;
}


bool KTProjectManager::isValid() const
{
	if ( !d->handler ) return false;
	
	return d->handler->isValid();
}

void KTProjectManager::setupProjectDir()
{
	QString dataDir = CACHE_DIR + "/" + (d->project->projectName().isEmpty() ? DAlgorithm::randomString(6) : d->project->projectName());
	
	QDir project = dataDir;
	
	if( !project.exists() )
	{
		if( project.mkpath(project.absolutePath()) )
		{
			QStringList dirs;
			dirs << "audio" << "images" << "video";
			
			foreach(QString dir, dirs)
			{
				project.mkdir(dir);
			}
		}
	}
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
	
	if ( d->handler )
	{
		d->handler->handleProjectRequest( request );
	}
	else
	{
		qDebug("ERROR: NO HANDLER");
	}
}

void KTProjectManager::handleLocalRequest(const KTProjectRequest *request)
{
	KTRequestParser parser;
	parser.parse( request->xml());
	
	emit responsed(parser.response());
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
		KTProjectCommand *command = new KTProjectCommand(d->commandExecutor, request);
		
		if ( addToStack )
		{
			d->undoStack->push(command);
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
	return d->project;
}


QUndoStack *KTProjectManager::undoHistory() const
{
	return d->undoStack;
}


void KTProjectManager::emitResponse( KTProjectResponse *response)
{
	D_FUNCINFO << response->action();
	
	if( response->action() != KTProjectRequest::Select )
	{
		d->isModified = true;
	}
	
	if ( !d->handler )
	{
		emit responsed( response );
	}
	else if ( d->handler->commandExecuted(response ) )
	{
		emit responsed( response );
	}
}


