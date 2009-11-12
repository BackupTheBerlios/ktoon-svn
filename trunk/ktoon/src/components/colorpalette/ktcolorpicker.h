/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by Jorge Cuadrado <kuadrosx@toonka.com>            *
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

#include <QFrame>

#ifndef KTCOLORPICKER_H
#define KTCOLORPICKER_H

class KTColorPicker : public QFrame
{
    Q_OBJECT
    public:
        KTColorPicker(QWidget* parent);
        ~KTColorPicker();
        int hue();
        int sat();

    public slots:
        void setCol(int h, int s);
        void setH(int h);
        void setS(int s);

    signals:
        void newCol(int h, int s);

    protected:
        QSize sizeHint() const;
        void paintEvent(QPaintEvent*);
        void mouseMoveEvent(QMouseEvent *);
        void mousePressEvent(QMouseEvent *);

    private:
        struct Private;
        Private *const k;

        QPoint colPt();
        int huePt(const QPoint &pt);
        int satPt(const QPoint &pt);
        void setCol(const QPoint &pt);
};

#endif

