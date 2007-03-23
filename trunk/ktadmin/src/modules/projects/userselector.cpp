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
 
#include "userselector.h"

#include <dcore/ddebug.h>

#include <QTreeWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QToolButton>
#include <QCheckBox>
#include <QButtonGroup>

namespace Projects {

struct UserSelector::Private
{
	QTreeWidget *left;
	QTreeWidget *right;
	QHBoxLayout *layout;
	QHash<QString, QButtonGroup*> buttons;
};

UserSelector::UserSelector() : QFrame(), d(new Private)
{
	d->layout = new QHBoxLayout(this);
	
	d->left = new QTreeWidget;
	d->left->setSelectionMode ( QAbstractItemView::ContiguousSelection );
	d->left->setHeaderLabels ( QStringList() << tr("login") );
	
	d->right = new QTreeWidget;
	d->right->setSelectionMode ( QAbstractItemView::ContiguousSelection );
	d->right->setHeaderLabels ( QStringList() << tr("login") << tr("Owner") << tr("Designer") );
	d->right->setColumnCount(3);
	
	QVBoxLayout *buttons = new QVBoxLayout;
	QToolButton *bottonLeft = new QToolButton;
	buttons->addWidget(bottonLeft);
	bottonLeft->setText("<<");
	connect(bottonLeft, SIGNAL(clicked()), this, SLOT( moveSelectedItemsToLeft()));
	
	QToolButton *bottonRight = new QToolButton;
	bottonRight->setText(">>");
	connect(bottonRight, SIGNAL(clicked()), this, SLOT(moveSelectedItemsToRight()));
	buttons->addWidget(bottonRight);
	
	d->layout->addWidget(d->left);
	d->layout->addLayout(buttons);
	d->layout->addWidget(d->right);
}


UserSelector::~UserSelector()
{
	delete d->left;
	delete d->right;
	qDeleteAll (d->buttons);
}


QTreeWidget *UserSelector::leftTree()
{
	return d->left;
}

QTreeWidget *UserSelector::rightTree()
{
	return d->right;
}



void UserSelector::addUserAvailable(const QString& login)
{
	QTreeWidgetItem *user = new QTreeWidgetItem(d->left);
	user->setText(0, login);
}
		
void UserSelector::addUserSelected(const QString& login, UserType type )
{
	QTreeWidgetItem *user = new QTreeWidgetItem(d->right);
	user->setText(0, login);
	
	QButtonGroup *group = new QButtonGroup(this);
	
	QCheckBox *owner = new QCheckBox();
	QCheckBox *designer = new QCheckBox();
	
	if(type == Owner)
	{
		owner->setChecked(true);
	}
	else if(type == Designer)
	{
		designer->setChecked(true);
	}
	
	group->addButton(owner, 0);
	group->addButton(designer, 1);

	d->buttons.insert(login, group);
	d->right->setItemWidget ( user, 1, owner );
	d->right->setItemWidget ( user, 2, designer );
}

QMultiHash<UserSelector::UserType, QString> UserSelector::selectedUsers()
{
	QMultiHash<UserType, QString>  hash;
	QTreeWidgetItemIterator it(d->right);
	while( (*it) )
	{
		if(QCheckBox *owner =  qobject_cast<QCheckBox*>(d->right->itemWidget((*it), 1)))
		{
			if(QCheckBox *designer =  qobject_cast<QCheckBox*>(d->right->itemWidget((*it), 2)))
			{
				if(owner->isChecked())
				{
					hash.insert(Owner, (*it)->text(0));
				}
				else if(designer->isChecked())
				{
					hash.insert(Designer, (*it)->text(0));
				}
			}
		}
		++it;
	}
	return hash;
}



void UserSelector::moveSelectedItemsToLeft()
{
	foreach(QTreeWidgetItem *item,  d->right->selectedItems())
	{
		if(item)
		{
			d->right->takeTopLevelItem (  d->right->indexOfTopLevelItem (item) );
			
			d->left->addTopLevelItem ( item );
			d->left->setCurrentItem(item );
			
			delete d->buttons.take(item->text(0));
		}
	}
}

void UserSelector::moveSelectedItemsToRight()
{
	foreach(QTreeWidgetItem *item,  d->left->selectedItems())
	{
		if(item)
		{
			d->left->takeTopLevelItem (  d->left->indexOfTopLevelItem (item) );
			d->right->addTopLevelItem ( item );
			d->right->setCurrentItem(item );
			
			QButtonGroup *group = new QButtonGroup(this);
	
			QCheckBox *owner = new QCheckBox();
			
			QCheckBox *designer = new QCheckBox();
			designer->setChecked(true);
			group->addButton(owner, 0);
			group->addButton(designer, 1);

			d->buttons.insert(item->text(0), group);
			d->right->setItemWidget ( item, 1, owner );
			d->right->setItemWidget ( item, 2, designer );
		}
	}
}
}
