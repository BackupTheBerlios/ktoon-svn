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

#include "ktlibraryobject.h"

#include "ktitemfactory.h"
#include "ktpixmapitem.h"

#include <QGraphicsSvgItem>
#include <QSvgRenderer>
#include <QTemporaryFile>
#include <QDir>

#include <ksound/kaudioplayer.h>
#include <kcore/kdebug.h>

struct KTLibraryObject::Private
{
	int type;
	QVariant data;
	QString dataPath;
	QString symbolName;
};

KTLibraryObject::KTLibraryObject(QObject *parent) : QObject(parent), d(new Private)
{
}


KTLibraryObject::~KTLibraryObject()
{
	delete d;
}

/**
 * Items, pics, text, and so...
 * @param data 
 */
void KTLibraryObject::setData(const QVariant &data)
{
	d->data = data;
}

QVariant KTLibraryObject::data() const
{
	return d->data;
}

QString KTLibraryObject::dataPath() const
{
	return d->dataPath;
}

void KTLibraryObject::setType(int type)
{
	d->type = type;
}

int KTLibraryObject::type() const
{
	return d->type;
}


void KTLibraryObject::setSymbolName(const QString &name)
{
	d->symbolName = name;
	d->symbolName.replace(QDir::separator(), "-");
}

QString KTLibraryObject::symbolName() const
{
	return d->symbolName;
}

void KTLibraryObject::fromXml(const QString &xml )
{
	QDomDocument document;
	
	if(! document.setContent(xml) )
	{
		return;
	}
	
	QDomElement objectTag = document.documentElement();
	
	if( objectTag.tagName() == "object" )
	{
		setSymbolName(objectTag.attribute("id"));
		
		if( d->symbolName.isEmpty() ) return;
		
		d->type = objectTag.attribute("type").toInt();
		
		d->dataPath = objectTag.attribute("path");
		
		QDomElement objectData = objectTag.firstChild().toElement();
		if( !objectTag.isNull())
		{
			QString data;
			{
				QTextStream ts(&data);
				ts << objectData;
			}
			
			loadData(data.toLocal8Bit());
		}
	}
}

QDomElement KTLibraryObject::toXml(QDomDocument &doc) const
{
	QDomElement object = doc.createElement("object");
	object.setAttribute("id", d->symbolName);
	object.setAttribute("type", d->type);
	
	QFileInfo finfo(d->dataPath);
	
	switch(d->type)
	{
		case Text:
		case Item:
		{
			QGraphicsItem *item = qvariant_cast<QGraphicsItem *>(d->data);
			
			if( item )
			{
				if( KTAbstractSerializable *serializable = dynamic_cast<KTAbstractSerializable *>(item) )
				{
					object.appendChild(serializable->toXml(doc));
				}
			}
		}
		break;
		case Svg:
		{
			QGraphicsItem *item = qvariant_cast<QGraphicsItem *>(d->data);
			
			if( item )
			{
				if( KTAbstractSerializable *serializable = dynamic_cast<KTAbstractSerializable *>(item) )
				{
					object.appendChild(serializable->toXml(doc));
				}
			}
			
			object.setAttribute("path", finfo.fileName());
		}
		break;
		case Image:
		{
			QGraphicsItem *item = qvariant_cast<QGraphicsItem *>(d->data);
			
			if( item )
			{
				if( KTAbstractSerializable *serializable = dynamic_cast<KTAbstractSerializable *>(item) )
				{
					object.appendChild(serializable->toXml(doc));
				}
			}
			
			object.setAttribute("path", finfo.fileName());
		}
		break;
		case Sound:
		{
			object.setAttribute("path", finfo.fileName());
		}
		break;
	}
	
	return object;
}

bool KTLibraryObject::loadData(const QByteArray &data)
{
	if( data.isEmpty() ) return false;
	
	bool ok = true;
	switch(d->type)
	{
		case KTLibraryObject::Item:
		{
			KTItemFactory factory;
			QGraphicsItem *item = factory.create(QString::fromLocal8Bit(data));
			
			setData( QVariant::fromValue(item) );
		}
		break;
		case KTLibraryObject::Image:
		{
			QPixmap pixmap;
			pixmap.loadFromData(data);
			
			KTPixmapItem *item = new KTPixmapItem;
			item->setPixmap(pixmap);
			
			setData(QVariant::fromValue(static_cast<QGraphicsItem *>(item)));
		}
		break;
		case KTLibraryObject::Sound:
		{
			QTemporaryFile soundFile(QDir::tempPath()+"/ktoon_sound_file_XXXXXX");
			soundFile.setAutoRemove(false);
			
			if( soundFile.open() )
			{
				soundFile.write(data);
				setData(soundFile.fileName());
				
				soundFile.close();
			}
		}
		break;
		case KTLibraryObject::Svg:
		{
			QGraphicsSvgItem *svg = new QGraphicsSvgItem;
			svg->renderer()->load(data);
			
			setData(QVariant::fromValue(static_cast<QGraphicsItem*>(svg)));
		}
		break;
		case KTLibraryObject::Text:
		{
			setData(QString::fromLocal8Bit(data));
		}
		break;
		default:
		{
			ok = false;
		}
		break;
	}
	
	return ok;
}

bool KTLibraryObject::loadDataFromPath(const QString &dataDir)
{
	switch(d->type)
	{
		case KTLibraryObject::Image:
		{
			d->dataPath = dataDir+"/images/"+d->dataPath;
			
			QFile f(d->dataPath);
			
			if( f.open(QIODevice::ReadOnly) )
			{
				loadData(f.readAll());
			}
		}
		break;
		case KTLibraryObject::Sound:
		{
			d->dataPath = dataDir+"/audio/"+d->dataPath;
		}
		break;
		case KTLibraryObject::Svg:
		{
		}
		break;
		default: return false; break;
	}
	
	return true;
}

void KTLibraryObject::saveData(const QString &dataDir)
{
	switch( d->type )
	{
		case KTLibraryObject::Sound:
		{
			QString saved = dataDir + "/audio/";
			
			if( ! QFile::exists(saved) )
			{
				QDir dir;
				dir.mkpath(saved);
			}
			
			QFile::copy(QString(d->data.toString()), saved+d->symbolName);
			QFile::remove(QString(d->data.toString()));
			
			d->dataPath = saved+d->symbolName;
			d->data = "";
		}
		break;
		case KTLibraryObject::Svg:
		{
		}
		break;
		case KTLibraryObject::Image:
		{
			QString dest = dataDir+"/images/";
			
			if( ! QFile::exists(dest) )
			{
				QDir dir;
				dir.mkpath(dest);
			}
			
			qgraphicsitem_cast<KTPixmapItem *>(qvariant_cast<QGraphicsItem *>(d->data))->pixmap().save(dest+d->symbolName, "PNG");
			
			d->dataPath = dest+d->symbolName;
		}
		break;
		default: break;
	}
}


