/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
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
 

#include "ktlistprojectdialog.h"
#include <kgui/ktreelistwidget.h>
#include <kgui/ktreewidgetsearchline.h>

#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>

struct KTListProjectDialog::Private
{
	QTreeWidget *tree;
	DTreeWidgetSearchLine *search;
	QPushButton *accept, *cancel;

};

KTListProjectDialog::KTListProjectDialog()
	: QDialog(), d(new Private)
{
	setWindowTitle ( tr("List Projects from Server"));
	setModal ( true );
	QVBoxLayout *layout = new QVBoxLayout(this);
	setLayout(layout);
	QHBoxLayout *search = new QHBoxLayout;

	QToolButton *button = new QToolButton;
	button->setIcon( QIcon(THEME_DIR+"/icons/clear_right.png"));


	search->addWidget(button);
	d->tree = new  QTreeWidget;
	
	
	d->tree->setHeaderLabels( QStringList() << tr("name") << tr("author") << tr("description") );
	d->tree->header()->show();
	d->search = new DTreeWidgetSearchLine(this,d->tree);
	search->addWidget( d->search );
	connect(button, SIGNAL(clicked()), d->search, SLOT(clear()));
	
	connect(d->tree, SIGNAL(itemDoubleClicked ( QTreeWidgetItem *, int)), this, SLOT(execAccept(QTreeWidgetItem * , int )));
	
	layout->addLayout(search);
	layout->addWidget(d->tree);
	
	//----
	QHBoxLayout *buttons = new QHBoxLayout;
	d->accept = new QPushButton(tr("OK"));
	d->cancel = new QPushButton("Cancel");
	connect(d->accept, SIGNAL( clicked ()), this, SLOT(accept()));
	connect(d->cancel, SIGNAL( clicked()), this, SLOT(reject()));
	buttons->addWidget(d->accept);
	buttons->addWidget(d->cancel);
	layout->addLayout(buttons);
	
	
	
}


KTListProjectDialog::~KTListProjectDialog()
{
}

void KTListProjectDialog::addProject(const QString& name, const QString& author, const QString& description)
{
	QTreeWidgetItem *item = new QTreeWidgetItem(d->tree);
	item->setText(0, name);
	item->setText(1, author);
	item->setText(2, description);
}

QString KTListProjectDialog::currentProject()
{
	QTreeWidgetItem *item = d->tree->currentItem ();
	if(item)
	{
		return item->text(0);
	}
	return "";
}

void KTListProjectDialog::execAccept(QTreeWidgetItem * , int )
{
	accept();
}
