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


#ifndef KANIMWIDGET_H
#define KANIMWIDGET_H
/**
 * @file kanimwidget.h
 * @brief Include this file if you need the class KAnimWidget
 */

#include <QPixmap>
#include <QHideEvent>
#include <QShowEvent>
#include <QList>
#include <QWidget>
#include "kcore/kglobal.h"

typedef QList<QPixmap> ListOfPixmaps;

/**
 * @if english
 * @short The class KAnimWidget provides a widget containing a simple animation of moving text or image sequences  
 * @elseif spanish
 * @short La clase KAnimWidget provee de un widget que hace una simple animacion, de un texto o una secuencia de imagenes.
 * 
 * @Author David Cuadrado
 * @endif
 */
class K_GUI_EXPORT KAnimWidget : public QWidget
{
    public:
        enum Type { AnimText = 0, AnimPixmap };

        /**
          * Builds a KAnimWidget object with a background, an animated text and a parent widget
          * @param px background
          * @param text text to animate 
          * @param parent 
          */
        KAnimWidget(const QPixmap &px, const QString &text, QWidget *parent = 0);

        /**
          * Builds a KAnimWidget object with a background, an image list and a parent widget
          * @param lop images to animate 
          * @param parent 
          */
        KAnimWidget(ListOfPixmaps lop, QWidget *parent = 0);

        /**
         * Destructor
         */
        ~KAnimWidget();

        /**
         * set an image as animation background
         * @param px background 
         */
        void setBackgroundPixmap(const QPixmap &px);

    protected:
        /**
         * Starts animation
         */
        void showEvent(QShowEvent * e);

        /**
         * Stops animation 
         */
        void hideEvent(QHideEvent * e);

    protected:
        /**
         * Handles animation flow
         */
        void timerEvent(QTimerEvent *e);

        /**
         * Draws animation
         */
        void paintEvent(QPaintEvent *e);

    private:
        Type m_type;
        class Controller;
        Controller *m_controller;
        QPixmap m_background;
        QString m_text;
        QRectF m_textRect;

        ListOfPixmaps m_pixmaps;
        int m_pixmapIndex;
        int m_counter;
        int m_lines;
        int fontSize;
        int m_end;
};

#endif
