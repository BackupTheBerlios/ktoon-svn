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
#ifndef KTSVG2QT_H
#define KTSVG2QT_H

#include <QString>
#include <QPainterPath>
#include <QPen>
#include <QXmlAttributes>
#include <QBrush>
#include "ktglobal.h"

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class KTOON_EXPORT KTSvg2Qt
{
    private:
        KTSvg2Qt();
        ~KTSvg2Qt();
        
    public:
        static bool svgpath2qtpath(const QString &data, QPainterPath &path);
        static bool svgmatrix2qtmatrix(const QString &data, QMatrix &matrix);
        static bool parsePointF(const QString &pointstr, QPointF &point);
        
        static void parsePen(QPen &pen, const QXmlAttributes &attributes);
        static bool parseBrush(QBrush &brush, const QXmlAttributes &attributes);
        
        static QList<qreal> parseNumbersList(QString::const_iterator &itr);
};

#endif
