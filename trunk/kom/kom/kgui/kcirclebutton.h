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


#ifndef CCBUTTON_H
#define CCBUTTON_H
/**
 * @file kcirclebutton.h
 * Include this file if you need the class KCircleButton
 */
#include <QPushButton>
#include <QStyleOptionButton>
#include "kcore/kglobal.h"

/**
 * @short The KCircleButton class provides a circular button
 * @author David Cuadrado <krawek@gmail.com>
*/

class K_GUI_EXPORT KCircleButton : public QPushButton
{
    Q_OBJECT
    public:
        /**
         * Constructs a KCircleButton
         */
        KCircleButton(int diameter, bool animate = true, QWidget *parent = 0);
        /**
         * Destructor
         */
        ~KCircleButton();
        
        QStyleOptionButton styleOption() const;
        QSize sizeHint() const;
        
    protected:
        /**
         * Paints the button
         * @param e 
         */
        void paintEvent(QPaintEvent *e);
        
        /**
         * Init animation
         */
        void enterEvent(QEvent *);
        /**
         * End animation
         */
        void leaveEvent(QEvent *);
        
        /**
         * Creates mask of button
         */
        virtual void paintMask();
        
    private slots:
        void animate();
        
    private:
        QPixmap m_mask;
        QPixmap m_pix;
        int m_diameter : 22;
        
        class Animator;
        Animator *m_animator;
};

#endif
