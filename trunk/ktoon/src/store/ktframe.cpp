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

// #include "ktbuttonitem.h"
// #include "kttextitem.h"

KTFrame::KTFrame(KTLayer *parent) : QObject(parent), m_name("Frame"), m_isLocked(false), m_isVisible(true)
{
	init();
}


void KTFrame::init()
{
// 	setItemIndexMethod(QGraphicsScene::NoIndex);
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
// 			dDebug() << "Item??? " << e.tagName();
#if 0
			if ( e.tagName() == "frame" )
			{
				KTFrame *frame = createFrame( m_frames.count() );
				
				if ( frame )
				{
					QDomDocument newDoc;
					newDoc.appendChild(newDoc.importNode(n, true ));
					frame->fromXml( newDoc.toString(0) );
				}
			}
#endif
		}
		
		n = n.nextSibling();
	}
}

QDomElement KTFrame::toXml(QDomDocument &doc)
{
	QDomElement root = doc.createElement("frame");
	root.setAttribute("name", m_name );
	doc.appendChild(root);
	
	QList<QGraphicsItem *>::ConstIterator iterator = m_items.begin();
	
	while ( iterator != m_items.end() )
	{
// 		root.appendChild( (*iterator)->toXml(doc) );
		++iterator;
	}
	
	return root;
}

void KTFrame::addGraphic(QGraphicsItem *item)
{
	if ( m_items.count() )
	{
		item->setZValue(m_items.last()->zValue()+1);
	}
	
	m_items << item;
// 	addItem(item);
}

void KTFrame::removeGraphic(QGraphicsItem *item)
{
	m_items.removeAll(item);
// 	removeItem(item);
}

void KTFrame::replaceGraphic(int position, QGraphicsItem *item)
{
	m_items.replace(position, item);
}

void KTFrame::recoverItems()
{
// 	foreach(QGraphicsItem *item, m_items)
// 	{
// 		addItem(item);
// 	}
}

bool KTFrame::removeItemAt(int position)
{
	if ( position < 0 || position >= m_items.count() )
	{
		return false;
	}
	
	QGraphicsItem *item = m_items.takeAt(position);
	
	return true;
}

QGraphicsItem *KTFrame::createItem(int position, const QString &xml)
{
	KTItemFactory itemFactory;
	
	QGraphicsItem *item = itemFactory.create( xml );
	if ( item )
	{
		addGraphic( item );
	}
	
	return item;
}


QList<QGraphicsItem *> KTFrame::graphics() const
{
	return m_items;
}

QGraphicsItem *KTFrame::item(int position) const
{
	if ( position < 0 || position >= m_items.count() )
	{
		D_FUNCINFO << " FATAL ERROR: index out of bound " << position;
		return 0;
	}
	
	return m_items[position];
}

KTLayer *KTFrame::layer() const
{
	return static_cast<KTLayer *>(parent());
}

