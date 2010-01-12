/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
 *                                                                         *
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

#include "ktsplash.h"

#include <qpixmap.h>
#include <qbitmap.h>
#include <qimage.h>
#include <QtDebug>

#include <cmath> //abs

#include <kgui/kimageeffect.h>
#include <kcore/kdebug.h>
#include <kcore/kglobal.h>

//------------------ CONSTRUCTOR -----------------

KTSplash::KTSplash() : QSplashScreen(0), m_size(3), m_state(60), m_counter(0)
{
    QImage image(THEME_DIR + "images/splash.png");
    setPixmap(QPixmap::fromImage(image));
    m_version = tr("Version ") + kAppProp->version();

    m_timer = new QTimer(this);
    connect(m_timer, SIGNAL(timeout()), this, SLOT(animate()));
    m_timer->start(200);
}

KTSplash::~KTSplash()
{
    delete m_timer;
}

void KTSplash::animate()
{
    m_state -= 10;

    repaint();
}

void KTSplash::setMessage(const QString &msg)
{
    QSplashScreen::showMessage(msg,Qt::AlignTop, palette().text().color());
    m_message = msg;
    QString number;
    number = number.setNum(m_counter);
    QString path = THEME_DIR + "images/splash" + number + ".png";
    QImage image(path);
    setPixmap(QPixmap::fromImage(image));
    m_counter++;

    animate();

    QTimer::singleShot(10, this, SLOT(animate()));
}

void KTSplash::drawContents(QPainter * painter)
{
    int position;

    // Draw background circles
    painter->setPen(Qt::NoPen);
    painter->setBrush(palette().background());
    painter->drawEllipse(51,7,9,9);
    painter->drawEllipse(62,7,9,9);
    painter->drawEllipse(73,7,9,9);

    QColor fill = palette().background().color();

    fill.setAlpha(m_state);

    painter->fillRect(rect(), fill);
    painter->save();

    QColor baseColor = palette().alternateBase().color();
    for (int i = 0; i < m_size; i++) {
         position = (m_state+i)%(2*m_size-1);
         int r = abs(baseColor.red()-18*i)%255;
         int g = abs(baseColor.green()-10*i)%255;
         int b = abs(baseColor.blue()-28*i)%255;
         painter->setBrush(QColor(r,g,b));

         if (position < 3)
             painter->drawEllipse(51+position*11, 7, 9, 9);
    }

    painter->restore();

    painter->setPen(0xCFCDD3);

    // Draw version number
    QRect r = rect();
    r.setRect(r.x() + 5, r.y() + 22, r.width() - 30, r.height() - 30);

    QFont forig = painter->font();
    painter->setFont(QFont("helvetica", 12, 10, true));
    painter->drawText(r, Qt::AlignRight, m_version);

    painter->setFont(forig);

    // Draw message at given position, limited to 43 chars
    // If message is too long, string is truncated
    if (m_message.length() > 40) 
        m_message.truncate(39); m_message += "...";

    painter->drawText (90, 16, m_message);
}
