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

#include "ktpalettedocument.h"

KTPaletteDocument::KTPaletteDocument(const QString &name, bool isEditable) : QDomDocument()
{
    QDomProcessingInstruction header = this->createProcessingInstruction("xml","version=\"1.0\" encoding=\"UTF-8\"");
    this->appendChild(header);
    
    QDomElement root = createElement("Palette");
    root.setAttribute("name", name);
    
    if (isEditable)
        root.setAttribute("editable", "true");
    else
        root.setAttribute("editable", "false");
    
    appendChild(root);
}

KTPaletteDocument::~KTPaletteDocument()
{
}

void KTPaletteDocument::addColor(const QColor &color)
{
    QDomElement element = createElement("Color");
    
    element.setAttribute("colorName", color.name());
    element.setAttribute("alpha", QString::number(color.alpha()));
    
    documentElement().appendChild(element);
}

void KTPaletteDocument::addGradient(const QGradient &gradient)
{
    QDomElement element = createElement("Gradient");
    
    element.setAttribute("type", gradient.type());
    element.setAttribute("spread", gradient.spread());
    const QGradient *gr = &gradient;
    switch (gradient.type()) {
            case QGradient::LinearGradient:
            {
                 element.setAttribute("startX", static_cast<const QLinearGradient *>(gr)->start().x());
                 element.setAttribute("startY", static_cast<const QLinearGradient *>(gr)->start().y());
                
                 element.setAttribute("finalX", static_cast<const QLinearGradient *>(gr)->finalStop().x());
                 element.setAttribute("finalY", static_cast<const QLinearGradient *>(gr)->finalStop().y());
            }
            break;
            case QGradient::RadialGradient:
            {
                 element.setAttribute("centerX", static_cast<const QRadialGradient *>(gr)->center().x());
                 element.setAttribute("centerY", static_cast<const QRadialGradient *>(gr)->center().y());
                
                 element.setAttribute("focalX", static_cast<const QRadialGradient *>(gr)->focalPoint().x());
                 element.setAttribute("focalY", static_cast<const QRadialGradient *>(gr)->focalPoint().y());
                
                 element.setAttribute("radius", static_cast<const QRadialGradient *>(gr)->radius());
            }
            break;
            case QGradient::ConicalGradient:
            {
                 element.setAttribute("centerX", static_cast<const QRadialGradient *>(gr)->center().x());
                 element.setAttribute("centerY", static_cast<const QRadialGradient *>(gr)->center().y());
                
                 element.setAttribute("angle", static_cast<const QConicalGradient *>(gr)->angle());
            }
            break;
            case QGradient::NoGradient:
            {
            }
            break;
    }
    
    QGradientStops stops = gradient.stops();
    
    foreach (QGradientStop stop, stops) {
             QDomElement stopElement = createElement("Stop");
             stopElement.setAttribute("value", stop.first);
             stopElement.setAttribute("colorName", stop.second.name());
             stopElement.setAttribute("alpha", stop.second.alpha());
        
             element.appendChild(stopElement);
    }
    
    documentElement().appendChild(element);
}

void KTPaletteDocument::setElements(const QList<QBrush > &brushes)
{
    foreach (QBrush brush, brushes) {
             if (brush.gradient())
                 addGradient(*brush.gradient());
             else
                 addColor(brush.color());
    }
}
