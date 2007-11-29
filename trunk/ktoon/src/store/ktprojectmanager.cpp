/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktscene.h"
#include "ktlayer.h"
#include "ktframe.h"


#include "ktprojectrequest.h"
#include "ktprojectcommand.h"
#include "ktcommandexecutor.h"

#include "ktprojectmanagerparams.h"

#include "ktabstractprojectmanagerhandler.h"

#include "ktprojectresponse.h"
#include "ktrequestbuilder.h"
#include "ktrequestparser.h"

#include <kcore/kalgorithm.h>
#include <kcore/kdebug.h>


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
		
		QString copyFrame;
};

KTProjectManager::KTProjectManager(QObject *parent) : QObject(parent), k(new Private())
{
	KINIT;
	
	k->isModified = false;
	k->handler = 0;
	
	k->project = new KTProject(this);
	k->undoStack = new QUndoStack(this);
	
	k->commandExecutor = new KTCommandExecutor(k->project);
	
	connect(k->commandExecutor, SIGNAL(responsed( KTProjectResponse* )), this, SLOT(emitResponse( KTProjectResponse *)));
	
	connect(k->project, SIGNAL(responsed(KTProjectResponse*)), this, SIGNAL(responsed(KTProjectResponse *)));
}


KTProjectManager::~KTProjectManager()
{
	KEND;
	delete k;
}

void KTProjectManager::setParams(KTProjectManagerParams *params)
{
	if ( k->params ) delete k->params;
	k->params = params;
	
	k->handler->initialize(k->params);
}

KTProjectManagerParams *KTProjectManager::params() const
{
	return k->params;
}

void KTProjectManager::setHandler(KTAbstractProjectHandler *handler)
{
	if ( k->handler )
	{
		disconnect(k->handler, SIGNAL(sendCommand(const KTProjectRequest *, bool)), this, SLOT(createCommand(const KTProjectRequest *, bool)));
		disconnect(k->handler, SIGNAL(sendLocalCommand(const KTProjectRequest *)), this, SLOT(handleLocalRequest(const KTProjectRequest *)));
		
		delete k->handler;
		k->handler = 0;
	}
	
	k->handler = handler;
	k->handler->setParent(this);
	
	k->handler->setProject(k->project);
	
	connect(k->handler, SIGNAL(sendCommand(const KTProjectRequest *, bool)), this, SLOT(createCommand(const KTProjectRequest *, bool)));
	connect(k->handler, SIGNAL(sendLocalCommand(const KTProjectRequest *)), this, SLOT(handleLocalRequest(const KTProjectRequest *)));
}

KTAbstractProjectHandler *KTProjectManager::handler() const
{
	return k->handler;
}

void KTProjectManager::setupNewProject()
{
	K_FUNCINFO;
	
	if ( !k->handler || !k->params)
	{
		qDebug("ERROR: HANDLER!");
		return;
	}
	
	closeProject();
	
	k->project->setProjectName( k->params->projectName() );
	k->project->setAuthor( k->params->author() );
	
	if ( ! k->handler->setupNewProject(k->params) )
	{
		qDebug("ERROR WHILE SETUP PROJECT");
		return;
	}
	
	k->project->setOpen(true);
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
	if ( !k->handler ) return;
	
	if (  k->project->isOpen() )
	{
		if ( ! k->handler->closeProject() )
		{
			qDebug("ERROR: WHILE CLOSING THE PROJECT");
			return;
		}
		
		k->project->clear();
	}
	
	
	k->project->setOpen(false);
	k->isModified = false;
	
	k->undoStack->clear();
}

bool KTProjectManager::saveProject(const QString &fileName)
{
	bool result = k->handler->saveProject(fileName, k->project);
	
	k->isModified = !result;
	
	return result;
}

bool KTProjectManager::loadProject(const QString &fileName)
{
	if ( ! k->handler )
	{
		kFatal() << "NO HANDLER!";
		return false;
	}
	
	bool ok = k->handler->loadProject(fileName, k->project);
	
	if ( ok )
	{
		k->project->setOpen(true);
		k->isModified = false;
	}
	
	return ok;
}

