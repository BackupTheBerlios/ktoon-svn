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

#include "ktbrushmanager.h"
#include <kcore/kdebug.h>


/**
 * This class handles the set of brushes for painting.
 * Here is where methods to set/unset brushes are defined.
 * @author David Cuadrado <krawek@toonka.com>
*/

struct KTBrushManager::Private
{
    Private() : pen(QPen(Qt::black, 1, Qt::SolidLine, Qt::RoundCap)), brush(Qt::transparent) {}
    QPen pen;
    QBrush brush;
};

KTBrushManager::KTBrushManager(QObject * parent) : QObject(parent), k(new Private)
{
    kFatal() << "KTBrushManager::KTBrushManager - Just starting object! 1";
}

KTBrushManager::KTBrushManager(const QPen &pen, const QBrush &brush, QObject * parent) : QObject(parent), k(new Private)
{
    kFatal() << "KTBrushManager::KTBrushManager - Just starting object! 2";

    k->pen = pen;
    k->brush = brush;
}

KTBrushManager::~KTBrushManager()
{
    delete k;
}

void KTBrushManager::setPen(const QPen &pen)
{
    kFatal() << "KTBrushManager::setPen - Pen updated!";
    k->pen = pen;
    emit penChanged(pen);
}

void KTBrushManager::setPenColor(const QColor &color)
{
    kFatal() << "KTBrushManager::setPenColor - Pen color updated!";
    k->pen.setColor(color);
    emit penChanged(k->pen);
}

QPen KTBrushManager::pen() const
{
    return k->pen;
}

void KTBrushManager::setBrush(const QBrush &brush)
{
    kFatal() << "KTBrushManager::setBrush - Brush updated!";
    k->brush = brush;
    emit brushChanged(brush);
}

QBrush KTBrushManager::brush() const
{
    return k->brush;
}

int KTBrushManager::penWidth() const
{
    return k->pen.width();
}

QColor KTBrushManager::penColor() const
{
    return k->pen.color();
}

QBrush KTBrushManager::penBrush() const
{
    return k->pen.brush();
}

QBrush KTBrushManager::brushColor() const
{
    return k->brush.color();
}
