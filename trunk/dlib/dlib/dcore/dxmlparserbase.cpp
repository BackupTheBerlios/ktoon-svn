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

#include "dxmlparserbase.h"

#include <ddebug.h>

struct DXmlParserBase::Private
{
	QString currentTag;
	QString root;
	bool isParsing;
	bool readText;
	bool ignore;
	QString document;
};

DXmlParserBase::DXmlParserBase() : QXmlDefaultHandler(), d(new Private)
{
}


DXmlParserBase::~DXmlParserBase()
{
	delete d;
}

void DXmlParserBase::initialize()
{
}


bool DXmlParserBase::startDocument()
{
	d->isParsing = true;
	
	d->currentTag = QString();
	d->root = QString();
	d->readText = false;
	d->ignore = false;
	
	initialize();
	return true;
}

bool DXmlParserBase::endDocument()
{
	d->isParsing = false;
	return true;
}

bool DXmlParserBase::startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
	if ( d->ignore ) return true;
	
	if ( d->root.isEmpty() )
	{
		d->root = qname;
	}
	
	bool r = startTag(qname, atts);
	
	d->currentTag = qname;
	
	return r;
}


bool DXmlParserBase::endElement( const QString&, const QString& , const QString& qname)
{
	return endTag(qname);
}


bool DXmlParserBase::characters(const QString & ch)
{
	if ( d->ignore ) return true;
	
	if ( d->readText )
	{
		text(ch.simplified());
		d->readText = false;
	}
	
	return true;
}

bool DXmlParserBase::error ( const QXmlParseException & exception )
{
	dWarning() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	dWarning() << __PRETTY_FUNCTION__ << " Document: " << d->document;
	return true;
}

bool DXmlParserBase::fatalError ( const QXmlParseException & exception )
{
	dFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
	dWarning() << __PRETTY_FUNCTION__ << " Document: " << d->document;
	return true;
}


void DXmlParserBase::setReadText(bool read)
{
	d->readText = read;
}

void DXmlParserBase::setIgnore(bool ignore)
{
	d->ignore = ignore;
}

QString DXmlParserBase::currentTag() const
{
	return d->currentTag;
}

QString DXmlParserBase::root() const
{
	return d->root;
}

bool DXmlParserBase::parse(const QString &doc)
{
	QXmlSimpleReader reader;
	
	reader.setContentHandler(this);
	reader.setErrorHandler(this);
	
	QXmlInputSource xmlsource;
	xmlsource.setData(doc);
	
	d->document = doc;
	
	return reader.parse(&xmlsource);
}


bool DXmlParserBase::parse(QFile *file)
{
	if ( !file->isOpen() )
	{
		if( ! file->open(QIODevice::ReadOnly | QIODevice::Text) )
		{
			dWarning() << "Cannot open file " << file->fileName();
			return false;
		}
	}
	
	return parse(QString::fromLocal8Bit(file->readAll()) );
}

