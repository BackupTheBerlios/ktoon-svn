/***************************************************************************
 *   Project KOM: KToon Open Media library                                 *
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

#include "kseparator.h"
#include <QStyleOption>
#include <QPainter>

KSeparator::KSeparator(QWidget* parent) : QFrame(parent)
{
   setLineWidth(1);
   setMidLineWidth(0);
   setOrientation(Qt::Horizontal);
}

KSeparator::KSeparator(Qt::Orientation orientation, QWidget* parent)
   : QFrame(parent)
{
   setLineWidth(1);
   setMidLineWidth(0);
   setOrientation(orientation);
}

void KSeparator::setOrientation(Qt::Orientation orientation)
{
   if (orientation == Qt::Vertical) {
      setFrameShape(QFrame::VLine);
      setFrameShadow(QFrame::Sunken);
      setMinimumSize(2, 0);
   } else {
      setFrameShape(QFrame::HLine);
      setFrameShadow(QFrame::Sunken);
      setMinimumSize(0, 2);
   }
}

Qt::Orientation KSeparator::orientation() const
{
   return (frameStyle() & VLine) ? Qt::Vertical : Qt::Horizontal;
}

void KSeparator::virtual_hook(int, void*)
{ 
    /*BASE::virtual_hook( id, data );*/ 
}
