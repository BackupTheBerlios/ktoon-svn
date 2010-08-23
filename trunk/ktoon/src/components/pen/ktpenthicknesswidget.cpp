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

#include "ktpenthicknesswidget.h"
#include <kcore/kdebug.h>

struct KTPenThicknessWidget::Private
{
    int thickness;
    QColor color;
};

KTPenThicknessWidget::KTPenThicknessWidget(QWidget *parent) : QWidget(parent), k(new Private)
{
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
}

KTPenThicknessWidget::~KTPenThicknessWidget()
{
}

void KTPenThicknessWidget::render(int thickness)
{
    k->thickness = thickness;
    update();
}

void KTPenThicknessWidget::setColor(const QColor color)
{
    k->color = color;
}

QSize KTPenThicknessWidget::minimumSizeHint() const
{
    return QSize(100, 106);
}

QSize KTPenThicknessWidget::sizeHint() const
{
    return QSize(100, 106);
}

void KTPenThicknessWidget::paintEvent(QPaintEvent *)
{
     QPainter painter(this);
     painter.setRenderHint(QPainter::Antialiasing, true);
     painter.fillRect(0, 0, width(), height(), QColor(255, 255, 255));

     QPen border(QColor(0, 0, 0));
     border.setWidth(0.5);
     painter.setPen(border);
     painter.drawRect(0, 0, width(), height());

     painter.translate(width() / 2, height() / 2);

     QBrush brush(k->color, Qt::SolidPattern);
     QPen pen(Qt::NoPen);

     painter.setPen(pen);
     painter.setBrush(brush);
     painter.drawEllipse(-(k->thickness/2), -(k->thickness/2), k->thickness, k->thickness);
}
