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

#include "kxmlparserbase.h"

#include <kdebug.h>

struct KXmlParserBase::Private
{
    QString currentTag;
    QString root;
    bool isParsing;
    bool readText;
    bool ignore;
    QString document;
};

KXmlParserBase::KXmlParserBase() : QXmlDefaultHandler(), k(new Private)
{
}

KXmlParserBase::~KXmlParserBase()
{
    delete k;
}

void KXmlParserBase::initialize()
{
}

bool KXmlParserBase::startDocument()
{
    k->isParsing = true;
    k->currentTag = QString();
    k->root = QString();
    k->readText = false;
    k->ignore = false;
    
    initialize();

    return true;
}

bool KXmlParserBase::endDocument()
{
    k->isParsing = false;
    return true;
}

bool KXmlParserBase::startElement(const QString& , const QString& , const QString& qname, const QXmlAttributes& atts)
{
    if (k->ignore) 
        return true;
    
    if (k->root.isEmpty())
        k->root = qname;
    
    bool r = startTag(qname, atts);
    
    k->currentTag = qname;
    
    return r;
}


bool KXmlParserBase::endElement(const QString&, const QString& , const QString& qname)
{
    return endTag(qname);
}


bool KXmlParserBase::characters(const QString & ch)
{
    if (k->ignore) 
        return true;
    
    if (k->readText) {
        text(ch.simplified());
        k->readText = false;
    }
    
    return true;
}

bool KXmlParserBase::error(const QXmlParseException & exception)
{
    kWarning() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
    kWarning() << __PRETTY_FUNCTION__ << " Document: " << k->document;

    return true;
}

bool KXmlParserBase::fatalError(const QXmlParseException & exception)
{
    kFatal() << exception.lineNumber() << "x" << exception.columnNumber() << ": " << exception.message();
    kWarning() << __PRETTY_FUNCTION__ << " Document: " << k->document;

    return true;
}

void KXmlParserBase::setReadText(bool read)
{
    k->readText = read;
}

void KXmlParserBase::setIgnore(bool ignore)
{
    k->ignore = ignore;
}

QString KXmlParserBase::currentTag() const
{
    return k->currentTag;
}

QString KXmlParserBase::root() const
{
    return k->root;
}

bool KXmlParserBase::parse(const QString &doc)
{
    QXmlSimpleReader reader;
    
    reader.setContentHandler(this);
    reader.setErrorHandler(this);
    
    QXmlInputSource xmlsource;
    xmlsource.setData(doc);
    
    k->document = doc;
    
    return reader.parse(&xmlsource);
}

bool KXmlParserBase::parse(QFile *file)
{
    if (!file->isOpen()) {
        if (! file->open(QIODevice::ReadOnly | QIODevice::Text)) {
            kWarning() << "Cannot open file " << file->fileName();
            return false;
        }
    }
    
    return parse(QString::fromLocal8Bit(file->readAll()));
}
