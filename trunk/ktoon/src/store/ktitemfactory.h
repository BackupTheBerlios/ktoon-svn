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

#include <QXmlDefaultHandler>
#include <QStack>

class QGraphicsItem;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTItemFactory : public QXmlDefaultHandler
{
	public:
		KTItemFactory();
		~KTItemFactory();
		
		/**
		 * Analiza etiquetas de apertura del documento XML
		 */
		bool startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts);
		
		/**
		 * Analiza etiquetas de cierre del documento XML
		 */
		bool endElement( const QString& ns, const QString& localname, const QString& qname);
		
		/**
		 * Muestra errores en el analisis del documento
		 */
		bool error ( const QXmlParseException & exception );
		
		/**
		 * Muestra errores fatales en el analisis del documento
		 */
		bool fatalError ( const QXmlParseException & exception );
		
	public:
		QGraphicsItem *create(const QString &xml);
		
	private:
		QString m_qname;
		QString m_root;
		
		QGraphicsItem *m_item;
		
		QStack<QGraphicsItem *> m_childs;
};

#endif
