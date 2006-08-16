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

#ifndef KTSERIALIZER_H
#define KTSERIALIZER_H

#include <QDomDocument>
#include <QDomElement>
#include <QXmlAttributes>
#include <QBrush>
#include <QPen>

class QGraphicsItem;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTSerializer
{
	public:
		KTSerializer();
		~KTSerializer();
		
		static QDomElement properties(const QGraphicsItem *item, QDomDocument &doc);
		static void loadProperties(QGraphicsItem *item, const QXmlAttributes &atts);
		static void loadProperties(QGraphicsItem *item, const QDomElement &e);
		
		static QDomElement brush(const QBrush *brush, QDomDocument &doc);
		static void loadBrush(QBrush &brush, const QXmlAttributes &atts);
		static void loadBrush(QBrush &brush, const QDomElement &e);
		
		static QDomElement pen(const QPen *pen, QDomDocument &doc);
		static void loadPen(QPen &pen, const QXmlAttributes &atts);
		static void loadPen(QPen &pen, const QDomElement &e);
		
};

#endif