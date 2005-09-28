/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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
#ifndef KTEDITSPINBOX_H
#define KTEDITSPINBOX_H

#include <QGroupBox>
#include <QSpinBox>
#include <QSlider>

/**
 * @author Jorge Cuadrado
*/
class KTEditSpinBox : public QGroupBox
{
	Q_OBJECT
	public:
		KTEditSpinBox(int value, int valueMin, int valueMax, int step, QString text, QWidget *parent = 0, const char *name = 0);
		~KTEditSpinBox();
		void setRange(int min, int max);
		int value();
		
	private:
		QSlider *m_slider;
		QSpinBox *m_spin;
		void setupConnects();
	
	public slots:
		void setValue(int value);
		
	signals:
		void valueChanged( int );
};

#endif
