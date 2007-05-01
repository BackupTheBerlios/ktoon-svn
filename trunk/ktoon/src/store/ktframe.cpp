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

#include "ktframe.h"
#include "ktlayer.h"

#include <dcore/ddebug.h>

#include <QGraphicsItem>

#include "ktitemfactory.h"
#include "ktgraphicobject.h"
#include "ktgraphiclibraryitem.h"
#include "ktlibrary.h"

#include "ktitemgroup.h"
#include "ktitemtweener.h"

#include "ktprojectloader.h"

#include "ktscene.h"
#include "ktlayer.h"

struct KTFrame::Private
{
	QString name;
	bool isLocked;
	bool isVisible;
	GraphicObjects graphics;
	int repeat;
};

KTFrame::KTFrame(KTLayer *parent) : QObject(parent), d(new Private)
{
	d->name = "Frame";
	d->isLocked = false;
	d->isVisible = true;
	d->repeat = 1;
}

KTFrame::~KTFrame()
{
	d->graphics.clear(true);
	delete d;
}


void KTFrame::clean()
{
	d->graphics.clear(true);
}

void KTFrame::setFrameName(const QString &name)
{
	d->name = name;
}

void KTFrame::setLocked(bool isLocked)
{
	d->isLocked = isLocked;
}

QString KTFrame::frameName() const
{
	return d->name;
}

bool KTFrame::isLocked() const
{
	return d->isLocked;
}

void KTFrame::setVisible(bool isVisible)
{
	d->isVisible = isVisible;
}

bool KTFrame::isVisible() const
{
	return d->isVisible;
}


void KTFrame::fromXml(const QString &xml )
{
	QDomDocument document;
	
	if (! document.setContent(xml) )
	{
		return;
	}
	
	QDomElement root = document.documentElement();
	
	setFrameName( root.attribute( "name", frameName() ) );
	
	QDomNode n = root.firstChild();
	
	while( !n.isNull() )
	{
		QDomElement e = n.toElement();
		
		if(!e.isNull())
		{
			if( e.tagName() == "object" )
			{
				QDomNode n2 = e.firstChild();
				
				KTGraphicObject *last = 0;
				
				while(!n2.isNull())
				{
					QDomElement e2 = n2.toElement();
					
					if( e2.tagName() == "tweening" && last)
					{
						KTItemTweener *tweener = new KTItemTweener(0, last);
						
						QString newDoc;
					
						{
							QTextStream ts(&newDoc);
							ts << n2;
						}
						
						tweener->fromXml(newDoc);
						
						last->setTweener(tweener);
					}
					else
					{
						QString newDoc;
						
						{
							QTextStream ts(&newDoc);
							ts << n2;
						}
						
						createItem(d->graphics.count(), newDoc);
						last = d->graphics.visualValue(d->graphics.count()-1);
					}
					n2 = n2.nextSibling();
				}
			}
		}
		
		n = n.nextSibling();
	}
}

QDomElement KTFrame::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("frame");
	root.setAttribute("name", d->name );
	doc.appendChild(root);
	
	foreach(KTGraphicObject *object, d->graphics.visualValues() )
	{
		root.appendChild( object->toXml(doc) );
	}
	
	return root;
}

void KTFrame::addItem(QGraphicsItem *item)
{
	insertItem(d->graphics.count(), item);
}

void KTFrame::insertItem(int position, QGraphicsItem *item)
{
	if ( d->graphics.contains(position-1) )
	{
		if ( QGraphicsItem *lastItem = d->graphics.visualValue(position-1)->item() )
		{
			item->setZValue(lastItem->zValue()+1);
		}
	}
	
	KTGraphicObject *object = new KTGraphicObject(item, this);
	d->graphics.insert(position, object);
}

QGraphicsItemGroup *KTFrame::createItemGroupAt(int position, QList<qreal> group )
{
	D_FUNCINFO;
	
	qSort(group.begin(), group.end());
	
	
	KTItemGroup *g = new KTItemGroup(0);
	int count = 0;
	
	foreach( qreal p, group )
	{
		int pos = (int)p - count;
		
		QGraphicsItem *item = this->item(pos);
		
		KTGraphicObject *object = this->graphic(pos);
		object->setItem(0);
		
		removeGraphicAt(pos);
		
		g->addToGroup( item );
		
		delete object;
		
		count++;
	}
	
	insertItem(position, g );
	return g;
}

QList<QGraphicsItem *> KTFrame::destroyItemGroup(int position)
{
	QList<QGraphicsItem *> items;
	if ( KTItemGroup *group = qgraphicsitem_cast<KTItemGroup *>(item(position)) )
	{
		removeGraphicAt(position);
		
		items = group->childs();
		foreach(QGraphicsItem *child, group->childs())
		{
			group->removeFromGroup(child);
			addItem(child);
		}
	}
	return items;
}

