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

#include "ktcolorvalue.h"
#include <QHBoxLayout>
#include <QLabel>
#include <QCheckBox>
#include <QDoubleSpinBox>
#include <QGridLayout>
#include <QSpinBox>
#include <QLineEdit>

#include <dcore/ddebug.h>
#include <dgui/ddoublecombobox.h>

#include <cmath>

struct KTItemColorValue::Private
{
	QSpinBox *value;
	~Private()
	{
		delete value;
	};
};

KTItemColorValue::KTItemColorValue( const QString &text, QWidget *parent ) :QFrame(parent), d(new Private)
{
	QHBoxLayout * layout = new QHBoxLayout;
	layout->setSpacing(0);
	layout->setMargin(0);
	
	setLayout(layout);
	QLabel *labelText = new QLabel( text, this);
	labelText->setSizePolicy ( QSizePolicy::Fixed,  QSizePolicy::Fixed);
	d->value = new QSpinBox(this);
	d->value->setMaximum ( 255 );
	d->value->setMinimum ( 0 );
	d->value->setSizePolicy ( QSizePolicy::Fixed,  QSizePolicy::Fixed);
	connect(d->value, SIGNAL(valueChanged( int)) , this, SIGNAL(valueChanged( int)));
	connect(d->value, SIGNAL(valueChanged( const QString &)), this, SIGNAL(valueChanged( const QString &)));
	connect(d->value, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
	layout->addWidget(labelText);
	layout->addWidget(d->value);
}

KTItemColorValue::~KTItemColorValue()
{
	delete d;
}

void KTItemColorValue::setValue( int val)
{
	d->value->setValue(val);
}

int  KTItemColorValue::value()
{
	return d->value->value();
}

void KTItemColorValue::setMax(int max)
{
	d->value->setMaximum( max );
}

void KTItemColorValue::setRange(int minimum, int maximum)
{
	d->value->setRange( minimum, maximum );
}

void KTItemColorValue::setSuffix(const QString &suffix )
{
	d->value->setSuffix(suffix);
}

struct KTColorValue::Private
{
	KTItemColorValue *valueR, *valueG, *valueB, *valueH, *valueS, *valueV; 
	DDoubleComboBox *valueA;
// 	QGridLayout *layout;
	bool ok, percent;
};

KTColorValue::KTColorValue(QWidget *parent) : QFrame(parent), d(new Private)
{
	DINIT;
	d->ok = true;
	
	setLayout(new QVBoxLayout());
// 	d->layout = new QGridLayout;
// 	setLayout(d->layout);
	
	setupForm();
}


KTColorValue::~KTColorValue()
{
	DEND;
}

void KTColorValue::setupForm()
{
	QGridLayout *gridLayout = new QGridLayout;
	
	d->valueR = new KTItemColorValue("R");
	connect(d->valueR, SIGNAL(editingFinished()), this, SLOT(syncValuesRgb()));
	
	d->valueG = new KTItemColorValue("G", this);
	connect(d->valueG, SIGNAL(editingFinished()), this, SLOT(syncValuesRgb()));
	
	d->valueB = new KTItemColorValue("B", this);
	connect(d->valueB, SIGNAL(editingFinished()), this, SLOT(syncValuesRgb()));
	
	d->valueH = new KTItemColorValue("H", this);
	d->valueH->setMax(359);
	connect(d->valueH, SIGNAL(valueChanged(int)), this, SIGNAL(hueChanged(int)));
	
	d->valueS = new KTItemColorValue("S", this);
	connect(d->valueS, SIGNAL(valueChanged(int)), this, SIGNAL(saturationChanged( int)));
	
	d->valueV = new KTItemColorValue("V", this);
	connect(d->valueV, SIGNAL(valueChanged(int)), this, SIGNAL(valueChanged( int)));
	
	QHBoxLayout *boxLayout = new QHBoxLayout;
	boxLayout->setSpacing(1);
	
	d->valueA = new DDoubleComboBox(0, 255, this);
	QLabel * label = new QLabel("A");
	label->setSizePolicy ( QSizePolicy::Fixed,  QSizePolicy::Fixed);
	boxLayout->addWidget(label);
	
	boxLayout->addWidget(d->valueA);
	
	for(int i = 0; i < 255; i += 25)
	{
		d->valueA->addValue(i);
	}
	
	connect(d->valueA, SIGNAL(editingFinished()), this, SLOT(syncValuesRgb()));
	QCheckBox *show = new QCheckBox(tr("percent"));
	
	gridLayout->addWidget(d->valueR, 0, 0,Qt::AlignTop | Qt::AlignLeft);
	gridLayout->addWidget(d->valueG, 1, 0,Qt::AlignTop | Qt::AlignLeft);
	gridLayout->addWidget(d->valueB, 2, 0,Qt::AlignTop | Qt::AlignLeft);
	gridLayout->addWidget(d->valueH, 0, 1,Qt::AlignTop | Qt::AlignLeft);
	gridLayout->addWidget(d->valueS, 1, 1,Qt::AlignTop | Qt::AlignLeft);
	gridLayout->addWidget(d->valueV, 2, 1,Qt::AlignTop | Qt::AlignLeft);
	
	
	boxLayout->addWidget(show);
	
	static_cast<QHBoxLayout*>(layout())->addLayout(gridLayout);
	static_cast<QHBoxLayout*>(layout())->addLayout(boxLayout);
	
	connect(show, SIGNAL(toggled( bool )), this, SLOT(setAlfaValuePercent( bool )));
	
}

void KTColorValue::setColor(const QBrush &brush)
{
	QColor color = brush.color();
	d->ok = false;
	d->valueR->setValue(color.red());
	d->valueG->setValue(color.green());
	d->valueB->setValue(color.blue());
	d->valueH->setValue(color.hue ());
	d->valueS->setValue(color.saturation());
	d->valueV->setValue(color.value ());
	
	d->valueA->setValue( d->valueA->currentIndex(), color.alpha());
	
	d->ok = true;
}


void KTColorValue::setAlfaValuePercent(bool enable)
{
	d->valueA->setShowAsPercent(enable);
	d->percent = enable;
}

void KTColorValue::syncValuesRgb(int)
{
	if(d->ok)
	{
		int r = d->valueR->value();
		int g = d->valueG->value();
		int b = d->valueB->value();
		int a = (int) ::ceil(d->valueA->value());
		
		QColor tmp = QColor::fromRgb(r,g,b,a);
		d->valueH->setValue( tmp.hue ());
		d->valueS->setValue( tmp.saturation());
		d->valueV->setValue( tmp.value ());
		emit brushChanged(QColor::fromRgb(r,g,b,a));
	}
}

int KTColorValue::hue()
{
	return d->valueH->value();
}

int KTColorValue::saturation()
{
	return d->valueS->value();
}

int KTColorValue::value()
{
	return d->valueV->value();
}

int KTColorValue::alpha()
{
	return (int) ::ceil(d->valueA->value());
}
