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

#include "dconfigurationdialog.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QStackedWidget>
#include <QLabel>
#include <QHeaderView>
#include <QDialogButtonBox>
#include <QListWidget>

#include "dseparator.h"

#include "ddebug.h"

////////////////

struct DConfigurationDialog::Private
{
	QListWidget *list;
	QStackedWidget *pageArea;
	
};

DConfigurationDialog::DConfigurationDialog(QWidget *parent) : QDialog(parent), d(new Private)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	
	QHBoxLayout *pages = new QHBoxLayout;
	
	d->list = new QListWidget;
	d->list->setViewMode(QListView::IconMode);
	d->list->setFlow(QListView::TopToBottom);
	d->list->setIconSize(QSize(96, 84));
	d->list->setMovement(QListView::Static);
	d->list->setMaximumWidth(128);
	d->list->setSpacing(12);
	
	connect(d->list, SIGNAL(currentItemChanged(QListWidgetItem *, QListWidgetItem *)), this, SLOT(changePage(QListWidgetItem *, QListWidgetItem*)));
	
	pages->addWidget(d->list);
	
	d->pageArea = new QStackedWidget;
	pages->addWidget(d->pageArea, 1);
	
	layout->addLayout(pages);
	
	QDialogButtonBox *buttonBox = new QDialogButtonBox(QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Apply, Qt::Horizontal, this);
	
	connect(buttonBox, SIGNAL(accepted()), this, SLOT(ok()));
	connect(buttonBox, SIGNAL(rejected()), this, SLOT(cancel()));
	connect(buttonBox->button(QDialogButtonBox::Apply), SIGNAL(clicked()), this, SLOT(apply()));
	
	layout->addWidget(new DSeparator());
	layout->addWidget(buttonBox);
}


DConfigurationDialog::~DConfigurationDialog()
{
	delete d;
}

void DConfigurationDialog::addPage(QWidget *page, const QString &label, const QIcon &icon)
{
	QListWidgetItem *pageItem = new QListWidgetItem(d->list);
	pageItem->setIcon(icon);
	pageItem->setText(label);
	pageItem->setTextAlignment(Qt::AlignHCenter);
	pageItem->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
	
	d->pageArea->addWidget(page);
}

QWidget *DConfigurationDialog::currentPage() const
{
	return d->pageArea->currentWidget();
}

void DConfigurationDialog::ok()
{
	accept();
}

void DConfigurationDialog::cancel()
{
	reject();
}

void DConfigurationDialog::apply()
{
}

void DConfigurationDialog::changePage(QListWidgetItem *curr, QListWidgetItem *prev)
{
	if (!curr)
		curr = prev;
	
	d->pageArea->setCurrentIndex(d->list->row(curr));
}