void KTFrame::replaceItem(int position, QGraphicsItem *item)
{
	KTGraphicObject *toReplace = this->graphic(position);
	
	if ( toReplace )
	{
		toReplace->setItem(item);
	}
}

bool KTFrame::moveItem(int currentPosition, int newPosition)
{
	D_FUNCINFO << "current "<< currentPosition << " new "  << newPosition;
	if(currentPosition == newPosition || currentPosition < 0 || currentPosition >= d->graphics.count() || newPosition < 0 || newPosition >= d->graphics.count())
	{
		return false;
	}
	
	if(currentPosition < newPosition)
	{
		for( int i = currentPosition; i < newPosition; i++)
		{
			double tmp = d->graphics.visualValue(i)->item()->zValue();
			d->graphics.visualValue(i)->item()->setZValue(d->graphics.visualValue(i+1)->item()->zValue());
			d->graphics.visualValue(i+1)->item()->setZValue(tmp);
			d->graphics.moveVisual(i, i+1);
		}
	}
	else
	{
		for( int i = currentPosition; i > newPosition; i--)
		{
			double tmp = d->graphics.visualValue(i)->item()->zValue();
			d->graphics.visualValue(i)->item()->setZValue(d->graphics.visualValue(i-1)->item()->zValue());
			d->graphics.visualValue(i-1)->item()->setZValue(tmp);
			d->graphics.moveVisual(i, i-1);
		}
	}
	return true;
}

bool KTFrame::removeGraphicAt(int position)
{
	if ( position < 0 || position >= d->graphics.count() )
	{
		return false;
	}
	
	KTGraphicObject *object = d->graphics.takeVisual(position);
	
	QGraphicsItem *item = object->item();
	
	if ( item )
	{
		QGraphicsScene *scene = item->scene();
		
		if(scene)
		{
			scene->removeItem(item);
		}
		
		this->scene()->removeTweeningObject(object);
		
		return true;
	}
	
	return false;
}

QGraphicsItem *KTFrame::createItem(int position, const QString &xml, bool loaded)
{
	KTItemFactory itemFactory;
	itemFactory.setLibrary(project()->library());
	
	QGraphicsItem *item = itemFactory.create( xml );
	if( item )
	{
		insertItem(position, item );
	}
	
	if( loaded )
	{
		KTProjectLoader::createItem( scene()->visualIndex(), layer()->visualIndex(), visualIndex(), position, xml, project() );
	}
	
	return item;
}


GraphicObjects KTFrame::graphics() const
{
	return d->graphics;
}

KTGraphicObject *KTFrame::graphic(int position) const
{
	if ( position < 0 || position >= d->graphics.count() )
	{
		D_FUNCINFO << " FATAL ERROR: index out of bound " << position;
		return 0;
	}
	
	return d->graphics.visualValue(position);
}

QGraphicsItem *KTFrame::item(int position) const
{
	KTGraphicObject *object = graphic(position);
	
	if ( object )
	{
		return object->item();
	}
	
	return 0;
}

int KTFrame::visualIndexOf(KTGraphicObject *object) const
{
	return d->graphics.visualIndex(object);
}

int KTFrame::logicalIndexOf(KTGraphicObject *object) const
{
	return d->graphics.logicalIndex(object);
}

int KTFrame::visualIndexOf(QGraphicsItem *item) const
{
	foreach(KTGraphicObject *object, d->graphics.values() )
	{
		if (object->item() == item )
		{
			return d->graphics.visualIndex(object);
		}
	}
	return -1;
}

int KTFrame::logicalIndexOf(QGraphicsItem *item) const
{
	foreach(KTGraphicObject *object, d->graphics.values())
	{
		if (object->item() == item )
		{
			return d->graphics.logicalIndex(object);
		}
	}
	
	return -1;
}


int KTFrame::logicalIndex() const
{
	return layer()->visualIndexOf(const_cast<KTFrame *>(this));
}


void KTFrame::setRepeat(int repeat)
{
	d->repeat = repeat;
}

int KTFrame::repeat() const
{
	return d->repeat;
}

int KTFrame::visualIndex() const
{
	return layer()->logicalIndexOf(const_cast<KTFrame *>(this));
}

KTLayer *KTFrame::layer() const
{
	return static_cast<KTLayer *>(parent());
}

KTScene *KTFrame::scene() const
{
	return layer()->scene();
}

KTProject *KTFrame::project() const
{
	return layer()->project();
}

