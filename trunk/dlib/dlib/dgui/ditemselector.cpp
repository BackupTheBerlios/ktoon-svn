/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado   *
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

#include "ditemselector.h"

#include <QListWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

struct DItemSelector::Private
{
	QListWidget *available;
	QListWidget *selected;
};

DItemSelector::DItemSelector(QWidget *parent)
 : QWidget(parent), d( new Private )
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	
	d->available = new QListWidget;
	
	layout->addWidget(d->available);
	
	QVBoxLayout *controlBox = new QVBoxLayout(this);
	controlBox->setSpacing(0);
	controlBox->setMargin(2);
	controlBox->addStretch();
	
	QToolButton *bnext = new QToolButton;
	bnext->setText(">>");
	connect(bnext, SIGNAL(clicked()), this, SLOT(addCurrent()));
	
	controlBox->addWidget(bnext);
	
	QToolButton *bprev = new QToolButton;
	bprev->setText("<<");
	connect(bprev, SIGNAL(clicked()), this, SLOT(removeCurrent()));
	
	controlBox->addWidget(bprev);
	
	QToolButton *bup = new QToolButton;
	bup->setText("^");
	connect(bup, SIGNAL(clicked()), this, SLOT(upCurrent()));
	
	controlBox->addWidget(bup);
	
	QToolButton *bdown = new QToolButton;
	bdown->setText("v");
	connect(bdown, SIGNAL(clicked()), this, SLOT(downCurrent()));
	
	controlBox->addWidget(bdown);
	
	controlBox->addStretch();
	
	layout->addLayout(controlBox);
	
	d->selected = new QListWidget;
	layout->addWidget(d->selected);
}


DItemSelector::~DItemSelector()
{
	delete d;
}


void DItemSelector::addCurrent()
{
	int row = d->available->currentRow();
	if( row >= 0 )
	{
		QListWidgetItem *item = d->available->takeItem(row);
		d->selected->addItem(item);
		
		emit changed();
	}
}

void DItemSelector::removeCurrent()
{
	int row = d->selected->currentRow();
	if( row >= 0 )
	{
		QListWidgetItem *item = d->selected->takeItem(row);
		d->available->addItem(item);
		
		emit changed();
	}
}

void DItemSelector::upCurrent()
{
	int row = d->selected->currentRow();
	if( row >= 0 )
	{
		QListWidgetItem *item = d->selected->takeItem(row);
		
		d->selected->insertItem(row-1,item);
		
		d->selected->setCurrentItem(item);
		
		emit changed();
	}
}

void DItemSelector::downCurrent()
{
	int row = d->selected->currentRow();
	if( row >= 0 )
	{
		QListWidgetItem *item = d->selected->takeItem(row);
		
		d->selected->insertItem(row+1,item);
		d->selected->setCurrentItem(item);
		
		emit changed();
	}
}


void DItemSelector::setItems(const QStringList &items)
{
	d->available->clear();
	addItems(items);
}

int DItemSelector::addItem(const QString &item)
{
	QListWidgetItem *it = new QListWidgetItem(item, d->available);
	
	int index = d->available->count()-1;
	
	it->setData(4321, index);
	
	return index;
}

void DItemSelector::addItems(const QStringList &items)
{
	foreach(QString item, items)
		addItem(item);
}

QStringList DItemSelector::selectedItems() const
{
	QStringList items;
	for(int row = 0; row < d->selected->count(); row++)
	{
		QListWidgetItem *item = d->selected->item(row);
		items << item->text();
	}
	
	return items;
}

QList<int> DItemSelector::selectedIndexes() const
{
	QList<int> indexes;
	for(int row = 0; row < d->selected->count(); row++)
	{
		QListWidgetItem *item = d->selected->item(row);
		indexes << item->data(4321).toInt();
	}
	
	return indexes;
}

void DItemSelector::clear()
{
	d->available->clear();
	reset();
}

void DItemSelector::reset()
{
	d->selected->clear();
	
	emit changed();
}


