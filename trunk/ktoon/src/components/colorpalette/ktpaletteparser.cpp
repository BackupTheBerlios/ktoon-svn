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
 *   Copyright (C) 2010 Labtoon SAS - http://www.labtoon.org               *
 *   License:                                                              *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 3 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program.  If not, see <http://www.gnu.org/licenses/>. *
 ***************************************************************************/

#include "ktpaletteparser.h"
#include <kcore/kdebug.h>

struct KTPaletteParser::Private
{
    QString root, qname;
    QString paletteName;
    bool isEditable;
    QList<QBrush> brushes;

    QGradientStops gradientStops;
    QGradient *gradient;

    ~Private()
     {
       delete gradient;
     }
};

KTPaletteParser::KTPaletteParser(): KXmlParserBase(), k(new Private)
{
     // KINIT;
     k->paletteName = "";
     k->isEditable = false;
     k->gradient = 0;
}


KTPaletteParser::~KTPaletteParser()
{
     delete k;
     //	KEND;
}

bool KTPaletteParser::startTag(const QString &tag, const QXmlAttributes &atts)
{
    if (root() == "Palette") {
        if (tag == root()) {
            k->paletteName = atts.value("name");
            if (atts.value("editable") == "true")
                k->isEditable = true;
            else
                k->isEditable = false;
        } else if (tag == "Color") {
                   QColor c = QColor(atts.value("colorName"));
                   c.setAlpha( atts.value("alpha").toInt() );

                   if (c.isValid()) {
                       k->brushes << c;
                   } else {
                    #ifdef K_DEBUG
		           kError() << "Invalid Color";
                    #endif
                   }
        } else if (tag == "Gradient") {
                   if (k->gradient) 
                       delete k->gradient;

                   k->gradient = 0;
                   k->gradientStops.clear();

                   QGradient::Type type = QGradient::Type(atts.value("type").toInt());
                   QGradient::Spread spread = QGradient::Spread(atts.value("spread").toInt());

                   switch (type) {
                           case QGradient::LinearGradient:
                             {
                               k->gradient = new QLinearGradient(atts.value("startX").toDouble(),
                                             atts.value("startY").toDouble(),atts.value("finalX").toDouble(), 
                                             atts.value("finalY").toDouble());
                             }
                             break;
                           case QGradient::RadialGradient:
                             {
                               k->gradient = new QRadialGradient(atts.value("centerX").toDouble(),
                                             atts.value("centerY").toDouble(), atts.value("radius").toDouble(),
                                             atts.value("focalX").toDouble(),atts.value("focalY").toDouble() );
                             }
                             break;
                           case QGradient::ConicalGradient:
                             {
                               k->gradient = new QConicalGradient(atts.value("centerX").toDouble(),
                                             atts.value("centerY").toDouble(),atts.value("angle").toDouble());
                             }
                             break;
                           default:
                             {
                               #ifdef K_DEBUG
                                      kFatal() << "No gradient type: " << type;
                               #endif
                             }
                           break;
                   }
                   k->gradient->setSpread(spread);
        } else if (tag == "Stop") {
                   QColor c(atts.value("colorName") );
                   c.setAlpha(atts.value("alpha").toInt() );
                   k->gradientStops << qMakePair(atts.value("value").toDouble(), c);	
        }
     }

     return true;
}

bool KTPaletteParser::endTag(const QString& tag)
{
    if (root() == "Palette") {
        if (tag == "Gradient" && k->gradient) {
            k->gradient->setStops(k->gradientStops);
            k->brushes << *k->gradient;
            k->gradientStops.clear(); 
        }
    }

    return true;
}

void KTPaletteParser::text(const QString& )
{

}

QList<QBrush> KTPaletteParser::brushes() const
{
    return k->brushes;
}

QString KTPaletteParser::paletteName() const
{
    return k->paletteName;
}

bool KTPaletteParser::paletteIsEditable() const
{
    return k->isEditable;
}
