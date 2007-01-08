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

#include "ddebug.h"

#include "ktitemfactory.h"

#include <QGraphicsItem>
#include "ktgraphicobject.h"

#include "ktitemgroup.h"
#include "ktprojectloader.h"

#include "ktscene.h"
#include "ktlayer.h"

KTFrame::KTFrame(KTLayer *parent) : QObject(parent), m_name("Frame"), m_isLocked(false), m_isVisible(true)
{
}

KTFrame::~KTFrame()
{
}

void KTFrame::setFrameName(const QString &name)
{
	m_name = name;
}

void KTFrame::setLocked(bool isLocked)
{
	m_isLocked = isLocked;
}

QString KTFrame::frameName() const
{
	return m_name;
}

bool KTFrame::isLocked() const
{
	return m_isLocked;
}

void KTFrame::setVisible(bool isVisible)
{
	m_isVisible = isVisible;
}

bool KTFrame::isVisible() const
{
	return m_isVisible;
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
			QDomDocument newDoc;
			newDoc.appendChild(newDoc.importNode(n, true ));
			
			createItem(m_items.count(), newDoc.toString(0));
		}
		
		n = n.nextSibling();
	}
}

QDomElement KTFrame::toXml(QDomDocument &doc)
{
	QDomElement root = doc.createElement("frame");
	root.setAttribute("name", m_name );
	doc.appendChild(root);
	
	QList<KTGraphicObject *>::ConstIterator iterator = m_graphics.begin();
	
	while ( iterator != m_graphics.end() )
	{
		root.appendChild( (*iterator)->toXml(doc) );
		++iterator;
	}
	
	return root;
}

void KTFrame::addItem(QGraphicsItem *item)
{
	if ( m_graphics.count() )
	{
		if ( QGraphicsItem *lastItem = m_graphics.last()->item() )
		{
			item->setZValue(lastItem->zValue()+1);
		}
	}
	
	
	KTGraphicObject *object = new KTGraphicObject(item, this);
	m_graphics << object;
}


QGraphicsItemGroup *KTFrame::createItemGroupAt(int position, QList<qreal> group )
{
	D_FUNCINFO;
	
	int count = 0;
	
	qSort(group.begin(), group.end());
	
	
	KTItemGroup *g = new KTItemGroup(0);
	
	foreach( int pos, group )
	{
		QGraphicsItem *item = this->item(pos-count);
		
		g->addToGroup( item );
		count++;
	}
	
	m_graphics.insert(position, new KTGraphicObject(g, this));
	
	return g;
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
	if ( position < 0 || position >= m_graphics.count() )
	{
		return false;
	}
	
	KTGraphicObject *object = m_graphics.takeAt(position);
	
	QGraphicsItem *item = object->item();
	
	if ( item )
	{
		QGraphicsScene *scene = item->scene();
		
		if(scene)
		{
			scene->removeItem(item);
		}
		
		return true;
	}
	
	return false;
}

QGraphicsItem *KTFrame::createItem(int position, const QString &xml, bool loaded)
{
	KTItemFactory itemFactory;
	
	QGraphicsItem *item = itemFactory.create( xml );
	if ( item )
	{
		addItem( item );
	}
	
	if ( loaded )
	{
		KTProjectLoader::createItem( scene()->index(), layer()->index(), index(), position, xml, project());
	}
	
	return item;
}


QList<KTGraphicObject *> KTFrame::graphics() const
{
	return m_graphics;
}

KTGraphicObject *KTFrame::graphic(int position) const
{
	if ( position < 0 || position >= m_graphics.count() )
	{
		D_FUNCINFO << " FATAL ERROR: index out of bound " << position;
		return 0;
	}
	
	return m_graphics[position];
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
	return m_graphics.indexOf(object);
}

int KTFrame::indexOf(QGraphicsItem *item)
{
	int index = -1;
	foreach(KTGraphicObject *object, m_graphics)
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

