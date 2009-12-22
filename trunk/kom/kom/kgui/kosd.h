/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KTOSD_H
#define KTOSD_H

#include <QWidget>
#include <QPixmap>
#include <QPaintEvent>
#include <QTimer>
#include <QTextDocument>
#include <kcore/kglobal.h>

/**
 * @author David Cuadrado <krawek@toonka.com>
*/

class K_GUI_EXPORT KOsd : public QWidget
{
    Q_OBJECT;

    private:
        KOsd(QWidget *parent = 0);

    public:
        enum Level
        {
           None = -1,
           Info,
           Warning,
           Error,
           Fatal
        };
        ~KOsd();
	
        void display(const QString & message, Level level = Info, int ms = -1);

        static KOsd *self();

    private slots:
        void animate();

    protected:
        void paintEvent(QPaintEvent * e);
        void mousePressEvent(QMouseEvent * e);

    private:
        void drawPixmap(const QBrush &background, const QBrush &foreground);

    private:
        static KOsd *s_osd;
        QPixmap m_pixmap;
        QTimer *m_timer;
        QPalette m_palette; 

        struct Animation
        {
          QTimer timer;
          Level level;
          bool on;
        } *m_animator;

        QTextDocument *m_document;
};

#endif
