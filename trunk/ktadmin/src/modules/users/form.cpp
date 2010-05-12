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
#include <kgui/kformfactory.h>
#include <kcore/kdebug.h>

#include <QLineEdit>
#include <QVBoxLayout>

#include "packages/adduser.h"
#include "packages/updateuser.h"

#include "permissionschooser.h"
#include "permission.h"

namespace Users {

struct Form::Private
{
	QLineEdit *name;
	QLineEdit *login;
	QLineEdit *password;
	PermissionsChooser *permissions;
	bool update;
};

Form::Form(QWidget *parent) : Base::Form( tr("Add users"), parent ), d(new Private)
{
	d->update = false;
	init();
}

Form::Form(const QString &login, const QString name, const QList<Permission> & permissions,  QWidget *parent) : Base::Form( tr("Update user"), parent ), d(new Private)
{
	d->update = true;
	init();
	d->login->setDisabled(true);
	d->login->setText(login);
	d->name->setText(name);
	
	d->permissions->setPermissions(permissions);
	
	
}

void Form::init()
{
	QWidget *container = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(container);
	d->login = new QLineEdit;
	
	validatesRegExpOf("\\w+", d->login);
	
	layout->addLayout(KFormFactory::makeLine(tr("login"), d->login ));
	
	d->password = new QLineEdit;
	d->password->setEchoMode(QLineEdit::Password);
	layout->addLayout(KFormFactory::makeLine(tr("password"),  d->password ));
	
	d->name = new QLineEdit;
	layout->addLayout(KFormFactory::makeLine(tr("name"), d->name ));
	
	d->permissions = new PermissionsChooser(QStringList() << "project" << "admin");
	layout->addWidget(d->permissions);
	
	setCentralWidget(container);
}

void Form::acceptForm()
{
	if(d->update)
	{
		Packages::UpdateUser updateuser(d->login->text());
		
		if(!d->password->text().isNull())
		{
			updateuser.setPassword(d->password->text());
		}
		
		updateuser.setName(d->name->text());
		
		foreach(Users::Permission perm, d->permissions->permissions())
		{
			updateuser.addPermission( perm.module(), perm.read(), perm.write());
		}
		emit sendPackage(updateuser.toString());
	}
	else
	{
		Packages::AddUser adduser(d->login->text(), d->password->text(),d->name->text());
		
		foreach(Users::Permission perm, d->permissions->permissions())
		{
			adduser.addPermission( perm.module(), perm.read(), perm.write());
		}
		emit sendPackage(adduser.toString());
		resetForm();
	}
}

Form::~Form()
{
}

void Form::resetForm()
{
	d->password->clear();
	d->name->clear();
	d->login->clear();
}

}
