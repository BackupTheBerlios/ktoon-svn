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

#include "backupmanager.h"
#include "settings.h"

#include <QDir>
#include <QFileInfo>
#include <QDateTime>

#include <ddebug.h>

namespace Server {

struct BackupManager::Private
{
	QString dbfile;
	BackupDatabase *database;
};

BackupManager::BackupManager() : d(new Private)
{
	d->dbfile = Settings::self()->databaseDirPath()+"/backups.xml";
	
	QDir dir(Settings::self()->backupDirPath());
	if( ! dir.exists() )
	{
		dir.mkpath(Settings::self()->backupDirPath());
	}
	
	d->database = new BackupDatabase(d->dbfile);
}


BackupManager::~BackupManager()
{
	delete d;
}

bool BackupManager::makeBackup(const QString &filepath)
{
	QFileInfo fi(filepath);
	
	if( fi.exists() )
	{
		QDateTime date = QDateTime::currentDateTime();
		
		QString destfile = fi.baseName()+"-"+date.toString(Qt::ISODate);
		
		if( ! fi.completeSuffix().isEmpty() )
		{
			destfile += "."+fi.completeSuffix();
		}
		
		QFile file(filepath);
		
		if ( file.copy(Settings::self()->backupDirPath()+"/"+ destfile) )
		{
			d->database->addEntry(destfile, fi.baseName(), date);
			
			return true;
		}
	}
	
	return false;
}

QHash<QString, QList<BackupDatabase::Entry> > BackupManager::entries()
{
	return d->database->entries();
}

}
