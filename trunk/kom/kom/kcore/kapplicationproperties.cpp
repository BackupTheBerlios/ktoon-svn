/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
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

#include "kapplicationproperties.h"

#include <QDir>
#include <QLocale>

#include <QApplication>

KApplicationProperties *KApplicationProperties::s_instance = 0;

struct KApplicationProperties::Private
{
	QString homeDir;
	QString dataDir;
	QString themeDir;
	QString version;
	QString cacheDir;
};

KApplicationProperties::KApplicationProperties() : d(new Private)
{
}


KApplicationProperties::~KApplicationProperties()
{
	delete d;
}

void KApplicationProperties::setDataDir(const QString &v)
{
	d->dataDir = v;
}

void KApplicationProperties::setHomeDir(const QString &v)
{
	d->homeDir = v;
}

void KApplicationProperties::setThemeDir(const QString &v)
{
	d->themeDir = v;
}

void KApplicationProperties::setCacheDir(const QString &v)
{
	d->cacheDir = v;
}

void KApplicationProperties::setVersion(const QString &v)
{
	d->version = v;
}

QString KApplicationProperties::dataDir() const
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

QString KApplicationProperties::homeDir() const
{
	return d->homeDir;
}

QString KApplicationProperties::themeDir() const
{
	if ( d->themeDir.isEmpty() )
	{
		return d->homeDir+"/themes/default";
	}
	
	return d->themeDir;
}

QString KApplicationProperties::configDir() const
{
	return QDir::homePath()+"/."+qApp->applicationName();
}

QString KApplicationProperties::cacheDir() const
{
	return d->cacheDir;
}

QString KApplicationProperties::version() const
{
	return d->version;
}

KApplicationProperties *KApplicationProperties::instance()
{
	if (s_instance == 0 )
	{
		s_instance = new KApplicationProperties;
	}
	
	return s_instance;
}


