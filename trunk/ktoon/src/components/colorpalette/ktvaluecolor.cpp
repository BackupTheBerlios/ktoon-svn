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
#include <QCheckBox>
#include <QDoubleSpinBox>

#include <dcore/ddebug.h>

#include <cmath>

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
	connect(d->value, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
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



void KTItemValueColor::setRange(int minimum, int maximum)
{
	d->value->setRange( minimum, maximum );
}

void KTItemValueColor::setSuffix(const QString &suffix )
{
	d->value->setSuffix(suffix);
}

struct KTValueColor::Private
{
	KTItemValueColor *valueR, *valueG, *valueB, *valueH, *valueS, *valueV; QDoubleSpinBox *valueA;
	QGridLayout *layout;
	bool ok, percent;
};

KTValueColor::KTValueColor(QWidget *parent) : QFrame(parent), d(new Private)
{
	DINIT;
	d->ok = true;
	d->layout = new QGridLayout;
	setLayout(d->layout);
// 	d->layout->setSizeConstraint(QLayout::SetFixedSize);
	d->layout->setSizeConstraint(QLayout::SetNoConstraint);
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
	connect(d->valueR, SIGNAL(editingFinished()), this, SLOT(syncValuesRgb()));
	
	d->valueG = new KTItemValueColor("G", this);
// 	d->valueG = new DEditSpinBox(0, 0, 255, 1,"G", this);
	connect(d->valueG, SIGNAL(editingFinished()), this, SLOT(syncValuesRgb()));
	
	d->valueB = new KTItemValueColor("B", this);
// 	d->valueB = new DEditSpinBox(0, 0, 255, 1,"B", this);
	connect(d->valueB, SIGNAL(editingFinished()), this, SLOT(syncValuesRgb()));
	
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
	
	QHBoxLayout *layout = new QHBoxLayout;
	
	d->valueA = new QDoubleSpinBox(/*"A",*/ this);
	d->valueA->setDecimals(0);
	d->valueA->setRange(0, 255.0);
	d->valueA->setValue(255.0);
	layout->addWidget(new QLabel("A"));
	layout->addWidget(d->valueA);
	
// 	d->valueA->setValue(255);
// 	d->valueA = new DEditSpinBox(0, 0, 255, 1,"A", this);
	connect(d->valueA, SIGNAL(editingFinished()), this, SLOT(syncValuesRgb()));
// 	editingFinished ()
	QCheckBox *show = new QCheckBox(tr("percent"));
	
	d->layout->addWidget(d->valueR, 0, 0,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addWidget(d->valueG, 1, 0,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addWidget(d->valueB, 2, 0,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addWidget(d->valueH, 0, 1,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addWidget(d->valueS, 1, 1,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addWidget(d->valueV, 2, 1,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addLayout(layout, 3, 0,Qt::AlignTop | Qt::AlignLeft);
	d->layout->addWidget(show, 3,1 ,Qt::AlignTop | Qt::AlignLeft);
	connect(show, SIGNAL(toggled( bool )), this, SLOT(setAlfaValuePercent( bool )));
	
}

void KTValueColor::setColor(const QBrush &brush)
{
	QColor color = brush.color();
	d->ok = false;
	d->valueR->setValue(color.red());
	d->valueG->setValue(color.green());
	d->valueB->setValue(color.blue());
	d->valueH->setValue(color.hue ());
	d->valueS->setValue(color.saturation());
	d->valueV->setValue(color.value ());
	
	if(d->percent)
	{
		d->valueA->setValue( ::ceil((color.alpha()/255.0)*100) );
	}
	else
	{
		d->valueA->setValue( color.alpha());
	}
	
	d->ok = true;
}


void KTValueColor::setAlfaValuePercent(bool enable)
{
	d->percent = enable;
	if(enable)
	{
		d->valueA->setDecimals(2);
		d->valueA->setValue( ((d->valueA->value()/255.0)*100) );
		d->valueA->setRange( 0.0, 100.0 );
		d->valueA->adjustSize();
		adjustSize();
	}
	else
	{
		d->valueA->setRange ( 0.0, 255.0 );
		d->valueA->setValue((d->valueA->value()*255.0)/100);
		d->valueA->setDecimals(0);
	}
}

void KTValueColor::syncValuesRgb(int)
{
	if(d->ok)
	{
		int r = d->valueR->value();
		int g = d->valueG->value();
		int b = d->valueB->value();
		int a = 0;
		if(d->percent)
		{
			a = (int) ((d->valueA->value()*255.0)/100);
		}
		else
		{
			a = (int) d->valueA->value();
		}
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
	if(d->percent)
	{
		return (int) ((d->valueA->value()*255.0)/100);
	}
	return (int)d->valueA->value();
}
