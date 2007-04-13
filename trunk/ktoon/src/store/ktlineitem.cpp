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

#include "ktlineitem.h"
#include "ktserializer.h"
#include <dcore/ddebug.h>

KTLineItem::KTLineItem(QGraphicsItem * parent, QGraphicsScene * scene) : QGraphicsLineItem(parent, scene)
{
}


KTLineItem::~KTLineItem()
{
}


void KTLineItem::fromXml(const QString &xml)
{
}


QDomElement KTLineItem::toXml(QDomDocument &doc) const
{
	QDomElement root = doc.createElement("line");
	
	root.setAttribute("x1", line().x1());
	root.setAttribute("y1", line().y1());
	root.setAttribute("x2", line().x2());
	root.setAttribute("y2", line().y2());
	
	root.appendChild( KTSerializer::properties( this, doc));
	
	QPen pen = this->pen();
	root.appendChild(KTSerializer::pen(&pen, doc));

	return root;
}

