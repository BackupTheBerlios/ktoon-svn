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

#include <dsound/daudioplayer.h>
#include <dcore/ddebug.h>

struct KTLibraryObject::Private
{
	int type;
	QVariant data;
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
		d->symbolName = objectTag.attribute("id");
		
		if( d->symbolName.isEmpty() ) return;
		
		d->type = objectTag.attribute("type").toInt();
		
		QDomElement objectData = objectTag.firstChild().toElement();
		
		QDomDocument data;
		data.appendChild(data.importNode(objectData, true ));
		
		loadData(data.toString(0).toLocal8Bit()); // FIXME: No va a funcionar para binarios!
	}
}

QDomElement KTLibraryObject::toXml(QDomDocument &doc) const
{
	QDomElement object = doc.createElement("object");
	object.setAttribute("id", d->symbolName);
	object.setAttribute("type", d->type);
	
	switch(d->type)
	{
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
	}
	
	return object;
}

bool KTLibraryObject::loadData(const QByteArray &data)
{
	bool ok = true;
	switch(d->type)
	{
		case KTLibraryObject::Item:
		{
			KTItemFactory factory;
			QGraphicsItem *item = factory.create(QString::fromLocal8Bit(data));
			
			setData( QVariant::fromValue(item) );
		};
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

