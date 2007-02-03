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

#include "form.h"
#include <QLineEdit>
#include <QVBoxLayout>
#include <QTreeWidgetItem>

#include <dformfactory.h>
#include <ddebug.h>

#include "packages/addban.h"

namespace Bans {

struct Form::Private
{
	QLineEdit *pattern;
};


Form::Form(QWidget *parent)
 : Base::Form(tr("Create a new ban..."), parent), d(new Private)
{
	QWidget *container = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(container);
	
	d->pattern = new QLineEdit;
// 	validatesMaskOf("000.000.000.000;_", d->pattern);
	
	layout->addLayout(DFormFactory::makeGrid(QStringList() << tr("Pattern"), QWidgetList() << d->pattern));
	
	setCentralWidget(container);
	
	setAcceptDrops(true);
}


Form::~Form()
{
	delete d;
}

void Form::acceptForm()
{
	Packages::AddBan pkg(d->pattern->text());
	
	emit sendPackage(pkg.toString());
	
	resetForm();
}

void Form::resetForm()
{
	d->pattern->clear();
}

void Form::itemDropped(QTreeWidgetItem *item)
{
	d->pattern->setText(item->text(0));
}

}
