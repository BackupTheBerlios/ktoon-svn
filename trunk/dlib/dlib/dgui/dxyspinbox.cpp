/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
 *   krawek@gmail.com                                                     *
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

#include "dxyspinbox.h"

#include <QHBoxLayout>
#include <QSizePolicy>
#include "dapplication.h"

#include "ddebug.h"

DXYSpinBox::DXYSpinBox(const QString &title, QWidget *parent) : QGroupBox(title, parent), m_modifyTogether(true)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	
	QGridLayout *internal = new QGridLayout;
	m_textX = new QLabel("X: ");
	internal->addWidget(m_textX, 0, 0, Qt::AlignRight);
	
	m_x = new QDoubleSpinBox;
// 	m_x->setMaximumWidth(60);
	internal->addWidget(m_x, 0, 1);
	m_textX->setBuddy(m_x);
	
	
	m_textY = new QLabel("Y: ");
	internal->addWidget(m_textY, 1, 0, Qt::AlignRight);
	
	m_y = new QDoubleSpinBox;
// 	m_y->setMaximumWidth(60);
	internal->addWidget(m_y, 1, 1);
	
	m_textY->setBuddy(m_y);
	layout->addLayout(internal);
	
	m_separator = new QPushButton;
	m_separator->setFlat(true);

	m_separator->setMaximumWidth(20);
// 	m_separator->setMinimumHeight(50);
	layout->addWidget(m_separator);
	
	connect(m_separator, SIGNAL(clicked()), this, SLOT(toggleModify()));
	
	setLayout(layout);
	
	connect(m_x, SIGNAL(valueChanged (double)), this, SLOT(updateXValue(double)));
	connect(m_y, SIGNAL(valueChanged (double)), this, SLOT(updateYValue(double)));
	
	setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
	
	m_separator->setIcon(QPixmap(THEME_DIR+"/icons/vchain.png"));
}


DXYSpinBox::~DXYSpinBox()
{
}


void DXYSpinBox::updateXValue(double v)
{
	emit valueXChanged( v );
	
	if ( m_modifyTogether && m_y->value() != v )
	{
		emit valueXYChanged( v, v);
		m_y->setValue( v );
	}
	else
	{
		emit valueXYChanged( v, m_y->value());
	}
}

void DXYSpinBox::updateYValue(double v)
{
	emit valueYChanged( v );
	if ( m_modifyTogether && m_x->value() != v )
	{
		emit valueXYChanged( v, v);
		m_x->setValue( v );
	}
	else
	{
		emit valueXYChanged( m_x->value(), v);
	}
}

void DXYSpinBox::setModifyTogether(bool enable)
{
	m_modifyTogether = enable;
	toggleModify();
}

void DXYSpinBox::toggleModify()
{
	// TODO: Change the button icon !!
	m_modifyTogether = !m_modifyTogether;
	
	if ( m_modifyTogether )
	{
		m_separator->setIcon(QPixmap(THEME_DIR+"/icons/vchain.png"));
	}
	else
	{
		m_separator->setIcon(QPixmap(THEME_DIR+"/icons/vchain_broken.png"));
	}
}

void DXYSpinBox::setSingleStep(double step)
{
	m_x->setSingleStep(step);
	m_y->setSingleStep(step);
}

void DXYSpinBox::setMinimum ( double min)
{
	m_x->setMinimum(min);
	m_y->setMinimum(min);
}

void DXYSpinBox::setMaximum ( double max)
{
	m_x->setMaximum(max);
	m_y->setMaximum(max);
}

void DXYSpinBox::setX(double x)
{
	m_x->setValue(x);
}

void DXYSpinBox::setY(double y)
{
	m_y->setValue(y);
}

double DXYSpinBox::x()
{
	return m_x->value();
}

double DXYSpinBox::y()
{
	return m_y->value();
}
