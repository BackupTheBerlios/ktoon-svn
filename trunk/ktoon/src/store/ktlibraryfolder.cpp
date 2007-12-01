/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#include "ktlibraryfolder.h"
#include "ktlibraryobject.h"

#include <kcore/kdebug.h>

#include "ktprojectloader.h"
#include "ktproject.h"

struct KTLibraryFolder::Private
{
	QString id;
	Folders folders;
	LibraryObjects objects;
	
	KTProject *project;
};

KTLibraryFolder::KTLibraryFolder(const QString &id, KTProject *project, QObject *parent) : QObject(parent), k(new Private)
{
	k->id = id;
	k->project = project;
}


KTLibraryFolder::~KTLibraryFolder()
{
	delete k;
}

KTLibraryObject *KTLibraryFolder::createSymbol(KTLibraryObject::Type type, const QString &name, const QByteArray &data, bool loaded)
{
	KTLibraryObject *object = new KTLibraryObject(this);
	object->setType(type);
	
	if(!object->loadData(data))
	{
		delete object;
		return 0;
	}
	
	bool ret = addObject( object, name);
	object->saveData(k->project->dataDir());
	
	if( loaded && ret )
	{
		KTProjectLoader::createSymbol(type, name, data, k->project);
	}
	
	return object;
}

bool KTLibraryFolder::addObject(KTLibraryObject *object, const QString &id)
{
	if ( !k->objects.contains(id ) )
	{
		k->objects.insert(id, object);
		object->setSymbolName(id);
		object->setParent(this);

		return true;
	}
	
	return false;
}

bool KTLibraryFolder::removeObject(const QString &id)
{
	int c = k->objects.remove(id);
	
	return c > 0;
}

void KTLibraryFolder::addFolder(KTLibraryFolder *folder)
{
	k->folders << folder;
}

bool KTLibraryFolder::moveObject(const QString &id, KTLibraryFolder *folder)
{
	if ( k->objects.contains(id) )
	{
		KTLibraryObject *object = k->objects[id];
		removeObject( id );
		
		folder->addObject( object, id);
		
		return true;
	}
	
	return false;
}

void KTLibraryFolder::setId(const QString &id)
{
	k->id = id;
}

QString KTLibraryFolder::id() const
{
	return k->id;
}

KTLibraryObject *KTLibraryFolder::findObject(const QString &id) const
{
	foreach ( QString oid, k->objects.keys())
	{
		if ( oid == id )
		{
			return k->objects[oid];
		}
	}
	
	foreach ( KTLibraryFolder *folder, k->folders )
	{
		KTLibraryObject *object = folder->findObject(id);
		
		if ( object )
		{
			return object;
		}
	}
	
	kDebug() << "Cannot find symbol with id: " << id;
	
	return 0;
}

int KTLibraryFolder::objectsCount() const
{
	return k->objects.count();
}

int KTLibraryFolder::foldersCount() const
{
	return k->folders.count();
}

KTProject *KTLibraryFolder::project() const
{
	return k->project;
}

Folders KTLibraryFolder::folders() const
{
	return k->folders;
}

LibraryObjects KTLibraryFolder::objects() const
{
	return k->objects;
}

void KTLibraryFolder::fromXml(const QString &xml )
{
	QDomDocument document;
	
	if(! document.setContent(xml) )
	{
		return;
	}
	
	QDomElement root = document.documentElement();
	QDomNode n = root.firstChild();
	
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		
		if(!e.isNull())
		{
			if( e.tagName() == "object" )
			{
				QDomDocument objectDocument;
				objectDocument.appendChild(objectDocument.importNode(n, true ));
				
				KTLibraryObject *object = new KTLibraryObject(this);
				object->fromXml(objectDocument.toString(0));
				object->loadDataFromPath(k->project->dataDir());
				
				addObject(object, object->symbolName() );
				
				QDomElement objectData = objectDocument.documentElement().firstChild().toElement();
				
				QString data;
				if( !objectData.isNull())
				{
					QTextStream ts(&data);
					ts << objectData;
				}
				
				KTProjectLoader::createSymbol(KTLibraryObject::Type(object->type()), object->symbolName(), data.toLocal8Bit(), k->project);
			}
			else if( e.tagName() == "folder" )
			{
				QDomDocument folderDocument;
				folderDocument.appendChild(folderDocument.importNode(n, true ));
				
				KTLibraryFolder *folder = new KTLibraryFolder( e.attribute("id"), k->project, this);
				folder->fromXml(folderDocument.toString(0));
				
				addFolder(folder);
			}
		}
		
		n = n.nextSibling();
	}
}

QDomElement KTLibraryFolder::toXml(QDomDocument &doc) const
{
	QDomElement folder = doc.createElement("folder");
	folder.setAttribute("id", k->id);
	
	foreach( KTLibraryObject *object, k->objects.values())
	{
		folder.appendChild(object->toXml(doc));
	}
	
	foreach( KTLibraryFolder *folderObject, k->folders)
	{
		folder.appendChild(folderObject->toXml(doc));
	}
	
	return folder;
}



