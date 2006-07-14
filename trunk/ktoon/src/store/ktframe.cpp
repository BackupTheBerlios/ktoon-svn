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
#include "ddebug.h"


KTFrame::KTFrame(QObject *parent) : QGraphicsScene(parent), m_name("Frame"), m_isLocked(false), m_isVisible(true)
{
	init();
}

KTFrame::KTFrame(const QString &frameName, QObject * parent) : QGraphicsScene(parent), m_name(frameName), m_isLocked(false), m_isVisible(true)
{	
	init();
}

void KTFrame::init()
{
	setItemIndexMethod(QGraphicsScene::NoIndex);
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
			dDebug() << "Item??? " << e.tagName();
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
	
	QList<QGraphicsItem *> items = this->items();
	
	QList<QGraphicsItem *>::ConstIterator iterator = items.begin();
	
	while ( iterator != items.end() )
	{
// 		root.appendChild( (*iterator)->toXml(doc) );
		++iterator;
	}
	
	return root;
}

