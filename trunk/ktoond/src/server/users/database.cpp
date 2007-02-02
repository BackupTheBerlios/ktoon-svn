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

#include "database.h"
#include <QDomDocument>
#include <QFile>
#include <QTextStream>

#include <ddebug.h>

namespace Users {

struct Database::Private
{
// 	QDomDocument document;
	QString dbfile;
};

Database::Database(const QString &dbfile) : d(new Private)
{
	d->dbfile = dbfile;
// 	init(dbfile);
}


Database::~Database()
{
	delete d;
}


QDomDocument Database::loadDataBase()
{
	dDebug() << "loading database of " << d->dbfile;
	QFile file(d->dbfile);
	QDomDocument document;
	if( !file.exists() )
	{
		if ( file.open(QIODevice::WriteOnly | QIODevice::Text) )
		{
			QTextStream ts(&file);
			document.appendChild(document.createElement("projects"));
			
			ts << document.toString();
			file.close();
		}
	}
	
	if ( file.open(QIODevice::ReadOnly | QIODevice::Text) )
	{
		if ( !document.setContent(file.readAll()) )
		{
			dWarning() << file.fileName() << " is corrupted!";
		}
		file.close();
	}
	return document;
}

void Database::addUser(const Users::User &user)
{
	QDomDocument db = loadDataBase();
	QDomNode  root = db.firstChild();
	
	root.appendChild(user.toXml(db));
	
	QFile file(d->dbfile);
	
	if ( file.open(QIODevice::WriteOnly| QIODevice::Text) )
	{
		QTextStream ts(&file);
		ts << db.toString();
		file.close();
	}
}


void Database::updateUser(const Users::User &user)
{
	QDomDocument doc = loadDataBase();
	
	QDomElement docElem = doc.documentElement();

	QDomNode n = docElem.firstChild();
	while(!n.isNull())
	{
		QDomElement e = n.toElement(); 
		if(!e.isNull()) 
		{
			if(e.attribute("login") == user.login())
			{
				docElem.removeChild(n);
				docElem.appendChild(user.toXml(doc));
			}
		}
		n = n.nextSibling();
	}
	
	QFile file(d->dbfile);
	
	if ( file.open(QIODevice::WriteOnly| QIODevice::Text) )
	{
		QTextStream ts(&file);
		ts << doc.toString();
		file.close();
	}
}

void Database::removeUser(const QString &login)
{
	QDomDocument doc = loadDataBase();
	
	QDomElement docElem = doc.documentElement();
	QDomNode n = docElem.firstChild();
	while(!n.isNull())
	{
		QDomElement e = n.toElement(); 
		if(!e.isNull()) 
		{
			if(e.attribute("login") == login)
			{
				docElem.removeChild(n);
			}
		}
		n = n.nextSibling();
	}
	QFile file(d->dbfile);
	
	if ( file.open(QIODevice::WriteOnly| QIODevice::Text) )
	{
		QTextStream ts(&file);
		ts << doc.toString();
		file.close();
	}
}

}



