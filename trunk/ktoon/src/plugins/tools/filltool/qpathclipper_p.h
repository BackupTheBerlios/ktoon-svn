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

#ifndef QPATHCLIPPER_P_H
#define QPATHCLIPPER_P_H

#include <QtGui/qpainterpath.h>
#include <QtCore/qlist.h>

QT_BEGIN_HEADER

QT_MODULE(Gui)

class PathVertex;

class Q_GUI_EXPORT QPathClipper
{
public:
    enum Operation {
        BoolAnd,
        BoolOr,
        BoolSub,
        BoolInSub
    };
public:
    QPathClipper();
    QPathClipper(const QPainterPath &subject,
                 const QPainterPath &clip);
    ~QPathClipper();

    void setSubjectPath(const QPainterPath &path);
    QPainterPath subjectPath() const;

    void setClipPath(const QPainterPath &path);
    QPainterPath clipPath() const;

    QPainterPath clip(Operation op=BoolAnd);
    bool intersect();
    bool contains();
private:
    Q_DISABLE_COPY(QPathClipper)
    class Private;
    Private *k;
};

QT_END_HEADER

#endif // QPATHCLIPPER_P_H
