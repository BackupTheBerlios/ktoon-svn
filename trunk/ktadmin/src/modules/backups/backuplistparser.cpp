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

#include "backuplistparser.h"
#include <QHash>

#include <dcore/ddebug.h>

namespace Backups {

struct BackupListParser::Private
{
	QString current;
	QHash<QString, QStringList> backups;
};

BackupListParser::BackupListParser()
 : KTXmlParserBase(), d(new Private)
{
}


BackupListParser::~BackupListParser()
{
	delete d;
}

void BackupListParser::initialize()
{
	d->backups.clear();
}

bool BackupListParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if( tag == "entry" )
	{
		d->current = atts.value("name");
	}
	else if ( tag == "backup" )
	{
		d->backups[d->current] << atts.value("date");
	}
	return true;
}

bool BackupListParser::endTag(const QString &)
{
	return true;
}

void BackupListParser::text(const QString &)
{
}

QStringList BackupListParser::backups(const QString &project) const
{
	return d->backups[project];
}

}
