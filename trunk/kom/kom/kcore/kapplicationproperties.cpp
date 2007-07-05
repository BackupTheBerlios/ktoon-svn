/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
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

#include "kapplicationproperties.h"

#include <QDir>
#include <QLocale>

#include <QApplication>

DApplicationProperties *DApplicationProperties::s_instance = 0;

struct DApplicationProperties::Private
{
	QString homeDir;
	QString dataDir;
	QString themeDir;
	QString version;
	QString cacheDir;
};

DApplicationProperties::DApplicationProperties() : d(new Private)
{
}


DApplicationProperties::~DApplicationProperties()
{
	delete d;
}

void DApplicationProperties::setDataDir(const QString &v)
{
	d->dataDir = v;
}

void DApplicationProperties::setHomeDir(const QString &v)
{
	d->homeDir = v;
}

void DApplicationProperties::setThemeDir(const QString &v)
{
	d->themeDir = v;
}

void DApplicationProperties::setCacheDir(const QString &v)
{
	d->cacheDir = v;
}

void DApplicationProperties::setVersion(const QString &v)
{
	d->version = v;
}

QString DApplicationProperties::dataDir() const
{
	if ( d->dataDir.isEmpty() )
	{
		QString locale = QString(QLocale::system().name()).left(2);
	
		if ( locale.length() < 2 )
		{
			locale = "en";
		}
		
		return d->homeDir+"/data/"+locale+"/";
	}
	
	return d->dataDir;
}

QString DApplicationProperties::homeDir() const
{
	return d->homeDir;
}

QString DApplicationProperties::themeDir() const
{
	if ( d->themeDir.isEmpty() )
	{
		return d->homeDir+"/themes/default";
	}
	
	return d->themeDir;
}

QString DApplicationProperties::configDir() const
{
	return QDir::homePath()+"/."+qApp->applicationName();
}

QString DApplicationProperties::cacheDir() const
{
	return d->cacheDir;
}

QString DApplicationProperties::version() const
{
	return d->version;
}

DApplicationProperties *DApplicationProperties::instance()
{
	if (s_instance == 0 )
	{
		s_instance = new DApplicationProperties;
	}
	
	return s_instance;
}


