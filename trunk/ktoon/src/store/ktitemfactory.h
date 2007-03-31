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

#ifndef KTITEMFACTORY_H
#define KTITEMFACTORY_H

#include <QStack>
#include <QPen>
#include <QBrush>

#include "dcore/dxmlparserbase.h"

#include "ktglobal_store.h"

class QGraphicsItem;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class STORE_EXPORT KTItemFactory : public DXmlParserBase
{
	public:
		KTItemFactory();
		~KTItemFactory();
		
		/**
		 * Analiza etiquetas de apertura del documento XML
		 */
		bool startTag(const QString& qname, const QXmlAttributes& atts);
		
		void text( const QString & ch );
		
		/**
		 * Analiza etiquetas de cierre del documento XML
		 */
		bool endTag(const QString& qname);
		
	public:
		QGraphicsItem *create(const QString &xml);
		bool loadItem(QGraphicsItem *item, const QString &xml);
		
	private:
		void setItemPen(const QPen &pen);
		void setItemBrush(const QBrush &brush);
		void setItemGradient(const QGradient& gradient, bool brush );
		
		QPen itemPen() const;
		QBrush itemBrush() const;
		
		QGraphicsItem* createItem(const QString &xml);
		
	private:
		struct Private;
		Private *const d;
};

#endif
