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

#ifndef KEDITSPINBOX_H
#define KEDITSPINBOX_H

#include <QGroupBox>
#include <QSpinBox>
#include <QSlider>

#include <kcore/kglobal.h>

/**
 * @if english
 * This class represents a spinbox editable
 * @elseif spanish
 * Esta clase representa un spinbox editable
 * @endif
 * @author Jorge Cuadrado
*/
class K_GUI_EXPORT KEditSpinBox : public QGroupBox
{
    Q_OBJECT
    public:
        /**
         * @if english
         * Default constructor
         * @elseif spanish
         * Constructor por defecto
         * @endif
         * @param value 
         * @param valueMin 
         * @param valueMax 
         * @param step 
         * @param text 
         * @param parent 
         * @param name 
         * @return 
         */
        KEditSpinBox(int value, int valueMin, int valueMax, int step, QString text, QWidget *parent = 0, const char *name = 0);
        ~KEditSpinBox();
        /**
         * @if english
         * Set a range
         * @elseif spanish
         * Pone un rango
         * @endif
         * @param min 
         * @param max 
         */
        void setRange(int min, int max);
        /**
         * @if english
         * Returns the current value
         * @elseif spanish
         * Retorna el valor actual
         * @endif
         * @return 
         */
        int value();
        
    private:
        QSlider *m_slider;
        QSpinBox *m_spin;
        void setupConnects();
    
    public slots:
        /**
         * @if english
         * Sets the actual value
         * @elseif spanish
         * Pone el valor actual
         * @endif
         * @param value 
         */
        void setValue(int value);
        
    signals:
        /**
         * @if english
         * This signal is emitted when value is changed
         * @elseif spanish
         * Este signal es emitido cuando el valor cambia
         * @endif
         * @param  
         */
        void valueChanged(int);
};

#endif
