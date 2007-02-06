/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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
#include <QTextEdit>
#include <QVBoxLayout>
#include <QLabel>

#include <dformfactory.h>

#include "packages/addproject.h"
#include "packages/updateproject.h"

namespace Projects {

struct Form::Private
{
	QLineEdit *name;
	QLineEdit *author;
	QTextEdit *description;
	bool update;
};

Form::Form(QWidget *parent): Base::Form(tr("Add project"), parent), d(new Private())
{
	
	d->update = false;
	init();
}


Form::Form(const QString &name, const QString &author, const QString & description, const QStringList& users, QWidget *parent): Base::Form(tr("Modify project"), parent), d(new Private())
{
	d->update = true;
	init();
	d->name->setText(name);
	d->name->setDisabled(true);
	
	d->author->setText(author);
	d->description->setText(description);
}

Form::~Form()
{
	
}

void Form::resetForm()
{
	d->name->clear();
	d->author->setText(tr("anonymous"));
	d->description->clear();
}

void Form::acceptForm()
{
	if(d->update)
	{
		Packages::UpdateProject update(d->name->text());
		
		if( d->author->isModified ())
		{
			update.setAuthor(d->author->text());
		}
		
		update.setDescription(d->description->toPlainText());
		
		emit sendPackage(update.toString());
	}
	else
	{
		Packages::AddProject project(d->name->text(), d->author->text(), d->description->toPlainText());
		emit sendPackage(project.toString());
	}
}

//private
void Form::init()
{
	QWidget *container = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(container);
	
	d->name = new QLineEdit();
	validatesRegExpOf("\\w+", d->name);
	layout->addLayout(DFormFactory::makeLine(tr("name"), d->name ));
	
	d->author = new QLineEdit(tr("anonymous"));
	validatesRegExpOf("\\w+", d->author);
	layout->addLayout(DFormFactory::makeLine(tr("author"), d->author ));
	
	layout->addWidget(new QLabel(tr("description")));
	d->description = new QTextEdit;
	layout->addWidget(d->description);
	
	setCentralWidget(container);
}

}
