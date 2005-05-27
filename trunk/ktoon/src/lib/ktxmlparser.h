/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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
#ifndef KTXMLPARSER_H
#define KTXMLPARSER_H

#include <qxml.h>
#include <qmap.h>

typedef QMap<QString, QString> XMLResults;


/**
@author David Cuadrado
*/
class KTXmlParser : public QXmlDefaultHandler
{
	public:
		KTXmlParser();
		~KTXmlParser();
		
		bool startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts);
		
		bool endElement( const QString& ns, const QString& localname, const QString& qname);

		bool characters ( const QString & ch );
		
		XMLResults getResults();
		
	private:
		QString m_root,m_qname;
		XMLResults m_results;
		int fileNumber;
};

#endif
