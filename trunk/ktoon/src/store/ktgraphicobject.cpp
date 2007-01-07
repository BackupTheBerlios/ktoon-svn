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

KTGraphicObject::KTGraphicObject(QGraphicsItem *item, KTFrame *parent)
	: QObject(parent), m_item(item)
{
	initItemData();
}


KTGraphicObject::~KTGraphicObject()
{
}

void KTGraphicObject::fromXml(const QString &xml )
{
}

QDomElement KTGraphicObject::toXml(QDomDocument &doc)
{
	if ( KTAbstractSerializable *is = dynamic_cast<KTAbstractSerializable *>(m_item) )
	{
		return is->toXml(doc);
	}
	
	return QDomElement();
}


void KTGraphicObject::setItem(QGraphicsItem *item)
{
	m_item = item;
	initItemData();
}

QGraphicsItem *KTGraphicObject::item() const
{
	return m_item;
}

void KTGraphicObject::setObjectName(const QString &name)
{
	m_name = name;
}

QString KTGraphicObject::objectName() const
{
	return m_name;
}

void KTGraphicObject::initItemData()
{
	
	if(! m_item->data(ScaleX).isValid())
	{
		m_item->setData(ScaleX, 1.0);
	}
	if(! m_item->data(ScaleY).isValid())
	{
		m_item->setData(ScaleY, 1.0);
	}
	if(! m_item->data(Rotate).isValid())
	{
		m_item->setData(Rotate, 0.0);
	}
	if(! m_item->data(TranslateX).isValid())
	{
		m_item->setData(TranslateX, 0.0);
	}
	if(! m_item->data(TranslateY).isValid())
	{
		m_item->setData(TranslateY, 0.0);
	}
}
