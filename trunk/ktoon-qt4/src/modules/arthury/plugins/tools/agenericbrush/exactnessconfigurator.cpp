/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                     *
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
#include "exactnessconfigurator.h"
#include <QBoxLayout>
ExactnessConfigurator::ExactnessConfigurator(QWidget *parent) :QWidget(parent)
{
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::LeftToRight, this);
	QLabel *label = new QLabel(tr("Exactness"));
	layout->addWidget(label);
	m_exactness = new QDoubleSpinBox();
	m_exactness->setDecimals ( 2 );
	m_exactness->setSingleStep ( 0.5 );
	m_exactness->setMaximum ( 100 );
	layout->addWidget(m_exactness);
}


ExactnessConfigurator::~ExactnessConfigurator()
{
}

double ExactnessConfigurator::exactness()
{
	m_exactness->value();
}

