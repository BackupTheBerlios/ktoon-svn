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

#ifndef KCOLORBUTTON_H
#define KCOLORBUTTON_H

#include <QAbstractButton>
#include <QMouseEvent>
#include <QDragEnterEvent>
#include <QDragLeaveEvent>
#include <QDragMoveEvent>
#include <QDropEvent>
#include "kcore/kglobal.h"

class QColor;

class K_GUI_EXPORT KColorButton : public QAbstractButton
{
    Q_OBJECT;
    Q_PROPERTY(QColor color READ color WRITE setColor);

    public:
        KColorButton(QWidget* parent = 0);
        ~KColorButton();

        void setColor(const QColor&);
        QColor color() const;

        QSize sizeHint() const;
        QSize minimumSizeHint() const;
        void setPalette(const QPalette &);

    public slots:
        virtual void showEditor();

    signals:
        void colorChanged(const QColor &color);

    protected:
        void paintEvent(QPaintEvent *e);
        void mousePressEvent(QMouseEvent* e);
        void mouseMoveEvent(QMouseEvent* e);
        void dragEnterEvent(QDragEnterEvent* e);
        void dragMoveEvent(QDragMoveEvent* e);
        void dropEvent(QDropEvent* e);

    private:
        QColor m_color;
        QPoint m_position;
};

#endif //KCOLORBUTTON_H
