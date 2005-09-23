/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado   *
 *   kuadrosx@toonka.com   *
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
#include "kteditspinbox.h"
//Added by qt3to4:
#include <QLabel>

KTEditSpinBox::KTEditSpinBox(QString text, QWidget *parent, const char *name ) :Q3VBox(parent, name)
{
	
	m_label = new QLabel(text, this);
	Q3HBox *hbox = new Q3HBox(this);
	hbox->setSpacing ( 10 );
	m_spin = new QSpinBox(hbox);
	m_slider = new QSlider(hbox);
	
	setupConnects();
}

KTEditSpinBox::KTEditSpinBox( int value, int minValue, int maxValue, int step, QString text, QWidget *parent, const char *name) :Q3VBox(parent, name)
{
	m_label = new QLabel(text, this);
	Q3HBox *hbox = new Q3HBox(this);
	m_spin = new QSpinBox(minValue, maxValue, step, hbox);
	m_spin->setValue(value);
	m_slider = new QSlider ( minValue,  maxValue, step, value, Qt::Horizontal  ,hbox);
	setupConnects();
}

KTEditSpinBox::~KTEditSpinBox()
{
}

void KTEditSpinBox::setupConnects()
{
	QObject::connect(m_spin, SIGNAL(valueChanged(int)),
			 m_slider, SLOT(setValue(int)));
	QObject::connect(m_slider, SIGNAL(valueChanged(int)),
			 m_spin, SLOT(setValue(int)));
	QObject::connect(m_slider,  SIGNAL(valueChanged(int)),this, SIGNAL(valueChanged(int)));
	QObject::connect(m_spin,  SIGNAL(valueChanged(int)),this, SIGNAL(valueChanged(int)));
}

void KTEditSpinBox::setRange(int min, int max)
{
	m_spin->setMinValue ( min );
	m_spin->setMaxValue ( max );
	m_slider->setMinValue ( min );
	m_slider->setMaxValue ( max );
}

void KTEditSpinBox::setValue(int value)
{
	m_spin->setValue(value);
	m_slider->setValue(value);
}

int KTEditSpinBox::value()
{
	return m_slider->value();
}
