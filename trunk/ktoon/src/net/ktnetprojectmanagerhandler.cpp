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

#include "ktnetprojectmanagerhandler.h"
#include "ktnetprojectmanagerparams.h"

#include "ktprojectresponse.h"


#include <kcore/kdebug.h>
#include <kgui/kosd.h>


#include "ktprojectcommand.h"
#include "ktcommandexecutor.h"

#include "ktnetsocket.h"

#include "ktprojectrequest.h"


#include "ktnewprojectpackage.h"
#include "ktconnectpackage.h"

#include "ktsavenetproject.h"
#include "ktopenpackage.h"
#include "ktchatpackage.h"
#include "ktnoticepackage.h"

#include "kterrorparser.h"
#include "ktprojectsparser.h"
#include "ktprojectparser.h"
#include "ktrequestparser.h"
#include "ktackparser.h"
#include "ktcomunicationparser.h"

#include "ktrequestbuilder.h"

#include "ktproject.h"

#include "ktlistprojectdialog.h"
#include "ktchat.h"
#include "ktnotice.h"

#include <QTemporaryFile>
#include <QTabWidget>

struct KTNetProjectManagerHandler::Private
{
	KTNetSocket *socket;
	QString projectName, author;
	
	KTNetProjectManagerParams *params;
	KTProject *project;
	
	QString sign;
	bool ownPackage;
	bool doAction;
	
	QTabWidget *comunicationModule;
	
	KTChat *chat;
	KTNotice *notices;
};

KTNetProjectManagerHandler::KTNetProjectManagerHandler(QObject *parent) : KTAbstractProjectHandler(parent), d(new Private)
{
	d->socket = new KTNetSocket(this);
	d->project = 0;
	d->params = 0;
	d->ownPackage = false;
	d->doAction = true;
	
	d->comunicationModule = new QTabWidget;
	d->comunicationModule->setWindowTitle(tr("Communications"));
	d->comunicationModule->setWindowIcon(QPixmap(THEME_DIR+"/icons/chat.png"));
	
	
	d->chat = new KTChat;
	d->comunicationModule->addTab(d->chat, tr("chat"));
	
	connect(d->chat, SIGNAL(requestSendMessage(const QString&)), this, SLOT(sendChatMessage(const QString&)));
	
	d->notices = new KTNotice;
	d->comunicationModule->addTab(d->notices, tr("notices"));
	
	connect(d->notices, SIGNAL(requestSendMessage(const QString&)), this, SLOT(sendNoticeMessage(const QString&)));
}


KTNetProjectManagerHandler::~KTNetProjectManagerHandler()
{
	d->chat->close();
	delete d;
}


void KTNetProjectManagerHandler::handleProjectRequest(const KTProjectRequest* request)
{
	// Esto llega desde el proyecto antes de ejecutar el comando!
	
	// TODO: Guardar una copia de los eventos o paquetes en una cola y reenviar a la GUI cuando llegue el paquete de que todo va bien desde el servidor!
	
	if ( d->socket->state() == QAbstractSocket::ConnectedState )
	{
		dDebug("net") << "SENDING: " << request->xml();
		d->socket->send( request->xml() );
	}
}


bool KTNetProjectManagerHandler::commandExecuted(KTProjectResponse *response)
{
	if( response->mode() == KTProjectResponse::Do )
	{
		d->doAction = true;
		return true;
	}
	
	KTProjectRequest request = KTRequestBuilder::fromResponse(response);
	handleProjectRequest( &request );
	
	d->doAction = false;
	
	return false;
}

bool KTNetProjectManagerHandler::saveProject(const QString &fileName, const KTProject *project)
{
	KTSaveNetProject saver(d->params->server(), d->params->port());
	
	return saver.save(fileName, project);
}

bool KTNetProjectManagerHandler::loadProject(const QString &fileName, KTProject *project)
{
	if ( d->socket->state() != QAbstractSocket::ConnectedState  )
		return false;
	
	return loadProjectFromServer(d->params->projectName());
}


bool KTNetProjectManagerHandler::loadProjectFromServer(const QString &name)
{
	KTOpenPackage package(name);
	d->socket->send(package);
	return true;
}

bool KTNetProjectManagerHandler::initialize(KTProjectManagerParams *params)
{
	KTNetProjectManagerParams *netparams = dynamic_cast<KTNetProjectManagerParams*>(params);
	if ( ! netparams ) return false;
	
	d->params = netparams;
	
	dDebug("net") << "Connecting to " << netparams->server() << ":" << netparams->port();
	
	d->socket->connectToHost(d->params->server(), d->params->port());
	
	bool connected = d->socket->waitForConnected(1000);
	if(connected)
	{
		KTConnectPackage connectPackage( d->params->login(), d->params->password());
		d->socket->send( connectPackage );
	}
	
	return connected;
}

