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

#include <QLineEdit>
#include <QVBoxLayout>

namespace Users {

struct Form::Private
{
	QLineEdit *name;
	QLineEdit *login;
	QLineEdit *password;
};

Form::Form(QWidget *parent) : QWidget(parent), d(new Private)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	d->login = new QLineEdit;
	layout->addLayout(DFormFactory::makeLine(tr("login"), d->login ));
	
	d->password = new QLineEdit;
	d->password->setEchoMode(QLineEdit::Password);
	layout->addLayout(DFormFactory::makeLine(tr("password"),  d->password ));
	
	d->name = new QLineEdit;
	layout->addLayout(DFormFactory::makeLine(tr("name"), d->name ));
	
}


Form::~Form()
{
}


}
