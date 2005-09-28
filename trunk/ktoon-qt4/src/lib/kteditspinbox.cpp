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
#include <QHBoxLayout>



KTEditSpinBox::KTEditSpinBox( int value, int minValue, int maxValue, int step, QString text, QWidget *parent, const char *name) :QGroupBox(parent)
{
	setObjectName(name);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setMargin(2);
	layout->setSpacing(0);
	setTitle(text);
	setLayout(layout);
	m_spin = new QSpinBox(minValue, maxValue, step, this);
	m_spin->setValue(value);
	layout->addWidget(m_spin);
	m_slider = new QSlider ( minValue,  maxValue, step, value, Qt::Horizontal  ,this);
	
	layout->addWidget(m_slider );
	setupConnects();
	setMinimumHeight(sizeHint().height());
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
