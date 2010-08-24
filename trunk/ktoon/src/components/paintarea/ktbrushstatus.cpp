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

#include "ktbrushstatus.h"

#include <QLabel>
#include <QHBoxLayout>
#include <QPen>
#include <QBrush>

#include <kgui/kseparator.h>
#include <kcore/kdebug.h>

KTBrushStatus::KTBrushStatus()
{
    QHBoxLayout *layout = new QHBoxLayout(this);
    layout->setMargin(2);
    layout->setSpacing(2);

    m_pen = new KTColorWidget;
    m_brush = new KTColorWidget;

    layout->addWidget(new KSeparator(Qt::Vertical));
    layout->addWidget(new QLabel(tr("Current Color")));
    layout->addSpacing(3);
    layout->addWidget(m_pen);
    layout->addSpacing(5);
}

KTBrushStatus::~KTBrushStatus()
{
}

void KTBrushStatus::setForeground(const QPen &pen)
{
    kFatal() << "KTBrushStatus::setForeground - setting brush from pen";
    m_pen->setBrush(pen.brush());
}

void KTBrushStatus::setBackground(const QBrush &brush)
{
    kFatal() << "KTBrushStatus::setBackground - setting brush from brush"; 
    m_brush->setBrush(brush);
}
