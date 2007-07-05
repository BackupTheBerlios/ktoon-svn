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

#ifndef KTABSTRACTSERIALIZABLE_H
#define KTABSTRACTSERIALIZABLE_H

#include <QString>

#include <QDomDocument>
#include <QDomElement>
#include "ktglobal_store.h"

/**
 * @if spanish
 * Clase abstracta para clases que necesiten ser guardadas, como los frames, scenas, items, etc.
 * @endif
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTAbstractSerializable
{
	protected:
		KTAbstractSerializable() {};
		
	public:
		virtual ~KTAbstractSerializable() {};
		
		virtual void fromXml(const QString &xml) = 0;
		virtual QDomElement toXml(QDomDocument &doc) const = 0;
};

#endif
