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

#ifndef KTLUMINACEPICKER_H
#define KTLUMINACEPICKER_H

#include <QPaintEvent>
#include <QPixmap>
#include <QMouseEvent>
#include <QWidget>

class KTLuminancePicker : public QWidget
{
    Q_OBJECT
    public:
        KTLuminancePicker(QWidget* parent=0);
        ~KTLuminancePicker();
        int value();

    public slots:
        void setCol(int h, int s, int v);
        void setCol(int h, int s);
        void setVal(int v);

    signals:
        void newHsv(int h, int s, int v);

    protected:
        void paintEvent(QPaintEvent*);
        void mouseMoveEvent(QMouseEvent *);
        void mousePressEvent(QMouseEvent *);

    private:
        enum { foff = 3, coff = 4 }; //frame and contents offset

    struct Private;
    Private *const k;
    int y2val(int y);
    int val2y(int val);
};

#endif

