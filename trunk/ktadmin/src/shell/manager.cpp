/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "manager.h"

#include "socket.h"
#include "modulewidgetbase.h"
#include "package.h"

#include <ddebug.h>
#include <dosd.h>

#include "packages/connect.h"
#include "packages/ackparser.h"

#include <QList>

struct Manager::Private
{
	Socket *socket;
	QList<Base::ModuleWidgetBase *> observers;
	
	bool enabled;
	
	QString sign;
};

Manager::Manager(QObject *parent) : QObject(parent), d(new Private)
{
	d->socket = new Socket(this);
	
	connect(d->socket, SIGNAL(connected()), this, SLOT(enable()));
	connect(d->socket, SIGNAL(disconnected()), this, SLOT(disable()));
}


Manager::~Manager()
{
	delete d;
}

void Manager::enable()
{
	d->enabled = true;
}

void Manager::disable()
{
	d->enabled = false;
}

void Manager::sendPackage(const QString &pkg)
{
	if ( d->enabled )
	{
		dDebug() << "sending " << pkg;
		d->socket->send(pkg);
	}
}

void Manager::handlePackage(const QString &root, const QString &xml)
{
	if ( !tryToHandle(root, xml) )
	{
		Base::Package *package = new Base::Package(root, xml);
		
		foreach(Base::ModuleWidgetBase *observer, d->observers)
		{
			observer->handlePackage(package);
			if( package->isAccepted() )
				break;
		}
		
		delete package;
	}
}

bool Manager::tryToHandle(const QString &root, const QString &xml)
{
	if( root == "ack" )
	{
		AckParser parser;
		if ( parser.parse(xml) )
		{
			DOsd::self()->display(parser.motd());
			d->sign = parser.sign();
			
			return true;
		}
	}
	
	
	return false;
}

void Manager::addObserver(Base::ModuleWidgetBase *obs)
{
	d->observers << obs;
}

void Manager::removeObserver(Base::ModuleWidgetBase *obs)
{
	d->observers.removeAll(obs);
}

bool Manager::connectToServer(const QString &server, int port)
{
	d->socket->connectToHost(server, port);
	
	return d->socket->waitForConnected(1000);
}


void Manager::authenticate(const QString &login, const QString &password)
{
	Packages::Connect cnn(login, password);
	d->socket->send(cnn);
}



