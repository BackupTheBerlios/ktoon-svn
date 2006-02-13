/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "gradientconfigurator.h"
#include "ktgradientmanager.h"

#include <QColorDialog>

GradientConfigurator::GradientConfigurator(QWidget *parent) : QWidget(parent)
{
	QVBoxLayout *mainLayout = new QVBoxLayout(this);
	
	m_gradientCreator = new KTGradientManager;
	mainLayout->addWidget(m_gradientCreator);
	
	connect(m_gradientCreator, SIGNAL(controlArrowAdded()), this, SLOT(chooseColor()));
	
}


GradientConfigurator::~GradientConfigurator()
{
}

const QGradient *GradientConfigurator::gradient() const
{
	return m_gradientCreator->currentGradient().gradient();
}

void GradientConfigurator::chooseColor()
{
	QRgb rgb = QColorDialog::getRgba ( qRgba(0,0,0,255), 0, this );

	m_gradientCreator->setCurrentColor(QColor::fromRgba(rgb));
}


