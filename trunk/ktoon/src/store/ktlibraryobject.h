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

#ifndef KTLIBRARYOBJECT_H
#define KTLIBRARYOBJECT_H

#include <QVariant>
#include <QGraphicsItem>

#include "ktabstractserializable.h"
#include "ktglobal_store.h"

class KTLibraryObject;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTLibraryObject : public QObject, public KTAbstractSerializable
{
	public:
		enum Type
		{
			Item = 0x01,
			Image,
			Sound,
			Svg,
			Text
		};
		
		KTLibraryObject(QObject *parent = 0);
		~KTLibraryObject();
		
		void setType(int type);
		int type() const;
		
		void setData(const QVariant &data);
		QVariant data() const;
		
		void setSymbolName(const QString &name);
		QString symbolName() const;
		
		
		bool loadData(const QByteArray &data);
		
	public:
		virtual void fromXml(const QString &xml );
		virtual QDomElement toXml(QDomDocument &doc) const;
		
	private:
		struct Private;
		Private *const d;
		
};

#endif


