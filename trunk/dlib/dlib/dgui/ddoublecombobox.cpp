/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#include "ddoublecombobox.h"

#include <QDoubleValidator>
#include <QLineEdit>

#include <cmath>

#define PERCENT(v) (v * 100.0) / maximum()
#define VALUE(p) (p * maximum()) / 100.0

struct DDoubleComboBox::Private
{
	bool showAsPercent;
	QDoubleValidator *validator;
	QLineEdit *editor;
};

DDoubleComboBox::DDoubleComboBox(double min, double max, QWidget *parent)
 : QComboBox(parent), d(new Private)
{
	
	d->validator = new QDoubleValidator(this);
	d->editor = new QLineEdit;
	d->editor->setValidator(d->validator);
	setLineEdit(d->editor);
	
	setValidator(d->validator);
	setMinimum(min);
	setMaximum(max);

	setDuplicatesEnabled(false);
	setInsertPolicy(QComboBox::InsertAlphabetically);
	
	connect(this, SIGNAL(activated( int )), this, SLOT(emitActivated(int)));
	connect(this, SIGNAL(highlighted( int )), this, SLOT(emitHighlighted(int)));
	connect(this, SIGNAL(currentIndexChanged( int )), this, SLOT(emitCurrentIndexChanged(int)));
	
	connect(d->editor, SIGNAL(editingFinished()), this, SIGNAL(editingFinished()));
	connect(d->editor, SIGNAL(returnPressed()), this, SIGNAL(editingFinished()));
	
	
	d->showAsPercent = false;
	
	setDecimals(2);
}


DDoubleComboBox::~DDoubleComboBox()
{
	delete d;
}

void DDoubleComboBox::setShowAsPercent(bool p)
{
	if( d->showAsPercent == p ) return;
	
	d->showAsPercent = p;
	
	for(int index = 0; index < count(); index++ )
	{
		if( d->showAsPercent )
		{
			setItemText(index, QString::number(PERCENT(itemText(index).toDouble())));
		}
		else
		{
			double value = VALUE(itemText(index).toDouble());
			setItemText(index, QString::number(value));
		}
	}
}

bool DDoubleComboBox::showAsPercent() const
{
	return d->showAsPercent;
}

void DDoubleComboBox::setDecimals(int n)
{
	d->validator->setDecimals(n);
}

int DDoubleComboBox::decimals() const
{
	return d->validator->decimals();
}

void DDoubleComboBox::setMaximum(double max)
{
	d->validator->setTop(max);
}

void DDoubleComboBox::setMinimum(double min)
{
	d->validator->setBottom(min);
}

double DDoubleComboBox::maximum() const
{
	return d->validator->top();
}

double DDoubleComboBox::minimum() const
{
	return d->validator->bottom();
}

void DDoubleComboBox::addValue(double v)
{
	if(minimum() < v && v < maximum() )
	{
		if(d->showAsPercent)
		{
			addItem(QString::number(PERCENT(v)));
		}
		else
		{
			addItem(QString::number(v));
		}
	}
}

void DDoubleComboBox::addPercent(double p)
{
	if(p >= 0 && p <= 100)
	{
		if(d->showAsPercent )
		{
			addItem(QString::number(p));
		}
		else
		{
			addItem(QString::number(VALUE(p)));
		}
	}
}

void DDoubleComboBox::emitHighlighted(int index)
{
	emit highlighted( itemText(index).toDouble());
}

void DDoubleComboBox::emitActivated(int index)
{
	emit activated( itemText(index).toDouble());
}

void DDoubleComboBox::emitCurrentIndexChanged(int index)
{
	emit currentIndexChanged( itemText(index).toDouble());
}

double DDoubleComboBox::value()
{
	if( d->showAsPercent )
	{
		return (VALUE(currentText().toDouble()));
	}
	
	return currentText().toDouble();
}

void DDoubleComboBox::setValue(int index, double v)
{
	if( d->showAsPercent )
	{
		setItemText(index, QString::number(PERCENT(v)));
	}
	else
	{
		setItemText(index, QString::number(v));
	}
}

void DDoubleComboBox::setPercent(int index, double p)
{
	if( d->showAsPercent )
	{
		setItemText(index, QString::number(p));
	}
	else
	{
		setItemText(index, QString::number(VALUE(p)));
	}
}

double DDoubleComboBox::percent()
{
	if( d->showAsPercent )
	{
		return currentText().toDouble();
	}
	
	return PERCENT(currentText().toDouble());
}
