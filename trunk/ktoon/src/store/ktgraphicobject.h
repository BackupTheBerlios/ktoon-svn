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

#ifndef KTGRAPHICOBJECT_H
#define KTGRAPHICOBJECT_H

#include <QObject>
#include "ktabstractserializable.h"
#include "ktglobal_store.h"

class QGraphicsItem;


/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTGraphicObject : public QObject, public KTAbstractSerializable
{
	public:
		KTGraphicObject(QGraphicsItem *item = 0, QObject *parent = 0);
		~KTGraphicObject();
		
		void setItem(QGraphicsItem *item);
		QGraphicsItem *item() const;
		
		void setObjectName(const QString &name);
		QString objectName() const;
		
	public:
		virtual void fromXml(const QString &xml );
		virtual QDomElement toXml(QDomDocument &doc);
		
	private:
		QString m_name;
		QGraphicsItem *m_item;
};

#endif
