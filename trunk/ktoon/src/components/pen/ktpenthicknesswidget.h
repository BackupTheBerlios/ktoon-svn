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

#ifndef KTPENTHICKNESSWIDGET_H
#define KTPENTHICKNESSWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QPaintEvent>
#include <QPen>
#include <QSize>

/**
 * This class shows a preview of the pen thickness 
 * @author Gustavo Gonzalez <xtingray@ktoon.net>
 **/

class KTPenThicknessWidget : public QWidget
{
    Q_OBJECT

    public:
        KTPenThicknessWidget(QWidget *parent = 0);
        ~KTPenThicknessWidget();

        QSize minimumSizeHint() const;
        QSize sizeHint() const;
        void setColor(const QColor color);
        void setBrush(int index);

    public slots:
        void render(int thickness);
        
    protected:
        void paintEvent(QPaintEvent *e);
   
    private:
        struct Private;
        Private *const k;
};

#endif
