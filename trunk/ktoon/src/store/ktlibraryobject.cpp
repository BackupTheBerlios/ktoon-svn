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
}

QDomElement KTLibraryObject::toXml(QDomDocument &doc) const
{
	QDomElement object = doc.createElement("object");
	
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



