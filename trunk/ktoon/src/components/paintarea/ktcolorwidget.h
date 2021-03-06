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

#ifndef KTPAINTAREASTATUS_H
#define KTPAINTAREASTATUS_H

#include <QBrush>
#include <QSize>
#include <QPaintEvent>
#include <QWidget>

/**
 * This class defines the options panel in the bottom of the paint area.
 * Controls for Rotation, Antialising and OpenGL
 * @author David Cuadrado <krawek@toonka.com>
*/

class KTColorWidget : public QWidget
{
    Q_OBJECT

    public:
        KTColorWidget() : m_brush(Qt::transparent) {};
        ~KTColorWidget() {};
        void setBrush(const QBrush &brush);
        QSize sizeHint() const;

    protected:
        void paintEvent(QPaintEvent *);

    private:
        QBrush m_brush;
};

#endif
