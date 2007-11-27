/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KXMLPARSERBASE_H
#define KXMLPARSERBASE_H

#include <QXmlDefaultHandler>
#include "kglobal.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_CORE_EXPORT KXmlParserBase : public QXmlDefaultHandler
{
	public:
		~KXmlParserBase();
		
		
	protected:
		KXmlParserBase();
		
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
		Private *const k;
};

#endif
