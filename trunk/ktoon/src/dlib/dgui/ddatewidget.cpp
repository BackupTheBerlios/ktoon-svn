/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@gmail.com                                                       *
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

#include "ddatewidget.h"
#include <QSpinBox>
#include <QComboBox>
#include <QHBoxLayout>

DDateWidget::DDateWidget(QWidget *parent) : QWidget(parent)
{
	init(QDate::currentDate());
}

DDateWidget::DDateWidget(const QDate &date, QWidget *parent) : QWidget(parent)
{
	init(date);
}

void DDateWidget::init(const QDate &date)
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	
	m_day = new QSpinBox;
	m_day->setMinimum(0);
	
	m_year = new QSpinBox;
	m_year->setMinimum(0);
	m_year->setMaximum(3000);
	
	m_mounth = new QComboBox;
	
	for(int i = 1; i <= 12; i++)
	{
		QDate tmp(1,i, 1);
		
		m_mounth->addItem( tmp.toString("MMMM"), i );
	}
	
	layout->addWidget( m_day );
	layout->addWidget(m_mounth);
	layout->addWidget(m_year);
	
	setDate(date);
	
	connect(m_day, SIGNAL(valueChanged (int)), this, SLOT(updateDateValues(int) ));
	connect(m_year, SIGNAL(valueChanged(int)), this, SLOT(updateDateValues(int)));
	connect(m_mounth, SIGNAL(activated( int )), this, SLOT(updateDateValues(int))); 
}

DDateWidget::~DDateWidget()
{
}

void DDateWidget::setDate(const QDate &date)
{
	m_date = date;
	
	m_day->blockSignals( true );
	m_year->blockSignals( true );
	m_mounth->blockSignals( true );
	
	m_day->setMaximum(date.daysInMonth());
	
	m_day->setValue( date.day());
	m_year->setValue( date.year());
	
	m_mounth->setCurrentIndex( date.month()-1 );
	
	m_day->blockSignals( false );
	m_year->blockSignals( false );
	m_mounth->blockSignals( false );
}

QDate DDateWidget::date() const
{
	return m_date;
}

void DDateWidget::updateDateValues(int)
{
	QDate newDate;
	
	int year = m_year->value();
	
	int day = m_day->value();
	
	int mounth = m_mounth->currentIndex()+1;
	
	
	newDate.setYMD(year, mounth, day);
	
	setDate(newDate);
}

