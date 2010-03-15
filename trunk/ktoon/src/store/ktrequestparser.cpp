/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9a                              *
 *   Project Contact: ktoon@labtoon.org                                    *
 *   Project Website: http://www.ktoon.net                                 *
 *   Project Leader: Gustavo Gonzalez <xtingray@ktoon.net>                 *
 *                                                                         *
 *   Developers:                                                           *
 *   2010:                                                                 * 
 *    Gustavo Gonzalez                                                     *
 *   2006:                                                                 *
 *    David Cuadrado                                                       *
 *    Jorge Cuadrado                                                       *
 *   2003:                                                                 *
 *    Fernado Roldan                                                       * 
 *    Simena Dinas                                                         *
 *                                                                         *
 *   License:                                                              *
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

#include "ktrequestparser.h"
#include <kcore/kdebug.h>

#include <QXmlSimpleReader>
#include <QXmlInputSource>

#include "ktprojectresponse.h"

struct KTRequestParser::Private
{
    QString sign;
    KTProjectResponse *response;
};

KTRequestParser::KTRequestParser() : KTXmlParserBase(), k(new Private())
{
    k->response = 0;
}

KTRequestParser::~KTRequestParser()
{
    delete k;
}

void KTRequestParser::initialize()
{
    k->response = 0;
}

bool KTRequestParser::startTag(const QString& qname, const QXmlAttributes& atts)
{
    if (qname == "request") {
        k->sign = atts.value("sign");
    } else if (qname == "item") {
               static_cast<KTItemResponse *>(k->response)->setItemIndex(atts.value("index").toInt());
    } else if (qname == "frame") {
               static_cast<KTFrameResponse *>(k->response)->setFrameIndex(atts.value("index").toInt());
    } else if (qname == "data") {
               setReadText(true);
    } else if (qname == "layer") {
               static_cast<KTLayerResponse *>(k->response)->setLayerIndex(atts.value("index").toInt());
    } else if (qname == "scene") {
               static_cast<KTSceneResponse *>(k->response)->setSceneIndex(atts.value("index").toInt());
    } else if (qname == "symbol") {
               static_cast<KTLibraryResponse*>(k->response)->setSymbolType(atts.value("type").toInt());
    } else if (qname == "action") {
               k->response = KTProjectResponseFactory::create( atts.value("part").toInt(), atts.value("id").toInt());
               k->response->setArg(atts.value("arg"));
    }

    return true;
}

bool KTRequestParser::endTag(const QString& qname)
{
    return true;
}

void KTRequestParser::text( const QString &ch )
{
    if (currentTag() == "data")
        k->response->setData(QByteArray::fromBase64(QByteArray(ch.toLocal8Bit())));
}

KTProjectResponse *KTRequestParser::response() const
{
    return k->response;
}

QString KTRequestParser::sign() const
{
    return k->sign;
}
