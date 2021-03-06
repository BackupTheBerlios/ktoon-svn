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

#ifndef QBEZIER_P_H
#define QBEZIER_P_H

//
//  W A R N I N G
//  -------------
//
// This file is not part of the Qt API.  It exists for the convenience
// of other Qt classes.  This header file may change from version to
// version without notice, or even be removed.
//
// We mean it.
//

#include "QtCore/qpoint.h"
#include "QtCore/qline.h"
#include "QtCore/qrect.h"
#include "QtCore/qvector.h"
#include "QtCore/qlist.h"

class QPolygonF;

class Q_GUI_EXPORT QBezier
{
public:
    static QBezier fromPoints(const QPointF &p1, const QPointF &p2,
                              const QPointF &p3, const QPointF &p4);

    inline QPointF pointAt(qreal t) const;
    inline QPointF normalVector(qreal t) const;

    QPolygonF toPolygon() const;
    void addToPolygon(QPolygonF *p) const;
    void addToPolygonIterative(QPolygonF *p) const;
    void addToPolygonMixed(QPolygonF *p) const;
    QRectF bounds() const;
    qreal length(qreal error = 0.01) const;
    void addIfClose(qreal *length, qreal error) const;

    qreal tAtLength(qreal len) const;

    QPointF pt1() const { return QPointF(x1, y1); }
    QPointF pt2() const { return QPointF(x2, y2); }
    QPointF pt3() const { return QPointF(x3, y3); }
    QPointF pt4() const { return QPointF(x4, y4); }

    inline QPointF midPoint() const;
    inline QLineF midTangent() const;

    inline QLineF startTangent() const;
    inline QLineF endTangent() const;

    inline void parameterSplitLeft(qreal t, QBezier *left);
    inline void split(QBezier *firstHalf, QBezier *secondHalf) const;
    int shifted(QBezier *curveSegments, int maxSegmets,
                qreal offset, float threshold) const;

    QVector< QList<QBezier> > splitAtIntersections(QBezier &a);

    QBezier bezierOnInterval(qreal t0, qreal t1) const;

    static QVector< QList<qreal> > findIntersections(const QBezier &a,
                                                     const QBezier &b);

    qreal x1, y1, x2, y2, x3, y3, x4, y4;
};

inline QPointF QBezier::midPoint() const
{
    return QPointF((x1 + x4 + 3*(x2 + x3))/8., (y1 + y4 + 3*(y2 + y3))/8.);
}

inline QLineF QBezier::midTangent() const
{
    QPointF mid = midPoint();
    QLineF dir(QLineF(x1, y1, x2, y2).pointAt(0.5), QLineF(x3, y3, x4, y4).pointAt(0.5));
    return QLineF(mid.x() - dir.dx(), mid.y() - dir.dy(),
                  mid.x() + dir.dx(), mid.y() + dir.dy());
}

inline QLineF QBezier::startTangent() const
{
    QLineF tangent(pt1(), pt2());
    if (tangent.isNull())
        tangent = QLineF(pt1(), pt3());
    if (tangent.isNull())
        tangent = QLineF(pt1(), pt4());
    return tangent;
}

inline QLineF QBezier::endTangent() const
{
    QLineF tangent(pt4(), pt3());
    if (tangent.isNull())
        tangent = QLineF(pt4(), pt2());
    if (tangent.isNull())
        tangent = QLineF(pt4(), pt1());
    return tangent;
}

inline QPointF QBezier::pointAt(qreal t) const
{
    Q_ASSERT(t >= 0);
    Q_ASSERT(t <= 1);
#if 1
    qreal a, b, c, d;
    qreal m_t = 1. - t;
    b = m_t * m_t;
    c = t * t;
    d = c * t;
    a = b * m_t;
    b *= 3. * t;
    c *= 3. * m_t;

    return QPointF(a*x1 + b*x2 + c*x3 + d*x4, a*y1 + b*y2 + c*y3 + d*y4);
#else
    // numerically more stable:
    qreal m_t = 1. - t;
    qreal a = x1*m_t + x2*t;
    qreal b = x2*m_t + x3*t;
    qreal c = x3*m_t + x4*t;
    a = a*m_t + b*t;
    b = b*m_t + c*t;
    qreal x = a*m_t + b*t;
    qreal a = y1*m_t + y2*t;
    qreal b = y2*m_t + y3*t;
    qreal c = y3*m_t + y4*t;
    a = a*m_t + b*t;
    b = b*m_t + c*t;
    qreal y = a*m_t + b*t;
    return QPointF(x, y);
#endif
}

inline QPointF QBezier::normalVector(qreal t) const
{
    qreal m_t = 1. - t;
    qreal a = m_t * m_t;
    qreal b = t * m_t;
    qreal c = t * t;

    return QPointF((y2-y1) * a + (y3-y2) * b + (y4-y3) * c,  -(x2-x1) * a - (x3-x2) * b - (x4-x3) * c);
}

inline void QBezier::split(QBezier *firstHalf, QBezier *secondHalf) const
{
    Q_ASSERT(firstHalf);
    Q_ASSERT(secondHalf);

    qreal c = (x2 + x3)*.5;
    firstHalf->x2 = (x1 + x2)*.5;
    secondHalf->x3 = (x3 + x4)*.5;
    firstHalf->x1 = x1;
    secondHalf->x4 = x4;
    firstHalf->x3 = (firstHalf->x2 + c)*.5;
    secondHalf->x2 = (secondHalf->x3 + c)*.5;
    firstHalf->x4 = secondHalf->x1 = (firstHalf->x3 + secondHalf->x2)*.5;

    c = (y2 + y3)/2;
    firstHalf->y2 = (y1 + y2)*.5;
    secondHalf->y3 = (y3 + y4)*.5;
    firstHalf->y1 = y1;
    secondHalf->y4 = y4;
    firstHalf->y3 = (firstHalf->y2 + c)*.5;
    secondHalf->y2 = (secondHalf->y3 + c)*.5;
    firstHalf->y4 = secondHalf->y1 = (firstHalf->y3 + secondHalf->y2)*.5;
}

inline void QBezier::parameterSplitLeft(qreal t, QBezier *left)
{
    left->x1 = x1;
    left->y1 = y1;

    left->x2 = x1 + t * ( x2 - x1 );
    left->y2 = y1 + t * ( y2 - y1 );

    left->x3 = x2 + t * ( x3 - x2 ); // temporary holding spot
    left->y3 = y2 + t * ( y3 - y2 ); // temporary holding spot

    x3 = x3 + t * ( x4 - x3 );
    y3 = y3 + t * ( y4 - y3 );

    x2 = left->x3 + t * ( x3 - left->x3);
    y2 = left->y3 + t * ( y3 - left->y3);

    left->x3 = left->x2 + t * ( left->x3 - left->x2 );
    left->y3 = left->y2 + t * ( left->y3 - left->y2 );

    left->x4 = x1 = left->x3 + t * (x2 - left->x3);
    left->y4 = y1 = left->y3 + t * (y2 - left->y3);
}

#endif // QBEZIER_P_H
