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

#ifndef __KTGRAPHICALGORITHM_H__
#define __KTGRAPHICALGORITHM_H__

#include <QString>
#include <QPolygon>
#include <QPainterPath>
#include "ktglobal.h"

/**
 * @author Jorge Cuadrado <kuadrosx@toonka.com>
 */

class KTOON_EXPORT KTGraphicalAlgorithm
{
    private:
        KTGraphicalAlgorithm() {}
        ~KTGraphicalAlgorithm() {};
        
    public:
        static QPainterPath bezierFit(QPolygonF &points_, float error, int from = 0, int to = -1);
        static QPolygonF polygonFit(const QPolygonF &points);
        static bool intersectLine(const QPointF &start, const QPointF& end, const QRectF& rect);
        static char calculateCode(const QPointF &point, const QRectF &window);
        static double distanceToPoint(const QPointF &pos);
        static double angleForPos(const QPointF &pos, const QPointF &anchor = QPointF(0,0));
        
    private:
        enum CohenSutherlandCode
        {
            Bit1 = 1<<1,
            Bit2 = 1<<2,
            Bit3 = 1<<3,
            Bit4 = 1<<4
        };
        
        static void printCode(char code);
};

#endif
