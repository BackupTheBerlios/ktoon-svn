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

#ifndef KTBRUSHESPARSER_H
#define KTBRUSHESPARSER_H

#include <QXmlDefaultHandler>
#include <QList>
#include <QPainterPath>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTBrushesParser : public QXmlDefaultHandler
{
	public:
		KTBrushesParser();
		~KTBrushesParser();
		bool startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts);
		
		bool endElement( const QString& ns, const QString& localname, const QString& qname);
		
		bool error ( const QXmlParseException & exception );
		bool fatalError ( const QXmlParseException & exception );
		
		QList<QPainterPath> brushes();
		
	private:
		QString m_root,m_qname;
		QList<QPainterPath> m_brushes;
		QStringList m_tmpPolygons;
};

#endif