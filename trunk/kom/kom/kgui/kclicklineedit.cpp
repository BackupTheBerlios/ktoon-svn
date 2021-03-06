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

#include "kclicklineedit.h"

#include <QPainter>

struct KClickLineEdit::Private
{
    QString clickMessage;
    bool drawClickMsg;
};

KClickLineEdit::KClickLineEdit(const QString &msg, QWidget *parent) : QLineEdit(parent), k(new Private)
{
    k->drawClickMsg = true;
    setClickMessage(msg);
    
    setFocusPolicy(Qt::ClickFocus);
    
    setStyleSheet(QString(":enabled { padding-right: %1; }").arg(8));
}

KClickLineEdit::~KClickLineEdit()
{
    delete k;
}

void KClickLineEdit::setClickMessage(const QString &msg)
{
    k->clickMessage = msg;
    repaint();
}

QString KClickLineEdit::clickMessage() const
{
    return k->clickMessage;
}

void KClickLineEdit::setText(const QString &txt)
{
    k->drawClickMsg = txt.isEmpty();
    repaint();
    QLineEdit::setText(txt);
}

void KClickLineEdit::paintEvent(QPaintEvent *e)
{
    QLineEdit::paintEvent(e);
    
    QPainter p(this);
    if (k->drawClickMsg == true && !hasFocus()) {
        QPen tmp = p.pen();
        p.setPen(palette().color(QPalette::Disabled, QPalette::Text));
        QRect cr = contentsRect();
        
        cr.adjust(3, 0, 0 ,0);
        p.drawText(cr, Qt::AlignVCenter, k->clickMessage);
        p.setPen( tmp );
    }
}

void KClickLineEdit::focusInEvent(QFocusEvent *ev)
{
    if (k->drawClickMsg == true) {
        k->drawClickMsg = false;
        repaint();
    }
    QLineEdit::focusInEvent(ev);
}

void KClickLineEdit::focusOutEvent(QFocusEvent *ev)
{
    if (text().isEmpty()) {
        k->drawClickMsg = true;
        repaint();
    }
    QLineEdit::focusOutEvent(ev);
}
