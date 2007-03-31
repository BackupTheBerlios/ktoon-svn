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
#include <dcore/ddebug.h>

struct KTItemValueColor::Private
{
	QSpinBox *value;
	~Private()
	{
		delete value;
	};
};

KTItemValueColor::KTItemValueColor( const QString &text, QWidget *parent ) :QFrame(parent), d(new Private)
{
	QHBoxLayout * layout = new QHBoxLayout;
	layout->setSpacing(0);
	layout->setMargin(0);
	
	setLayout(layout);
	QLabel *labelText = new QLabel( text, this);
// 	d->text->setSizePolicy ( QSizePolicy::Fixed,  QSizePolicy::Fixed);
	d->value = new QSpinBox(this);
	d->value->setMaximum ( 255 );
	d->value->setMinimum ( 0 );
// 	d->value->setSizePolicy ( QSizePolicy::Fixed,  QSizePolicy::Fixed);
	connect(d->value, SIGNAL(valueChanged( int)) , this, SIGNAL(valueChanged( int)));
	connect(d->value, SIGNAL(valueChanged( const QString &)) , this, SIGNAL(valueChanged( const QString &)));
	layout->addWidget(labelText);
	layout->addWidget(d->value);
}

KTItemValueColor::~KTItemValueColor()
{
	delete d;
}

void KTItemValueColor::setValue( int val)
{
	d->value->setValue(val);
}

int  KTItemValueColor::value()
{
	return d->value->value();
}

void KTItemValueColor::setMax(int max)
{
	d->value->setMaximum( max );
}

struct KTValueColor::Private
{
	KTItemValueColor *valueR, *valueG, *valueB, *valueH, *valueS, *valueV, *valueA;
	QGridLayout *layout;
	bool ok;
};

KTValueColor::KTValueColor(QWidget *parent) : QFrame(parent), d(new Private)
{
	DINIT;
	d->ok = true;
	d->layout = new QGridLayout;
	d->layout->setSpacing(5);
	d->layout->setMargin(10);
	setLayout(d->layout);
	setupForm();
}


KTValueColor::~KTValueColor()
{
	DEND;
}

void KTValueColor::setupForm()
{
	d->valueR = new KTItemValueColor("R", this);
// 	d->valueR = new DEditSpinBox(0, 0, 255, 1, "R", this);
	connect(d->valueR, SIGNAL(valueChanged(int)), this, SLOT(syncValuesRgb(int)));
	
	d->valueG = new KTItemValueColor("G", this);
// 	d->valueG = new DEditSpinBox(0, 0, 255, 1,"G", this);
	connect(d->valueG, SIGNAL(valueChanged(int)), this, SLOT(syncValuesRgb(int)));
	
	d->valueB = new KTItemValueColor("B", this);
// 	d->valueB = new DEditSpinBox(0, 0, 255, 1,"B", this);
	connect(d->valueB, SIGNAL(valueChanged(int)), this, SLOT(syncValuesRgb(int)));
	
	d->valueH = new KTItemValueColor("H", this);
// 	d->valueH = new DEditSpinBox(0, 0, 359, 1,"H", this);
	
	d->valueH->setMax(359);
	connect(d->valueH, SIGNAL(valueChanged(int)), this, SIGNAL(hueChanged(int)));
	
	d->valueS = new KTItemValueColor("S", this);
// 	d->valueS = new DEditSpinBox(0, 0, 255, 1,"S", this);
	connect(d->valueS, SIGNAL(valueChanged(int)), this, SIGNAL(saturationChanged( int)));
	
	d->valueV = new KTItemValueColor("V", this);
// 	d->valueV = new DEditSpinBox(0, 0, 255, 1,"V", this);
	connect(d->valueV, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged( int)));
	
	d->valueA = new KTItemValueColor("A", this);
// 	d->valueA = new DEditSpinBox(0, 0, 255, 1,"A", this);
	connect(d->valueA, SIGNAL(valueChanged(int)), this, SLOT(syncValuesRgb( int)));
	
// 	d->layout->setSizeConstraint(QLayout::SetFixedSize);
	d->layout->addWidget(d->valueR, 0, 0,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addWidget(d->valueG, 1, 0,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addWidget(d->valueB, 2, 0,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addWidget(d->valueH, 0, 1,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addWidget(d->valueS, 1, 1,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addWidget(d->valueV, 2, 1,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addWidget(d->valueA, 3, 0,Qt::AlignTop | Qt::AlignLeft);
}

void KTValueColor::setColor(const QBrush &brush)
{
	QColor color = brush.color();
	d->ok = false;
	d->valueR->setValue( color.red());
	d->valueG->setValue( color.green());
	d->valueB->setValue( color.blue());
	d->valueH->setValue( color.hue ());
	d->valueS->setValue( color.saturation());
	d->valueV->setValue( color.value ());
	d->valueA->setValue( color.alpha ());
	d->ok = true;
}

void KTValueColor::syncValuesRgb(int)
{
	if(d->ok)
	{
		int r = d->valueR->value();
		int g = d->valueG->value();
		int b = d->valueB->value();
		int a = d->valueA->value();
		
		QColor tmp = QColor::fromRgb(r,g,b,a);
		d->valueH->setValue( tmp.hue ());
		d->valueS->setValue( tmp.saturation());
		d->valueV->setValue( tmp.value ());
		emit brushChanged(QColor::fromRgb(r,g,b,a)) ;
	}
}

int KTValueColor::hue()
{
	return d->valueH->value();
}

int KTValueColor::saturation()
{
	return d->valueS->value();
}

int KTValueColor::value()
{
	return d->valueV->value();
}

int KTValueColor::alpha()
{
	return d->valueA->value();
}
