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
#include "permissionschooser.h"


#include <QCheckBox>
#include <QGridLayout>
#include <QLabel>
#include <QButtonGroup>

#include <ddebug.h>
#include <cmath>
#include <QMultiHash>

namespace Users {

struct PermissionsChooser::Private
{
	QList<Users::Permission> permissions;
	QButtonGroup *group;
	QMultiHash<QString, QCheckBox *> checks;
};

PermissionsChooser::PermissionsChooser(const QStringList& modules, QWidget *parent) : QGroupBox("Permissions", parent), d(new Private)
{
	QGridLayout *layout = new QGridLayout(this);
	
	layout->addWidget(new QLabel(tr("Module")), 0,0);
	layout->addWidget(new QLabel(tr("Read")), 0,1);
	layout->addWidget(new QLabel(tr("Write")), 0,2);
	d->group = new QButtonGroup(this) ;
	d->group->setExclusive ( false );
	connect(d->group, SIGNAL(buttonClicked ( int )), this, SLOT(changePremission(int)));
	int count = 1;
	foreach(QString module, modules)
	{
		Users::Permission premission;
		premission.setModule( module );
		premission.setRead(false);
		premission.setWrite(false);
		d->permissions << premission;
		
		layout->addWidget(new QLabel(module), count, 0);
		QCheckBox *read = new QCheckBox();
		d->checks.insert(module, read);
		layout->addWidget(read, count, 1);
		d->group->addButton (read, 2*count);
		
		QCheckBox *write = new QCheckBox();
		d->checks.insert(module, write);
		layout->addWidget(write, count, 2);
		d->group->addButton (write, 2*count+1);
		count++;
	}
}


PermissionsChooser::~PermissionsChooser()
{
}

QList<Users::Permission> PermissionsChooser::permissions()
{
	return d->permissions;
}


void PermissionsChooser::setPermissions(const QList<Users::Permission> & permissions )
{
	d->permissions = permissions;
	foreach(Users::Permission permission , permissions)
	{
		QList<QCheckBox*> values = d->checks.values(permission.module());
		
		values[0]->setChecked ( permission.read() );
		values[1]->setChecked ( permission.write() );
	}
}

void PermissionsChooser::changePremission(int id)
{
	if(id%2 == 0)
	{
		d->permissions[(int) ::ceil(id/2)-1].setRead(d->group->button(id)->isChecked ());
	}
	else
	{
		d->permissions[(int)::ceil(id/2)-1].setWrite(d->group->button(id)->isChecked ());
	}
}

}
