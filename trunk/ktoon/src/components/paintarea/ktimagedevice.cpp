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

#include "ktimagedevice.h"
#include <QPainter>
#include <QPaintEngine>
#include <QTimer>

#include <kcore/kdebug.h>

KTImageDevice::KTImageDevice(QWidget *parent) : QWidget(parent)
{
    KINIT;

    m_image = QImage(300,300, QImage::Format_RGB32);
    m_image.fill(Qt::white);

    // setAttribute(Qt::WA_PaintOnScreen);
    // setAttribute(Qt::WA_NoSystemBackground);
    // setAttribute(Qt::WA_OpaquePaintEvent);
    // setAutoFillBackground(true);
}

KTImageDevice::~KTImageDevice()
{
    KEND;
}

QPaintEngine *KTImageDevice::paintEngine() const
{
    qDebug("KTImageDevice: paint engine *****************************************");

    return QWidget::paintEngine();
}

void KTImageDevice::paintEvent(QPaintEvent *)
{
    qDebug("KTImageDevice: Paint event #####################################################");
    QPainter p(this);
    p.drawImage(0, 0, m_image);
}
