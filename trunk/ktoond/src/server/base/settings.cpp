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

#include "settings.h"

namespace Base {

Settings *Settings::s_settings = 0;

struct Settings::Private 
{
	QString databaseDirPath;
	QString backupDirPath;
};

Settings::Settings() : d(new Private())
{
}


Settings::~Settings()
{
	delete d;
}

Settings *Settings::self()
{
	if( ! s_settings )
		s_settings = new Settings();
	
	return s_settings;
}


void Settings::setDatabaseDirPath(const QString &dbdir)
{
	d->databaseDirPath = dbdir;
}

QString Settings::databaseDirPath() const
{
	return d->databaseDirPath;
}

void Settings::setBackupDirPath(const QString &dir)
{
	d->backupDirPath = dir;
}

QString Settings::backupDirPath() const
{
	return d->backupDirPath;
}

}