bool KTNetProjectManagerHandler::setupNewProject(KTProjectManagerParams *params)
{
	KTNetProjectManagerParams *netparams = dynamic_cast<KTNetProjectManagerParams*>(params);
	
	if ( ! netparams ) return false;
	
	SHOW_VAR(netparams->projectName());
	d->projectName = netparams->projectName();
	d->author = netparams->author();
	
	if ( ! d->socket->isOpen() )
	{
		bool connected = initialize(params);
		if ( !connected ) return false;
	}
	
	KTNewProjectPackage newProjectPackage(netparams->projectName(), netparams->author(), netparams->description() );
	d->socket->send(newProjectPackage);
	
	return true;
}



bool KTNetProjectManagerHandler::closeProject()
{
	return KTAbstractProjectHandler::closeProject();
}


void KTNetProjectManagerHandler::emitRequest(KTProjectRequest *request, bool toStack)
{
	emit sendCommand( request, toStack );
}

void KTNetProjectManagerHandler::handlePackage(const QString &root ,const QString &package )
{
	D_FUNCINFOX("net");
	if ( root == "request" )
	{
		KTRequestParser parser;
		if ( parser.parse(package) )
		{
			if ( parser.sign() == d->sign )
			{
				d->ownPackage = true;
			}
			else
			{
				d->ownPackage = false;
			}
			
			if( d->ownPackage && !d->doAction )
			{
				if( parser.response()->part() == KTProjectRequest::Item )
				{
					KTItemResponse *response = static_cast<KTItemResponse *>(parser.response());
					KTProjectRequest request = KTRequestBuilder::createFrameRequest(response->sceneIndex(), response->layerIndex(), response->frameIndex(), KTProjectRequest::Select);
					
					request.setExternal(!d->ownPackage);
					emit sendLocalCommand(&request);
				}
				return;
			}
			
			KTProjectRequest request = KTRequestBuilder::fromResponse( parser.response() );
			request.setExternal(!d->ownPackage);
			emitRequest(&request, d->doAction && d->ownPackage );
		}
		else // TODO: mostrar error
		{
			dError() << "Error parsing";
		}
	}
	else if( root == "ack")
	{
		//analizar el paquete
		KTAckParser parser;
		if ( parser.parse(package) )
		{
			d->sign = parser.sign();
			DOsd::self()->display(parser.motd(), DOsd::Info);
		}
		
	}
	else if(root == "error")
	{
		KTErrorParser parser;
		if(parser.parse(package) )
		{
			DOsd::self()->display(parser.error().message, DOsd::Level(parser.error().level));
		}
	}
	else if(root == "project")
	{
		KTProjectParser parser;
		if(parser.parse(package) )
		{
			QTemporaryFile file;
			if(file.open())
			{
				file.write(parser.data() );
				file.flush();
				
				
				if(d->project)
				{
					KTSaveProject *loader = new KTSaveProject;
					loader->load(file.fileName(), d->project);
					emit openNewArea(d->project->projectName());
					
					delete loader;
				}
			}
		}
	}
	else if(root == "projectlist")
	{
		KTProjectsParser parser;
		if(parser.parse(package) )
		{
			KTListProjectDialog dialog;
			foreach(KTProjectsParser::ProjectInfo info, parser.projectsInfo())
			{
				dialog.addProject( info.name, info.author, info.description);
			}
			if(dialog.exec () == QDialog::Accepted && !dialog.currentProject().isEmpty())
			{
				dDebug() << "opening " << dialog.currentProject() << "project";
				loadProjectFromServer(dialog.currentProject() );
			}
			else
			{
				//TODO: disconnect
			}
		}
	}
	else if (root == "chat")
	{
		KTComunicationParser parser;
		if(parser.parse(package))
		{
			d->chat->addMessage(parser.login(), parser.message());
		}
	}
	else if (root == "notice")
	{
		KTComunicationParser parser;
		if(parser.parse(package))
		{
			QString message = QObject::tr("Notice From") + ": "+ parser.login() + "\n" + parser.message();
			DOsd::self()->display(message);
			
			d->notices->addMessage(parser.login(), parser.message());
		}
	}
	else if (root == "wall")
	{
		KTComunicationParser parser;
		if(parser.parse(package))
		{
			QString message = QObject::tr("Wall From") + ": "+ parser.login() + "\n" + parser.message();
			DOsd::self()->display(message);
		}
	}
	else
	{
		dDebug("net") << "Unknown package: " << root;
	}
}


bool KTNetProjectManagerHandler::isValid() const
{
	return d->socket->state() == QAbstractSocket::ConnectedState;
}

void KTNetProjectManagerHandler::sendPackage(const QDomDocument &doc)
{
	d->socket->send(doc);
}

QTabWidget *KTNetProjectManagerHandler::comunicationWidget()
{
	return d->comunicationModule;
}

void KTNetProjectManagerHandler::setProject(KTProject *project)
{
	d->project = project;
}

void KTNetProjectManagerHandler::sendChatMessage(const QString & message)
{
	KTChatPackage package(message);
	sendPackage(package);
}


void KTNetProjectManagerHandler::sendNoticeMessage(const QString & message)
{
	KTNoticePackage package(message);
	sendPackage(package);
}

