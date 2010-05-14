/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
 
#include "importprojectparser.h"

namespace Parsers {

struct ImportProjectParser::Private
{
    QByteArray data;
};

ImportProjectParser::ImportProjectParser()
    : KTXmlParserBase(), k(new Private())
{
}

ImportProjectParser::~ImportProjectParser()
{
}

bool ImportProjectParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
    if (root() == "importproject") {
        if (tag == "data")
            setReadText(true);
    }

    return true;
}

bool ImportProjectParser::endTag(const QString &tag)
{
    return true;
}

void ImportProjectParser::text(const QString &text)
{
    k->data = QByteArray::fromBase64(text.toLocal8Bit());;
}

QByteArray ImportProjectParser::data() const
{
    return k->data;
}

}
