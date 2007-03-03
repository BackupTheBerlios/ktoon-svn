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
#include "packageobserver.h"

#include <ddebug.h>
#include <dosd.h>

#include "packages/connect.h"
#include "packages/ackparser.h"

#include <QList>

struct Manager::Private
{
	Socket *socket;
	QList<Base::PackageObserver *> observers;
	
	bool enabled;
	
	QString sign;
};

Manager::Manager(QObject *parent) : QObject(parent), d(new Private)
{
	d->socket = new Socket(this);
	
// 	connect(d->socket, SIGNAL(connected()), this, SLOT(enable()));
	connect(d->socket, SIGNAL(disconnected()), this, SLOT(disable()));
}


Manager::~Manager()
{
	delete d;
}

bool Manager::isEnabled() const
{
	return d->enabled;
}

void Manager::enable()
{
	d->enabled = true;
}

void Manager::disable()
{
	DOsd::self()->display(tr("Disconnected!"), DOsd::Warning);
	d->enabled = false;
	
	emit disconnected();
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
		
		foreach(Base::PackageObserver *observer, d->observers)
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
		Packages::AckParser parser;
		if ( parser.parse(xml) )
		{
			DOsd::self()->display(parser.motd());
			d->sign = parser.sign();
			
			d->enabled = true;
			emit connected();
			
			return true;
		}
	}
	
	
	return false;
}

void Manager::addObserver(Base::PackageObserver *obs)
{
	d->observers << obs;
}

void Manager::removeObserver(Base::PackageObserver *obs)
{
	d->observers.removeAll(obs);
}

bool Manager::connectToServer(const QString &server, int port)
{
	if ( d->socket->state() == QAbstractSocket::ConnectedState )
	{
		d->socket->disconnectFromHost();
		d->socket->waitForDisconnected();
	}
	
	d->socket->clearQueue();
	
	d->socket->connectToHost(server, port);
	
	bool connected =  d->socket->waitForConnected(1000);
	if( connected )
	{
		DOsd::self()->display(tr("Connected!"));
	}
	else
	{
		DOsd::self()->display(tr("Cannot connect to server!"), DOsd::Error);
	}
	
	return connected;
}


void Manager::authenticate(const QString &login, const QString &password)
{
	Packages::Connect cnn(login, password);
	d->socket->send(cnn);
}



