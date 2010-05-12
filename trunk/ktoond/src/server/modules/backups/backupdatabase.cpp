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

#include "backupdatabase.h"

#include <QDomDocument>
#include <QTextStream>
#include <QHash>

#include <kcore/kdebug.h>

namespace Backups {

struct Database::Private
{
	QString dbfile;
	
	QHash<QString, QList<Database::Entry> > entries;
	QString current;
};

Database::Database(const QString &file)
 : KTXmlParserBase(), d(new Private)
{
	d->dbfile = file;
	
	QFile f(file);
	
	if( !f.exists() )
	{
		QDomDocument doc;
		QDomElement root = doc.createElement("backups");
		doc.appendChild(root);
		
		f.open(QIODevice::WriteOnly | QIODevice::Text);
		
		QTextStream ts(&f);
		ts << doc.toString();
		
		f.close();
	}
}


Database::~Database()
{
	delete d;
}

bool Database::startTag(const QString &tag, const QXmlAttributes &atts)
{
	if( tag == "entry" )
	{
		d->current = atts.value("name");
	}
	else if ( tag == "backup" )
	{
		Entry entry;
		entry.date = QDateTime::fromString(atts.value("date"), Qt::ISODate);
		entry.file = atts.value("file");
		entry.origin = atts.value("origin");
		
		d->entries[d->current] << entry;
	}
	return true;
}

bool Database::endTag(const QString &)
{
	return true;
}

void Database::text(const QString &)
{
}

bool Database::addEntry(const QString &origFile, const QString &filename, const QString &name, const QDateTime &date)
{
	QDomDocument doc;
	QFile dbf(d->dbfile);
	
	if( doc.setContent(&dbf) )
	{
		dbf.close();
		
		QDomElement root = doc.documentElement();
		QDomNode n = root.firstChild();
		
		QDomElement target;
		
		while(!n.isNull())
		{
			QDomElement e = n.toElement();
			
			if(!e.isNull())
			{
				if( e.attribute("name") == name )
				{
					target = e;
				}
			}
			
			n = n.nextSibling();
		}
		
		if( target.isNull() )
		{
			target = doc.createElement("entry");
			target.setAttribute("name", name);
			root.appendChild(target);
		}
		
		QDomElement entry = doc.createElement("backup");
		entry.setAttribute("date", date.toString(Qt::ISODate) );
		entry.setAttribute("file", filename);
		entry.setAttribute("origin", origFile);
		
		target.appendChild(entry);
		
		dbf.open(QIODevice::WriteOnly | QIODevice::Text);
		
		QTextStream ts(&dbf);
		
		ts << doc.toString();
		
		dbf.close();
		
		return true;
	}
	
	return false;
}

bool Database::removeEntry(const QString &name, const QDateTime &date)
{
	QDomDocument doc;
	QFile dbf(d->dbfile);
	
	if( doc.setContent(&dbf) )
	{
		dbf.close();
		
		QDomElement root = doc.documentElement();
		QDomNode n = root.firstChild();
		QDomElement target;
		
		while(!n.isNull())
		{
			QDomElement e = n.toElement();
			
			if(!e.isNull())
			{
				if( e.attribute("name") == name )
				{
					target = e;
				}
			}
			
			n = n.nextSibling();
		}
		
		if( target.isNull() ) return false;
		
		QString dateStr = date.toString(Qt::ISODate);
		
		n = target.firstChild();
		while(!n.isNull())
		{
			QDomElement e = n.toElement();
			if(!e.isNull())
			{
				kDebug() << e.tagName();
				
				if ( e.attribute("date") == dateStr )
				{
					target.removeChild(e);
				}
			}
			n = n.nextSibling();
		}
		
		
		dbf.open(QIODevice::WriteOnly | QIODevice::Text);
		
		QTextStream ts(&dbf);
		
		ts << doc.toString();
		
		dbf.close();
		
		return true;
	}
	
	return false;
}

QHash<QString, QList<Database::Entry> > Database::entries()
{
	d->entries.clear();
	
	QFile f(d->dbfile);
	f.open(QIODevice::ReadOnly | QIODevice::Text);
	
	parse(f.readAll());
	f.close();
	
	return d->entries;
}

}


