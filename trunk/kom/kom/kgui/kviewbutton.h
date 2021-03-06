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


#ifndef KVIEWBUTTON_H
#define KVIEWBUTTON_H

#include <QToolButton>
#include <QStyleOptionToolButton>
#include <kgui/kideality.h>

class KToolView;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/
class K_IDEAL_EXPORT KViewButton : public QToolButton
{
    Q_OBJECT;
    public:
        KViewButton(Qt::ToolBarArea area, KToolView *toolView, QWidget * parent = 0);
        KViewButton(KToolView *toolView, QWidget *parent = 0);
        ~KViewButton();

        void setArea(Qt::ToolBarArea area);
        Qt::ToolBarArea area() const;

        QSize sizeHint() const;
        void fade();
        bool isSensible() const;
        bool blending() const;
        void setActivated(bool flag);

        KToolView *toolView() const;
        void setup();

    public slots:
        void setSensible(bool s);
        void setBlending(bool e);

    private:
        QMenu *createMenu();
        QStyleOptionToolButton styleOption() const;

    protected:
        virtual void paintEvent(QPaintEvent *e);
        virtual void mousePressEvent(QMouseEvent *e);
        virtual void enterEvent(QEvent*);
        virtual void leaveEvent(QEvent*);
		
    public slots:
        void setOnlyText();
        void setOnlyIcon();
        void toggleView();

    private slots:
        void animate();
        void toggleSensibility();

    signals:
        void helpIsOpen();

    private:
        Qt::ToolBarArea m_area;

        class Animator;
        Animator *m_animator;

        bool m_isSensible;
        bool m_blending;

        QPalette m_palette;

        KToolView *m_toolView;
};

#endif