/**
 * Retorna verdadero si el proyecto esta abierto
 */
bool KTProjectManager::isOpen() const
{
	return k->project->isOpen();
}

bool KTProjectManager::isModified() const
{
	return k->isModified;
}


bool KTProjectManager::isValid() const
{
	if ( !k->handler ) return false;
	
	return k->handler->isValid();
}

void KTProjectManager::setupProjectDir()
{
	QString dataDir = CACHE_DIR + "/" + (k->project->projectName().isEmpty() ? KAlgorithm::randomString(6) : k->project->projectName());
	
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
 * Esta funci�n es ejecutada cuando un evento es disparado por el proyecto.
 * Debe reimplementarse si se quiere dar un trato distinto al evento, como por ejemplo enviarlo por la red.
 * Por defecto, envia el evento por medio del signal commandExecuted
 * @param event 
 */
void KTProjectManager::handleProjectRequest(const KTProjectRequest *request)
{
	K_FUNCINFO;
	
	kWarning() << request->xml();
	
	// TODO: el handler debe decir cuando construir el comando
	
	if ( k->handler )
	{
		k->handler->handleProjectRequest( request );
	}
	else
	{
		qDebug("ERROR: NO HANDLER");
	}
}

void KTProjectManager::handleLocalRequest(const KTProjectRequest *request)
{
	KTRequestParser parser;
	
	if( parser.parse( request->xml()) )
	{
		if(KTFrameResponse *response = static_cast<KTFrameResponse *>(parser.response()))
		{
			int scenePos = response->sceneIndex();
			int layerPos = response->layerIndex();
			int position = response->frameIndex();
			
			if(response->action() == KTProjectRequest::Copy)
			{
				KTScene *scene = k->project->scene(scenePos);
				if ( scene )
				{
					KTLayer *layer = scene->layer( layerPos );
					if ( layer )
					{
						KTFrame *frame = layer->frame( position );
						if ( frame )
						{
							QDomDocument doc;
							doc.appendChild(frame->toXml( doc ));
							k->copyFrame = doc.toString(0);
							
							response->setArg( k->copyFrame );
							
						}
					}
				}
			}
			else if(response->action() == KTProjectRequest::Paste)
			{
				response->setArg(k->copyFrame);
				handleProjectRequest( & KTRequestBuilder::fromResponse( response ));
				return;
			}
		}
		parser.response()->setExternal(request->isExternal());
		emit responsed(parser.response());
	}
}


/**
 * Se crea un comando para realizar una accion, por ejemplo a�adir frame, este mismo comando tiene la informaci�n necesaria para revertir su efecto.
 * Normalmente este comando debe ser a�adido a una pila de comandos.
 * El comando creado no es borrado por esta clase, la responsabilidad de borrarlo esta en el usuario.
 * @param event 
 * @return 
 */
void KTProjectManager::createCommand(const KTProjectRequest *request, bool addToStack)
{
	K_FUNCINFO;
	
	if ( request->isValid() )
	{
		KTProjectCommand *command = new KTProjectCommand(k->commandExecutor, request);
		
		if ( addToStack )
		{
			k->undoStack->push(command);
		}
		else
		{
			command->redo();
		}
	}
	else
	{
		kWarning() << "invalid request";
	}
}


KTProject *KTProjectManager::project() const
{
	return k->project;
}


QUndoStack *KTProjectManager::undoHistory() const
{
	return k->undoStack;
}


void KTProjectManager::emitResponse( KTProjectResponse *response)
{
	K_FUNCINFO << response->action();
	
	if( response->action() != KTProjectRequest::Select )
	{
		k->isModified = true;
	}
	
	if ( !k->handler )
	{
		emit responsed( response );
	}
	else if ( k->handler->commandExecuted(response ) )
	{
		emit responsed( response );
	}
}


