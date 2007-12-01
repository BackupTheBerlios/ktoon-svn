/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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
#include "ktpathitem.h"

class QGraphicsItem;

/**
 * @author David Cuadrado <krawek@gmail.com>
 * @todo:
 * - Serialize fonts
 * 
*/
class KTSerializer
{
	public:
		KTSerializer();
		~KTSerializer();
		
		static QDomElement properties(const QGraphicsItem *item, QDomDocument &doc);
		static void loadProperties(QGraphicsItem *item, const QXmlAttributes &atts);
		static void loadProperties(QGraphicsItem *item, const QDomElement &e);
		
		static QDomElement gradient(const QGradient *gradient, QDomDocument &doc);
// 		static void loadGradient(QGradient *gradient, const QXmlAttributes &atts);
		static QGradient * createGradient(const QXmlAttributes &atts);
		
		static QDomElement brush(const QBrush *brush, QDomDocument &doc);
		static void loadBrush(QBrush &brush, const QXmlAttributes &atts);
		static void loadBrush(QBrush &brush, const QDomElement &e);
		
		
		static QDomElement pen(const QPen *pen, QDomDocument &doc);
		static void loadPen(QPen &pen, const QXmlAttributes &atts);
		static void loadPen(QPen &pen, const QDomElement &e);
		
		static QDomElement font(const QFont *font, QDomDocument &doc);
		static void loadFont(QFont &font, const QDomElement &e);
		static void loadFont(QFont &font, const QXmlAttributes &atts);
		
};		

#endif
