/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "ktproject.h"

#include <QDir>

#include <dcore/ddebug.h>

#include "ktscene.h"
#include "ktlayer.h"
#include "ktsoundlayer.h"
#include "ktframe.h"

#include "ktlibrary.h"
#include "ktlibraryobject.h"
#include "ktgraphiclibraryitem.h"

#include "ktitemfactory.h"

#include "ktprojectresponse.h"

#include "ktprojectloader.h"


#include <QGraphicsView>
#include <QGraphicsSvgItem>
#include <QSvgRenderer>


struct KTProject::Private
{
	Scenes scenes;
	QString name;
	QString dataDir;
	
	int sceneCounter;
	KTLibrary *library;
	bool isOpen;
	QString author;
	QString description;
};

/**
 * Constructor por defecto
 */
KTProject::KTProject(QObject *parent) : QObject(parent), d(new Private)
{
	DINIT;
	
	d->sceneCounter = 0;
	d->isOpen = false;
	
	d->library = new KTLibrary("library", this);
}


/**
 * Destructor por defecto
 */
KTProject::~KTProject()
{
	DEND;
	
	deleteDataDir();
	
	delete d;
}

/**
 * Esta funcion carga la libreria local
 */
void KTProject::loadLibrary(const QString &filename)
{
	QFile lfile(filename);
	if ( lfile.open(QIODevice::ReadOnly | QIODevice::Text))
	{
		d->library->fromXml(QString::fromLocal8Bit(lfile.readAll()));
		lfile.close();
	}
	else
	{
		dFatal("library") << "Cannot open library from: " << filename;
	}
}

/**
 * Cierra el proyecto
 */
void KTProject::clear()
{
	qDeleteAll(d->scenes);
	d->scenes.clear();
	
	d->sceneCounter = 0;
	
	deleteDataDir();
}

/**
 * Pone un nombre al proyecto
 */
void KTProject::setProjectName(const QString &name)
{
	d->name = name;
}

/**
 * Pone el autor del proyecto
 */
void KTProject::setAuthor(const QString &author)
{
	d->author = author;
}

/**
 * Pone la descripcion del proyecto
 */
void KTProject::setDescription(const QString& description)
{
	d->description = description;
}

/**
 * @if spanish
 * Retorna el nombre del proyecto
 * @endif
 * 
 * @if english
 * Returns project name
 * @endif
 */
QString KTProject::projectName() const
{
	return d->name;
}

/**
 * retorna el autor del proyecto
 * @return 
 */
QString KTProject::author() const
{
	return d->author;
}

/**
 * retorna la descripcion del proyecto
 * @return 
 */
QString KTProject::description() const
{
	return d->description;
}


KTScene *KTProject::createScene(int position, bool loaded )
{
	dDebug("project") << "Creating scene " << position;
	if( position < 0 || position > d->scenes.count() )
	{
		return 0;
	}
	
	KTScene *scene = new KTScene(this);
	d->scenes.insert(position, scene);
	d->sceneCounter++;
	
	scene->setSceneName(tr("Scene %1").arg(d->sceneCounter));
	
	if ( loaded )
	{
		KTProjectLoader::createScene(position, scene->sceneName(), this);
	}
	
	return scene;
}

bool KTProject::removeScene(int position)
{
	D_FUNCINFO;
	KTScene *toRemove = scene(position);
	
	if ( toRemove )
	{
		d->scenes.removeAt(position);
		
		delete toRemove;
		toRemove = 0;
		
		d->sceneCounter--;
		
		return true;
	}
	
	return false;
}


bool KTProject::moveScene(int position, int newPosition)
{
	if ( position < 0 || position >= d->scenes.count() || newPosition < 0 || newPosition >= d->scenes.count() )
	{
		dWarning() << "Failed moving scene!";
		return false;
	}
	
	KTScene *scene = d->scenes.takeAt(position);
	
	d->scenes.insert(newPosition, scene);
	
	return true;
}

KTScene *KTProject::scene(int position)
{
	D_FUNCINFOX("project")<< position;
	if( position < 0 || position >= d->scenes.count() )
	{
		D_FUNCINFO << " FATAL ERROR: index out of bound " << position;
		return 0;
	}
	return d->scenes[position];
}

int KTProject::indexOf(KTScene *scene) const
{
	return d->scenes.indexOf(scene);
}

void KTProject::fromXml(const QString &xml )
{
	QDomDocument document;
	
	if (! document.setContent(xml) )
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
			if ( e.tagName() == "project" )
			{
				setProjectName( e.attribute( "name", projectName() ) );
			}
			else if ( e.tagName() == "meta")
			{
				QDomNode n1 = e.firstChild();
				while( !n1.isNull() )
				{
					QDomElement e1 = n1.toElement();
					if ( e1.tagName() == "author")
					{
						if(e1.firstChild().isText())
						{
							setAuthor(e1.text());
						}
					}
					else if ( e1.tagName() == "description")
					{
						if(e1.firstChild().isText())
						{
							setDescription( e1.text());
						}
					}
					n1 = n1.nextSibling();
				}
			}
		}
		
		n = n.nextSibling();
	}
}

