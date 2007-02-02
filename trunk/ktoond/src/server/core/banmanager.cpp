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

#include "banmanager.h"

#include <QHash>
#include <QString>
#include <QApplication>

#include <ddebug.h>

namespace Server {

struct BanManager::Private
{
	int maxFails;
	QHash<QString, int> fails;
};

BanManager *BanManager::s_self = 0;

BanManager::BanManager(QObject *parent) : QObject(parent), d(new Private)
{
	d->maxFails = 10;
	
	d->fails.insert("hola", 11);
	d->fails.insert("prueba", 11);
	d->fails.insert("hotr", 11);
	d->fails.insert("banned", 11);
}


BanManager::~BanManager()
{
	delete d;
}

void BanManager::initialize(const QString &pt)
{
	if ( !d->fails.contains(pt) )
		d->fails.insert(pt, 0);
}

bool BanManager::isBanned(const QString &pt) const
{
	if ( d->fails.contains(pt) )
	{
		if ( d->fails[pt] >= d->maxFails )
		{
			return true;
		}
		
	}
	return false;
}

void BanManager::failed(const QString &pt)
{
	dDebug() << "FAILED: " << pt;
	d->fails[pt] += 1;
	
	dDebug() << "COUNT: " << d->fails[pt];
}

void BanManager::ban(const QString &pt)
{
	d->fails[pt] = d->maxFails;
}

void BanManager::unban(const QString &pt)
{
	d->fails[pt] = 0;
}

QStringList BanManager::allBanned() const
{
	QStringList allBanned;
	
	QHash<QString, int>::const_iterator i = d->fails.constBegin();
	
	while (i != d->fails.constEnd())
	{
		if ( isBanned(i.key()))
			allBanned << i.key();
		++i;
	}
	
	return allBanned;
}

BanManager *BanManager::self()
{
	if ( !s_self)
		s_self = new BanManager(qApp);
	
	return s_self;
}

}




