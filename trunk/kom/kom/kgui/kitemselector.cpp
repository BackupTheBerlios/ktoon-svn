/***************************************************************************
 *   Project KOM: KToon Open Media 0.1                                     *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2006 by David Cuadrado <krawek@gmail.com>               *
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

#include "kitemselector.h"

#include <QListWidget>
#include <QToolButton>
#include <QVBoxLayout>
#include <QHBoxLayout>

struct KItemSelector::Private
{
	QListWidget *available;
	QListWidget *selected;
};

KItemSelector::KItemSelector(QWidget *parent) : QWidget(parent), k( new Private )
{
	QHBoxLayout *layout = new QHBoxLayout(this);
	
	k->available = new QListWidget;
	
	layout->addWidget(k->available);
	
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
	
	k->selected = new QListWidget;
	layout->addWidget(k->selected);
}


KItemSelector::~KItemSelector()
{
	delete k;
}


void KItemSelector::addCurrent()
{
	int row = k->available->currentRow();
	if( row >= 0 )
	{
		QListWidgetItem *item = k->available->takeItem(row);
		k->selected->addItem(item);
		
		emit changed();
	}
}

void KItemSelector::removeCurrent()
{
	int row = k->selected->currentRow();
	if( row >= 0 )
	{
		QListWidgetItem *item = k->selected->takeItem(row);
		k->available->addItem(item);
		
		emit changed();
	}
}

void KItemSelector::upCurrent()
{
	int row = k->selected->currentRow();
	if( row >= 0 )
	{
		QListWidgetItem *item = k->selected->takeItem(row);
		
		k->selected->insertItem(row-1,item);
		
		k->selected->setCurrentItem(item);
		
		emit changed();
	}
}

void KItemSelector::downCurrent()
{
	int row = k->selected->currentRow();
	if( row >= 0 )
	{
		QListWidgetItem *item = k->selected->takeItem(row);
		
		k->selected->insertItem(row+1,item);
		k->selected->setCurrentItem(item);
		
		emit changed();
	}
}


void KItemSelector::setItems(const QStringList &items)
{
	k->available->clear();
	addItems(items);
}

int KItemSelector::addItem(const QString &item)
{
	QListWidgetItem *it = new QListWidgetItem(item, k->available);
	
	int index = k->available->count()-1;
	
	it->setData(4321, index);
	
	return index;
}

void KItemSelector::addItems(const QStringList &items)
{
	foreach(QString item, items)
		addItem(item);
}

QStringList KItemSelector::selectedItems() const
{
	QStringList items;
	for(int row = 0; row < k->selected->count(); row++)
	{
		QListWidgetItem *item = k->selected->item(row);
		items << item->text();
	}
	
	return items;
}

QList<int> KItemSelector::selectedIndexes() const
{
	QList<int> indexes;
	for(int row = 0; row < k->selected->count(); row++)
	{
		QListWidgetItem *item = k->selected->item(row);
		indexes << item->data(4321).toInt();
	}
	
	return indexes;
}

void KItemSelector::clear()
{
	k->available->clear();
	reset();
}

void KItemSelector::reset()
{
	k->selected->clear();
	
	emit changed();
}

