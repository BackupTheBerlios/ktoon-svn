/***************************************************************************
 *   Copyright (C) 2006 by Jorge Cuadrado                                  *
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

#include "exactnessconfigurator.h"
#include <QBoxLayout>

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QHeaderView>
#include <QPushButton>

#include <kcore/kglobal.h>
#include <kgui/kimagebutton.h>
#include <kcore/kdebug.h>
#include <kcore/kconfig.h>

ExactnessConfigurator::ExactnessConfigurator(QWidget *parent) :QWidget(parent)
{
	DINIT;
	QBoxLayout *mainLayout = new QBoxLayout(QBoxLayout::TopToBottom, this);
	
	QBoxLayout *layout = new QBoxLayout(QBoxLayout::TopToBottom);
	QLabel *label = new QLabel(tr("Smoothness"));
	layout->addWidget(label);
	m_exactness = new QDoubleSpinBox();
	
	m_exactness->setValue(0.1);
	m_exactness->setDecimals ( 2 );
	m_exactness->setSingleStep ( 0.1 );
	m_exactness->setMaximum ( 100 );
	layout->addWidget(m_exactness);
	
	mainLayout->addLayout(layout);
	
	m_table = new QTableWidget(5,3);
	connect(m_table, SIGNAL(itemClicked ( QTableWidgetItem *)), this, SLOT(updateValueFromItem(QTableWidgetItem *)));
	
	m_table->setSelectionMode ( QAbstractItemView::SingleSelection);
	
	m_table->horizontalHeader()->hide();
	m_table->verticalHeader()->hide();
	
	for (int row = 0; row < m_table->rowCount(); row++)
	{
		m_table->verticalHeader()->resizeSection(row, 15);
		
		for(int col = 0; col < m_table->columnCount(); col++)
		{
			QTableWidgetItem *newItem = new QTableWidgetItem;
			m_table->setItem(row, col, newItem);
		}
	}
	
	m_table->setItemSelected(m_table->item(0,0), true); 
	
	m_table->setHorizontalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	m_table->setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOff);
	
	m_table->setMaximumHeight(15*m_table->rowCount()+3);
	
	m_table->horizontalHeader()->setResizeMode( QHeaderView::Custom );
	
	mainLayout->addWidget(m_table);
	
	mainLayout->addStretch(2);
	
	
	QBoxLayout *buttonLayout = new QBoxLayout(QBoxLayout::LeftToRight);
	
	DImageButton *add = new DImageButton(QIcon(THEME_DIR+"/icons/plussign.png"),22, 0, false);
	buttonLayout->addWidget(add);
	
	connect(add, SIGNAL(clicked()), this, SLOT(addCurrentValue()));
	
	DImageButton *del = new DImageButton(QIcon(THEME_DIR+"/icons/minussign.png"), 22, 0, false);
	
	connect(del, SIGNAL(clicked()), this, SLOT(removeCurrentValue()));
	
	buttonLayout->addWidget(del);
	
	mainLayout->addLayout(buttonLayout);
	
	DCONFIG->beginGroup("Brush tool");
	double smoothness = DCONFIG->value("smoothness", -1).toDouble();
	
	if ( smoothness > 0 )
	{
		m_exactness->setValue(smoothness);
	}
}



ExactnessConfigurator::~ExactnessConfigurator()
{
	DCONFIG->beginGroup("Brush tool");
	DCONFIG->setValue("smoothness", m_exactness->value());
}

double ExactnessConfigurator::exactness() const
{
	return m_exactness->value();
}


void ExactnessConfigurator::resizeEvent(QResizeEvent *)
{
	int cellWidth = m_table->viewport()->width() / m_table->columnCount();
	
	for(int colIndex = 0; colIndex < m_table->columnCount(); colIndex++)
	{
		m_table->horizontalHeader()->resizeSection (colIndex, cellWidth );
	}
	
}


void ExactnessConfigurator::addCurrentValue()
{
	double value = m_exactness->value();
	
	QList<QTableWidgetItem *> selectionList = m_table->selectedItems();
	
	if ( !selectionList.isEmpty() )
	{
		selectionList[0]->setText(QString::number(value));
	}
}

void ExactnessConfigurator::removeCurrentValue()
{
	QList<QTableWidgetItem *> selectionList = m_table->selectedItems();
	
	if ( !selectionList.isEmpty() )
	{
		selectionList[0]->setText("");
	}
}

void ExactnessConfigurator::updateValueFromItem(QTableWidgetItem *item)
{
	if ( item )
	{
		bool ok = false;
		double value = item->text().toDouble(&ok);
		
		if ( ok )
		{
			m_exactness->setValue(value);
		}
	}
}

