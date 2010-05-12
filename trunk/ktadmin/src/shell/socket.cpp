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

#include "socket.h"

#include <kcore/kdebug.h>
#include "manager.h"

struct Socket::Private 
{
	Manager *manager;
};

Socket::Socket(Manager *manager,QObject *parent) : KTSocketBase(parent), d(new Private)
{
	d->manager = manager;
}


Socket::~Socket()
{
	delete d;
}

void Socket::readed(const QString &readed)
{
	kDebug("net") << "READED: " << readed;
	QDomDocument doc;
	
	if ( doc.setContent(readed) )
	{
		QString root = doc.documentElement().tagName();
		
		d->manager->handlePackage( root, readed);
	}
	else
	{
		qDebug("Isn't a document");
	}
}


