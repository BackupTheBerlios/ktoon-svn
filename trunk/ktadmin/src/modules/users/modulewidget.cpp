/***************************************************************************
 *   Copyright (C) 2007 by David Cuadrado                                  *
 *   krawek@gmail.com                                                      *
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

#include "modulewidget.h"
#include "modulebuttonbar.h"

#include <dtreelistwidget.h>
#include <dtreewidgetsearchline.h>
#include <ddebug.h>

#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>

#include <QMainWindow>


namespace Users {


struct ModuleWidget::Private
{
	QTreeWidget *tree;
	DTreeWidgetSearchLine *search;
	Base::ModuleButtonBar *buttonBar;
};

ModuleWidget::ModuleWidget(QWidget *parent)
	: Base::ModuleWidgetBase(parent), d(new Private)
{
	setWindowTitle(tr("Users"));
	
	QVBoxLayout *layout = new QVBoxLayout(this);
	setLayout(layout);
	QHBoxLayout *search = new QHBoxLayout;

	QToolButton *button = new QToolButton;
	button->setIcon( QIcon(THEME_DIR+"/icons/clear_right.png"));
	
	search->addWidget(button);
	d->tree = new  QTreeWidget;
	
	
	d->tree->setHeaderLabels( QStringList() << tr("Login") << tr("Name") );
	d->tree->header()->show();
	d->search = new DTreeWidgetSearchLine(this,d->tree);
	search->addWidget( d->search );
	connect(button, SIGNAL(clicked()), d->search, SLOT(clear()));
	
	layout->addLayout(search);
	layout->addWidget(d->tree);
	
	d->buttonBar = new Base::ModuleButtonBar(Base::ModuleButtonBar::Add | Base::ModuleButtonBar::Del | Base::ModuleButtonBar::Modify);
	layout->addWidget(d->buttonBar);
	connect(d->buttonBar, SIGNAL(buttonClicked( int )), this, SLOT(requestAction(int)));
}


ModuleWidget::~ModuleWidget()
{
}

void ModuleWidget::requestAction( int action)
{
	switch(action)
	{
		case Base::ModuleButtonBar::Add:
		{
			Form *form = new Form();
			emit postWidget(form);
			connect(form, SIGNAL(sendPackage(const QString &)), this, SIGNAL(sendPackage(const QString &)));
		}
		break;
		case Base::ModuleButtonBar::Del:
		{
		}
		break;
		case Base::ModuleButtonBar::Modify:
		{
		}
		break;
	}
}

void ModuleWidget::handlePackage(Base::Package *const pkg)
{
	SHOW_VAR(pkg->xml());
	//TODO si es un paquete de añadir un usuario se añade a la lista
}

}
