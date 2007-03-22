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
 
#include "comunicationmanager.h"
//base
#include "base/package.h"
#include "base/settings.h"
#include "base/logger.h"

//Qt
#include <QDomDocument>

//server/core
#include "core/server.h"
#include "core/connection.h"

//server/users
#include "users/user.h"

namespace Comunications {
	
Manager::Manager()
 : Base::Observer()
{
}


Manager::~Manager()
{
}

void Manager::handlePackage(Base::Package *const pkg)
{
	
	Server::Connection *cnn = pkg->source();
	Server::TcpServer *server = cnn->server();
	
	if ( pkg->root() == "chat" )
	{
		QDomDocument doc;
		doc.setContent(pkg->xml());
		
		QDomElement element = doc.firstChild().firstChildElement("message");
		element.setAttribute("from", cnn->user()->login());
		cnn->sendToAll(doc);
		pkg->accept();
	}
	else if ( pkg->root() == "notice" )
	{
		QDomDocument doc;
		doc.setContent(pkg->xml());
		
		QDomElement element = doc.firstChild().firstChildElement("message");
		element.setAttribute("from", cnn->user()->login());
		cnn->sendToAll(doc);//TODO: enviar a todos los clientes del proyecto
		pkg->accept();
	}
	else if ( pkg->root() == "wall" )
	{
		QDomDocument doc;
		doc.setContent(pkg->xml());
		QDomElement element = doc.firstChild().firstChildElement("message");
		element.setAttribute("from", cnn->user()->login());
		cnn->sendToAll(doc);
		pkg->accept();
	}
}

}
