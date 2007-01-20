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

#include <QCoreApplication>

#include <ddebug.h>
#include "server.h"
#include "packagehandler.h"


#include <dapplicationproperties.h>
#include <dconfig.h>

int main(int argc, char **argv)
{
	DDebug::setForceDisableGUI();
	
	QCoreApplication app(argc, argv);
	app.setApplicationName("ktoond");
	
	if ( !DCONFIG->isOk() )
	{
		DCONFIG->beginGroup("Connection");
		DCONFIG->setValue("host", "127.0.0.1");
		DCONFIG->setValue("port", "6502");
		
		DCONFIG->beginGroup("General");
		DCONFIG->setValue("repository", dAppProp->configDir()+ "/repository");
		
		
		DCONFIG->sync();
	}
	
	DCONFIG->beginGroup("Connection");	
	QString host = DCONFIG->value("host").toString();
	int port = DCONFIG->value("port").toInt();
	
	
	DCONFIG->beginGroup("General");
	dAppProp->setCacheDir(DCONFIG->value("repository").toString());
	
	QDir cache(dAppProp->cacheDir());
	if(!cache.exists ())
	{
		cache.mkdir(dAppProp->cacheDir());
	}
	
	Server::TcpServer server;
	server.createHandler<PackageHandler>();
	
	
	server.openConnection( host, port );
	
	dDebug() << "Running!";
	
	return app.exec();
}




