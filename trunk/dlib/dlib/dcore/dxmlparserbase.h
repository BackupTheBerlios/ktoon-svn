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

#ifndef DXMLPARSERBASE_H
#define DXMLPARSERBASE_H

#include <QXmlDefaultHandler>
#include "dglobal.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class D_CORE_EXPORT DXmlParserBase : public QXmlDefaultHandler
{
	public:
		~DXmlParserBase();
		
		
	protected:
		DXmlParserBase();
		
		bool startDocument();
		bool endDocument();
		
		bool startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts);
		bool characters(const QString & ch);
		bool endElement( const QString& ns, const QString& localname, const QString& qname);
		bool error ( const QXmlParseException & exception );
		bool fatalError ( const QXmlParseException & exception );
		
	protected:
		void setReadText(bool read);
		void setIgnore(bool ignore);
		
	public:
		virtual void initialize();
		virtual bool startTag(const QString &tag, const QXmlAttributes &atts) = 0;
		virtual bool endTag(const QString &tag) = 0;
		virtual void text(const QString &text) = 0;
		
	public:
		QString currentTag() const;
		QString root() const;
		
		bool parse(const QString &doc);
		bool parse(QFile *file);
		
	private:
		struct Private;
		Private *const d;
};

#endif