QDomElement KTProject::toXml(QDomDocument &doc) const
{
	QDomElement ktoon = doc.createElement("KToon");
	ktoon.setAttribute("version", "1");
	
	QDomElement project = doc.createElement("project");
	project.setAttribute("name", d->name);
	
	
	QDomElement meta = doc.createElement("meta");
	
	QDomElement author = doc.createElement("author");
	author.appendChild(doc.createTextNode(d->author));
	
	QDomElement description = doc.createElement("description");
	description.appendChild(doc.createTextNode(d->description));
	
	
	meta.appendChild(author);
	meta.appendChild(description);
	project.appendChild(meta);
	ktoon.appendChild(project);
	
	return ktoon;
}

Scenes KTProject::scenes() const
{
	return d->scenes;
}


bool KTProject::createSymbol(int type, const QString &name, const QByteArray &data)
{
	if( !d->isOpen ) return false;
	
	KTLibraryObject *object = new KTLibraryObject(d->library);
	object->setType(KTLibraryObject::Type(type));
	
	if ( !object->loadData(data ) )
		return false;
	
	d->library->addObject( object, name);
	object->saveData(this->dataDir());
	
	
	return true;
}

bool KTProject::removeSymbol(const QString &name)
{
	return d->library->removeObject(name);
}

bool KTProject::addSymbolToProject(const QString &name, int sceneIndex, int layerIndex, int frameIndex)
{
	dDebug() << sceneIndex << " " << layerIndex << " " << frameIndex;
	
	KTLibraryObject *object = d->library->findObject(name);
	
	KTFrame *target = 0;
	
	KTScene *scene = this->scene(sceneIndex);
	
	if(scene)
	{
		KTLayer *layer = scene->layer(layerIndex);
		if( layer )
		{
			target = layer->frame(frameIndex);
		}
	}
	
	if(object && target)
	{
		switch(object->type())
		{
			case KTLibraryObject::Text:
			case KTLibraryObject::Image:
			case KTLibraryObject::Svg:
			case KTLibraryObject::Item:
			{
				KTGraphicLibraryItem *libraryItem = new KTGraphicLibraryItem(object);
				target->addItem(libraryItem);
			}
			break;
			case KTLibraryObject::Sound:
			{
				KTSoundLayer *sound = scene->createSoundLayer(scene->soundLayers().count());
				sound->setFilePath(object->data().toString());
			}
			break;
		}
		return true;
	}
	return false;
}

bool KTProject::removeSymbolFromProject(const QString &name, int scene, int layer, int frame)
{
	dFatal("project") << "removeSymbolFromProject::Find me in ktproject.cpp";
	
	
	
	return false;
}

KTLibrary *KTProject::library() const
{
	return d->library;
}

void KTProject::emitResponse(KTProjectResponse *response)
{
	emit responsed(response);
}

void KTProject::setOpen(bool open)
{
	d->isOpen = open;
}

bool KTProject::isOpen()
{
	return d->isOpen;
}


bool KTProject::deleteDataDir()
{
	if(QFile::exists( dataDir() ) && !d->name.isEmpty() )
	{
		QDir dir(dataDir() );
		
		if( dir.exists("audio") && dir.exists("video") && dir.exists("images") || dir.exists("project.ktp") )
		{
			dDebug("project") << "Removing " << dir.absolutePath() << "...";
			
			dir.remove("project.ktp");
			dir.remove("library.ktl");
			
			foreach(QString scene, dir.entryList(QStringList() << "*.kts" ))
			{
				QFile::remove(scene);
			}
			
			foreach( QString subdir, QStringList() << "audio" << "video" << "images" )
			{
				if( dir.exists(subdir) )
				{
					dir.cd(subdir);
					foreach(QString file, dir.entryList() )
					{
						QString absolute = dir.absolutePath() + "/" + file;
						
						if( !file.startsWith(".") )
						{
							QFileInfo finfo(absolute);
							
							if( finfo.isFile() )
							{
								QFile::remove(absolute);
							}
						}
					}
					dir.cdUp();
					dir.rmdir(subdir);
				}
			}
			
			if( ! dir.rmdir(dir.absolutePath()) )
			{
				dError("project") << "Cannot remove project data directory!";
			}
		}
		return true;
	}
	
	return false;
}

QString KTProject::dataDir() const
{
	return CACHE_DIR + "/"+ d->name;
}



