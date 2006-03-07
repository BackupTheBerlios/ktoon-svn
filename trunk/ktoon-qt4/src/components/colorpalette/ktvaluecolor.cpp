/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
 *   kuadrosx@toonka.com                                                   *
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
#include "ktvaluecolor.h"
#include <QHBoxLayout>
#include <QLabel>
#include "ddebug.h"

KTItemValueColor::KTItemValueColor( const QString &text, QWidget *parent ) :QFrame(parent)
{
	DINIT;
	QHBoxLayout * m_layout = new QHBoxLayout;
	m_layout->setSpacing(0);
	m_layout->setMargin(0);
	setLayout(m_layout);
	QLabel *m_text = new QLabel( text, this);
	m_text->setSizePolicy ( QSizePolicy::Fixed,  QSizePolicy::Fixed);
	m_value = new QSpinBox(this);
	m_value->setMaximum ( 255 );
	m_value->setMinimum ( 0 );
	m_value->setSizePolicy ( QSizePolicy::Fixed,  QSizePolicy::Fixed);
	connect(m_value, SIGNAL(valueChanged( int)) , this, SIGNAL(valueChanged( int)));
	connect(m_value, SIGNAL(valueChanged( const QString &)) , this, SIGNAL(valueChanged( const QString &)));
	layout()->addWidget(m_text);
	layout()->addWidget(m_value);
}

KTItemValueColor::~KTItemValueColor()
{
	delete m_value;
	DEND;
}

void KTItemValueColor::setValue( int val)
{
	m_value->setValue(val);
}

int  KTItemValueColor::value()
{
	return m_value->value();
}

void KTItemValueColor::setMax(int max)
{
	m_value->setMaximum( max );
}


KTValueColor::KTValueColor(QWidget *parent) : QFrame(parent), ok(true)
{
	DINIT;
	m_layout = new QGridLayout;
	m_layout->setSpacing(2);
	m_layout->setMargin(0);
	setLayout(m_layout);
	setupForm();
}


KTValueColor::~KTValueColor()
{
	DEND;
}

void KTValueColor::setupForm()
{
// 	m_valueR = new KTItemValueColor("R", this);
	m_valueR = new DEditSpinBox(0, 0, 255, 1, "R", this);
	connect(m_valueR, SIGNAL(valueChanged(int)), this, SLOT(syncValuesRgb(int)));
	
// 	m_valueG = new KTItemValueColor("G", this);
	m_valueG = new DEditSpinBox(0, 0, 255, 1,"G", this);
	connect(m_valueG, SIGNAL(valueChanged(int)), this, SLOT(syncValuesRgb(int)));
	
// 	m_valueB = new KTItemValueColor("B", this);
	m_valueB = new DEditSpinBox(0, 0, 255, 1,"B", this);
	connect(m_valueB, SIGNAL(valueChanged(int)), this, SLOT(syncValuesRgb(int)));
	
// 	m_valueH = new KTItemValueColor("H", this);
	m_valueH = new DEditSpinBox(0, 0, 359, 1,"H", this);
	
// 	m_valueH->setMax(359);
	connect(m_valueH, SIGNAL(valueChanged(int)), this, SIGNAL(hueChanged(int)));
	
// 	m_valueS = new KTItemValueColor("S", this);
	m_valueS = new DEditSpinBox(0, 0, 255, 1,"S", this);
	connect(m_valueS, SIGNAL(valueChanged(int)), this, SIGNAL(saturationChanged( int)));
	
// 	m_valueV = new KTItemValueColor("V", this);
	m_valueV = new DEditSpinBox(0, 0, 255, 1,"V", this);
	connect(m_valueV, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged( int)));
	
// 	m_valueA = new KTItemValueColor("A", this);
	m_valueA = new DEditSpinBox(0, 0, 255, 1,"A", this);
	connect(m_valueA, SIGNAL(valueChanged(int)), this, SLOT(syncValuesRgb( int)));
	
// 	m_layout->setSizeConstraint(QLayout::SetFixedSize);
	m_layout->addWidget(m_valueR, 0, 0,Qt::AlignTop | Qt::AlignLeft);
	m_layout->addWidget(m_valueG, 1, 0,Qt::AlignTop | Qt::AlignLeft);
	m_layout->addWidget(m_valueB, 2, 0,Qt::AlignTop | Qt::AlignLeft);
	m_layout->addWidget(m_valueH, 0, 1,Qt::AlignTop | Qt::AlignLeft);
	m_layout->addWidget(m_valueS, 1, 1,Qt::AlignTop | Qt::AlignLeft);
	m_layout->addWidget(m_valueV, 2, 1,Qt::AlignTop | Qt::AlignLeft);
	m_layout->addWidget(m_valueA, 3, 0,Qt::AlignTop | Qt::AlignLeft);
}

void KTValueColor::setColor(const QBrush &brush)
{
	QColor color = brush.color();
	ok = false;
	m_valueR->setValue( color.red());
	m_valueG->setValue( color.green());
	m_valueB->setValue( color.blue());
	m_valueH->setValue( color.hue ());
	m_valueS->setValue( color.saturation());
	m_valueV->setValue( color.value ());
	m_valueA->setValue( color.alpha ());
	ok = true;
}

void KTValueColor::syncValuesRgb(int)
{
	if(ok)
	{
		int r = m_valueR->value();
		int g = m_valueG->value();
		int b = m_valueB->value();
		int a = m_valueA->value();
		QColor tmp = QColor::fromRgb(r,g,b,a);
		m_valueH->setValue( tmp.hue ());
		m_valueS->setValue( tmp.saturation());
		m_valueV->setValue( tmp.value ());
		emit colorChanged(QColor::fromRgb(r,g,b,a)) ;
	}
}

int KTValueColor::hue()
{
	return m_valueH->value();
}

int KTValueColor::saturation()
{
	return m_valueS->value();
}

int KTValueColor::value()
{
	return m_valueV->value();
}

int KTValueColor::alpha()
{
	return m_valueA->value();
}

