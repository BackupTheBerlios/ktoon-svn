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

KTSplash::KTSplash() : QSplashScreen(0), m_counter(0)
{
    flag = true;

    QImage image(THEME_DIR + "images/splash.png");
    setPixmap(QPixmap::fromImage(image));
    m_version = tr("Version ") + kAppProp->version();

    m_state = 0;

    m_pos[0] = 108;
    m_pos[1] = 119;
    m_pos[2] = 130;
}

KTSplash::~KTSplash()
{
}

void KTSplash::animate()
{
    repaint();
    m_state++;
}

void KTSplash::setMessage(const QString &msg)
{
    QSplashScreen::showMessage(msg, Qt::AlignTop, palette().text().color());
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
    if (flag) {
        flag = false;
        return;
    } 

    // Draw background circles
    painter->setRenderHints(QPainter::Antialiasing);
    painter->setPen(Qt::NoPen);
    painter->setBrush(QColor(230, 230, 230));
    painter->drawEllipse(106, 272, 9, 9);
    painter->drawEllipse(117, 272, 9, 9);
    painter->drawEllipse(128, 272, 9, 9);

    QColor fill = palette().background().color();
    fill.setAlpha(0);

    painter->fillRect(rect(), fill);
    painter->save();

    if (m_state < 3) {
        painter->setBrush(QColor(120,200,120));
        painter->drawEllipse(m_pos[m_state], 274, 5, 5);
    } else {
        m_state = 0;
    }

    painter->restore();
    painter->setPen(QColor(0, 0, 0));

    // Draw version number
    QRect r = rect();
    r.setRect(r.x() + 18, r.y() + 5, r.width() - 30, r.height() - 30);

    QFont forig = painter->font();
    painter->setFont(QFont("helvetica", 12, 10, false));
    painter->drawText(r, Qt::AlignRight, m_version);

    painter->setFont(forig);

    // Draw message at given position, limited to 43 chars
    // If message is too long, string is truncated
    if (m_message.length() > 40) 
        m_message.truncate(39); 

    //m_message += "...";
    painter->setFont(QFont("helvetica", 8, 10, false));
    painter->drawText(146, 280, m_message);
}
