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

KTSplash::KTSplash() : QSplashScreen(0), counter(0)
{
    flag = true;

    QImage image(THEME_DIR + "images/splash.png");
    setPixmap(QPixmap::fromImage(image));
    version = tr("Version ") + kAppProp->version() + "+" + kAppProp->codeName();

    state = 0;

    pos[0] = 10;
    pos[1] = 21;
    pos[2] = 32;
}

KTSplash::~KTSplash()
{
}

void KTSplash::animate()
{
    repaint();
    state++;
}

void KTSplash::setMessage(const QString &msg)
{
    QSplashScreen::showMessage(msg, Qt::AlignTop, palette().text().color());
    message = msg;
    QString number;
    number = number.setNum(counter);
    QString path = THEME_DIR + "images/splash" + number + ".png";
    QImage image(path);
    setPixmap(QPixmap::fromImage(image));
    counter++;

    animate();

    QTimer::singleShot(10, this, SLOT(animate()));
}

void KTSplash::drawContents(QPainter * painter)
{
    if (flag) {
        flag = false;
        return;
    } 

    // Draw background circles
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(230, 230, 230));
    painter->drawEllipse(8, 275, 9, 9);
    painter->drawEllipse(19, 275, 9, 9);
    painter->drawEllipse(30, 275, 9, 9);

    QColor fill = palette().background().color();
    fill.setAlpha(0);

    painter->fillRect(rect(), fill);
    painter->save();

    if (state < 3) {
        painter->setBrush(QColor(120,200,120));
        painter->drawEllipse(pos[state], 277, 5, 5);
    } else {
        state = 0;
    }

    painter->restore();
    painter->setPen(QColor(0, 0, 0));

    // Draw version number
    QRect r = rect();
    r.setRect(r.x() + 24, r.y() + 5, r.width() - 30, r.height() - 30);

    QFont forig = painter->font();
    painter->setFont(QFont("helvetica", 10, QFont::Normal, false));
    painter->drawText(r, Qt::AlignRight, version);

    painter->setFont(forig);

    // Draw message at given position, limited to 43 chars
    // If message is too long, string is truncated
    if (message.length() > 40) 
        message.truncate(39); 

    //message += "...";
    painter->setFont(QFont("helvetica", 7, 10, false));
    painter->drawText(46, 283, message);
}
