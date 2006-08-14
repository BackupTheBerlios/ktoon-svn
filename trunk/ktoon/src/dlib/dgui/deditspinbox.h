/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@gmail.com   *
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
#ifndef DEDITSPINBOX_H
#define DEDITSPINBOX_H

#include <QGroupBox>
#include <QSpinBox>
#include <QSlider>

#include <dcore/dglobal.h>

/**
 * @if english
 * This class represents a spinbox editable
 * @elseif spanish
 * Esta clase representa un spinbox editable
 * @endif
 * @author Jorge Cuadrado
*/
class D_GUI_EXPORT DEditSpinBox : public QGroupBox
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
		DEditSpinBox(int value, int valueMin, int valueMax, int step, QString text, QWidget *parent = 0, const char *name = 0);
		~DEditSpinBox();
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
		void valueChanged( int );
};

#endif
