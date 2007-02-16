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

namespace Registers {

struct Database::Private
{
	QString fileName;
	
	QString emailToFind;
	QHash<QString, QString> entry;
	
	QDomDocument loadDatabase()
	{
		QDomDocument doc;
		
		QFile f(fileName);
		if( f.exists() )
		{
			if( f.open(QIODevice::ReadOnly | QIODevice::Text) )
			{
				doc.setContent(f.readAll());
			}
		}
		
		return doc;
	}
	
	void save(const QDomDocument &doc)
	{
		QFile f(fileName);
		if( f.open(QIODevice::WriteOnly | QIODevice::Text) )
		{
			QTextStream ts(&f);
			ts << doc.toString(0) << endl;
		}
	}
};

Database::Database(const QString &dbfile) : d(new Private)
{
	d->fileName = dbfile;
}


Database::~Database()
{
	delete d;
}


QString Database::fileName() const
{
	return d->fileName;
}

void Database::removeRegister(const QString &email)
{
	QDomDocument db = d->loadDatabase();
	
	QDomElement root = db.documentElement();

	QDomNode reg = root.firstChild();
	bool ok = false;
	
	while(!reg.isNull())
	{
		QDomElement e = reg.toElement();
		if(!e.isNull())
		{
			QDomNode data = reg.firstChild();
			while(!data.isNull())
			{
				QDomElement e2 = data.toElement();
				
				if(!e2.isNull())
				{
					if( e2.tagName() == "email" )
					{
						if( e2.attribute("value") == email )
						{
							root.removeChild(reg);
							ok = true;
							
							break;
						}
					}
				}
				data = data.nextSibling();
			}
		}
		
		if(ok )	break;
		
		reg = reg.nextSibling();
	}
	
	if(ok)
	{
		d->save(db);
	}
	
}

bool Database::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if( tag == "login" )
	{
		d->entry["login"] = atts.value("value");
	}
	else if ( tag == "name" )
	{
		d->entry["name"] = atts.value("value");
	}
	else if ( tag == "email" )
	{
		d->entry["email"] = atts.value("value");
	}
	
	return true;
}
bool Database::endTag(const QString &tag)
{
	if( tag == "register" )
	{
		if( d->entry["email"] == d->emailToFind )
		{
			setIgnore(true);
		}
	}
	
	return true;
}
void Database::text(const QString &)
{
}

QHash<QString, QString> Database::findRegisterByEmail(const QString &email)
{
	d->emailToFind = email;
	QFile f(d->fileName);
	if( f.exists() )
	{
		if( f.open(QIODevice::ReadOnly | QIODevice::Text) )
		{
			QByteArray data = f.readAll();
			f.close();
			parse(data);
		}
	}
	
	return d->entry;
}

}


