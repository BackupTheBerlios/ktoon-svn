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

#include "ktgraphicobject.h"

#include <QMatrix>
#include <QGraphicsItem>

#include "ktframe.h"
#include "ktscene.h"
#include "ktitemtweener.h"

#include <dcore/ddebug.h>

struct KTGraphicObject::Private
{
	QString name;
	QGraphicsItem *item;
	
	KTItemTweener *tweener;
	
	KTFrame *frame;
};

KTGraphicObject::KTGraphicObject(QGraphicsItem *item, KTFrame *parent)
	: QObject(parent), d(new Private)
{
	DINIT;
	
	d->item = item;
	d->tweener = 0;
	d->frame = parent;
	
	initItemData();
}


KTGraphicObject::~KTGraphicObject()
{
	DEND;
	if( d->item )
	{
// 		if ( QGraphicsScene *scene = d->item->scene() )
// 		{
// 			scene->removeItem(d->item);
// 		}
		delete d->item;
	}
	
	if(d->tweener && d->frame->scene())
	{
		d->frame->scene()->removeTweeningObject(this);
	}
	delete d;
}

void KTGraphicObject::fromXml(const QString &xml )
{
}

QDomElement KTGraphicObject::toXml(QDomDocument &doc) const
{
	QDomElement object = doc.createElement("object");
	if ( KTAbstractSerializable *is = dynamic_cast<KTAbstractSerializable *>(d->item) )
	{
		object.appendChild(is->toXml(doc));
	}
	
	if(d->tweener)
	{
		object.appendChild(d->tweener->toXml(doc));
	}
	
	return object;
}


void KTGraphicObject::setItem(QGraphicsItem *item)
{
	d->item = item;
	initItemData();
}

QGraphicsItem *KTGraphicObject::item() const
{
	return d->item;
}

void KTGraphicObject::setObjectName(const QString &name)
{
	d->name = name;
}

QString KTGraphicObject::objectName() const
{
	return d->name;
}

void KTGraphicObject::initItemData()
{
	
	if(! d->item->data(ScaleX).isValid())
	{
		d->item->setData(ScaleX, 1.0);
	}
	if(! d->item->data(ScaleY).isValid())
	{
		d->item->setData(ScaleY, 1.0);
	}
	if(! d->item->data(Rotate).isValid())
	{
		d->item->setData(Rotate, 0.0);
	}
	if(! d->item->data(TranslateX).isValid())
	{
		d->item->setData(TranslateX, 0.0);
	}
	if(! d->item->data(TranslateY).isValid())
	{
		d->item->setData(TranslateY, 0.0);
	}
}

void KTGraphicObject::setTweener(KTItemTweener *tweener)
{
	d->tweener = tweener;
	
	if( d->tweener )
	{
		d->tweener->setItem(d->item);
		d->frame->scene()->addTweeningObject(this);
	}
	else
	{
		d->frame->scene()->removeTweeningObject(this);
	}
}

KTItemTweener *KTGraphicObject::tweener() const
{
	return d->tweener;
}

KTFrame *KTGraphicObject::frame() const
{
	return d->frame;
}

int KTGraphicObject::index() const
{
	return d->frame->indexOf(const_cast<KTGraphicObject *>(this));
}

