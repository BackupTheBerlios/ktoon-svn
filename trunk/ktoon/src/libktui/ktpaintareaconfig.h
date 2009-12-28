/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

#ifndef KTPAINTAREACONFIG_H
#define KTPAINTAREACONFIG_H

#include <QWidget>
#include <QSpinBox>

class KColorButton;

/**
 * @author David Cuadrado <krawek@toonka.com>
*/
class KTPaintAreaConfig : public QWidget
{
    Q_OBJECT;
    public:
        KTPaintAreaConfig(QWidget *parent = 0);
        ~KTPaintAreaConfig();
        
        QColor gridColor() const;
        QColor backgroundColor() const;
        QColor onionSkinColor() const;
        QColor onionSkinBackground() const;
        int gridSeparation() const;
        
    private:
        void setupPage();
        
    private:
        KColorButton *m_gridColor,*m_backgroundColor,*m_onionSkinColor,*m_onionSkinBackground;
        QSpinBox *m_gridSeparation;
};

#endif
