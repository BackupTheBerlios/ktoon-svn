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
	
	QList<QGraphicsItem *> items;
	QList<KTGraphicObject *> graphics;
};

KTFrame::KTFrame(KTLayer *parent) : QObject(parent), d(new Private)
{
	d->name = "Frame";
	d->isLocked = false;
	d->isVisible = true;
}

KTFrame::~KTFrame()
{
	qDeleteAll(d->graphics);
	
	delete d;
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
				
				while(!n2.isNull())
				{
					QDomElement e2 = n2.toElement();
					
					if( e2.tagName() == "tweening" )
					{
						KTItemTweener *tweener = new KTItemTweener(0, d->graphics.last());
						
						QDomDocument newDoc;
						newDoc.appendChild(newDoc.importNode(n2, true ));
						
						tweener->fromXml(newDoc.toString(0));
						
						d->graphics.last()->setTweener(tweener);
					}
					else
					{
						QDomDocument newDoc;
						newDoc.appendChild(newDoc.importNode(n2, true ));
						
						createItem(d->items.count(), newDoc.toString(0));
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
	
	QList<KTGraphicObject *>::ConstIterator iterator = d->graphics.begin();
	
	while ( iterator != d->graphics.end() )
	{
		root.appendChild( (*iterator)->toXml(doc) );
		++iterator;
	}
	
	return root;
}

void KTFrame::addItem(QGraphicsItem *item)
{
	if ( d->graphics.count() )
	{
		if ( QGraphicsItem *lastItem = d->graphics.last()->item() )
		{
			item->setZValue(lastItem->zValue()+1);
		}
	}
	
	KTGraphicObject *object = new KTGraphicObject(item, this);
	
#if 0
	KTItemTweener *tweener = new KTItemTweener(50, object);
	
	for(int step = 0; step < tweener->frames(); step++)
	{
// 		tweener->setRotationAt(step, i*30);
// 		tweener->setScaleAt(step, 0.1*(step+1), 1.0);
		tweener->setTranslationAt(step, step*10, step);
	}
	
	object->setTweener(tweener);
#endif 
	
	d->graphics << object;
}

QGraphicsItemGroup *KTFrame::createItemGroupAt(int position, QList<qreal> group )
{
	D_FUNCINFO;
	
	int count = 0;
	
	qSort(group.begin(), group.end());
	
	
	KTItemGroup *g = new KTItemGroup(0);
	
	foreach( qreal pos, group )
	{
		QGraphicsItem *item = this->item((int)pos);
		
		g->addToGroup( item );
		count++;
	}
	
	addItem( g );
	return g;
}

QList<QGraphicsItem *> KTFrame::destroyItemGroup(int position)
{
	QList<QGraphicsItem *> items;
	if ( KTItemGroup *group = qgraphicsitem_cast<KTItemGroup *>(item(position)) )
	{
		items = group->childs();
		foreach(QGraphicsItem *child, group->childs())
		{
			group->removeFromGroup(child);
		}
		removeGraphicAt(position);
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

bool KTFrame::removeGraphicAt(int position)
{
	if ( position < 0 || position >= d->graphics.count() )
	{
		return false;
	}
	
	KTGraphicObject *object = d->graphics.takeAt(position);
	
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
	
	QGraphicsItem *item = itemFactory.create( xml );
	if( item )
	{
		addItem( item );
		
		if( KTGraphicLibraryItem *symbol = qgraphicsitem_cast<KTGraphicLibraryItem *>(item) )
		{
			KTLibrary *library = project()->library();
			symbol->setObject(library->findObject(symbol->symbolName()));
		}
	}
	
	if( loaded )
	{
		KTProjectLoader::createItem( scene()->index(), layer()->index(), index(), position, xml, project() );
	}
	
	return item;
}


QList<KTGraphicObject *> KTFrame::graphics() const
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
	
	return d->graphics[position];
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

int KTFrame::indexOf(KTGraphicObject *object)
{
	return d->graphics.indexOf(object);
}

int KTFrame::indexOf(QGraphicsItem *item)
{
	int index = -1;
	foreach(KTGraphicObject *object, d->graphics)
	{
		index++;
		
		if (object->item() == item )
		{
			return index;
		}
	}
	
	return index;
}

int KTFrame::index() const
{
	return layer()->indexOf(const_cast<KTFrame *>(this));
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

