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
#include "deditspinbox.h"
#include <QHBoxLayout>



DEditSpinBox::DEditSpinBox( int value, int minValue, int maxValue, int step, QString text, QWidget *parent, const char *name) :QGroupBox(parent)
{
	setObjectName(name);
	QHBoxLayout *layout = new QHBoxLayout;
	layout->setMargin(2);
	layout->setSpacing(0);
	setTitle(text);
	setLayout(layout);
	m_spin = new QSpinBox(this);
	m_spin->setMinimum(minValue);
	m_spin->setMaximum(maxValue);
	m_spin->setSingleStep ( step );
	m_spin->setValue(value);
	layout->addWidget(m_spin);
	m_slider = new QSlider (Qt::Horizontal, this);
	m_slider->setMinimum(minValue);
	m_slider->setMaximum(maxValue);
	m_slider->setSingleStep ( step );
	
	layout->addWidget(m_slider );
	setupConnects();
	setMinimumHeight(sizeHint().height());
}

DEditSpinBox::~DEditSpinBox()
{
}

void DEditSpinBox::setupConnects()
{
	QObject::connect(m_spin, SIGNAL(valueChanged(int)),
			 m_slider, SLOT(setValue(int)));
	QObject::connect(m_slider, SIGNAL(valueChanged(int)),
			 m_spin, SLOT(setValue(int)));
	QObject::connect(m_slider,  SIGNAL(valueChanged(int)),this, SIGNAL(valueChanged(int)));
	QObject::connect(m_spin,  SIGNAL(valueChanged(int)),this, SIGNAL(valueChanged(int)));
}

void DEditSpinBox::setRange(int min, int max)
{
	m_spin->setMinimum ( min );
	m_spin->setMaximum ( max );
	m_slider->setMinimum ( min );
	m_slider->setMaximum ( max );
}

void DEditSpinBox::setValue(int value)
{
	m_spin->setValue(value);
	m_slider->setValue(value);
}

int DEditSpinBox::value()
{
	return m_slider->value();
}
