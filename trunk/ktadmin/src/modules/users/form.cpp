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
#include <dformfactory.h>
#include <ddebug.h>

#include <QLineEdit>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QAbstractButton>
#include "packages/adduser.h"

namespace Users {

struct Form::Private
{
	QLineEdit *name;
	QLineEdit *login;
	QLineEdit *password;
	QDialogButtonBox *buttons;
};

Form::Form(QWidget *parent) : Base::Form( QDialogButtonBox::Ok | QDialogButtonBox::Cancel | QDialogButtonBox::Reset, "Users", parent ), d(new Private)
{
	QWidget *container = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(container);
	d->login = new QLineEdit;
	layout->addLayout(DFormFactory::makeLine(tr("login"), d->login ));
	
	d->password = new QLineEdit;
	d->password->setEchoMode(QLineEdit::Password);
	layout->addLayout(DFormFactory::makeLine(tr("password"),  d->password ));
	
	d->name = new QLineEdit;
	layout->addLayout(DFormFactory::makeLine(tr("name"), d->name ));
	
	d->buttons = new QDialogButtonBox();
	layout->addWidget(d->buttons);
	
	setCentralWidget(container);

}

void Form::applyAction( QDialogButtonBox::ButtonRole role )
{
	switch(role)
	{
		case QDialogButtonBox::AcceptRole:
		{
			//TODO:validar con DFormValidator
			Packages::AddUser adduser(d->login->text(), d->password->text(),d->name->text());
			emit sendPackage(adduser.toString());
			clear();
		}
		break;
		case QDialogButtonBox::RejectRole:
		{
			close();
		}
		break;
		case QDialogButtonBox::ResetRole:
		{
			clear();
		}
		break;
		default:
		{
		}
		break;
	}
}

Form::~Form()
{
}

void Form::clear()
{
	d->password->clear();
	d->name->clear();
	d->login->clear();
}

}
