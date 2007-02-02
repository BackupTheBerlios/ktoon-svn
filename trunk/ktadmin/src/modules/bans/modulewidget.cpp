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

#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QHash>

#include "package.h"

#include "packages/removeban.h"

#include "banlistparser.h"
#include "removebanparser.h"
#include "form.h"

namespace Bans {

struct ModuleWidget::Private
{
	QHash<QString, QTreeWidgetItem *> items;
};

ModuleWidget::ModuleWidget(QWidget *parent) : Base::ModuleListWidget(Base::ModuleButtonBar::Add | Base::ModuleButtonBar::Del | Base::ModuleButtonBar::Modify, parent), d(new Private)
{
	setWindowTitle(tr("Bans"));
	
	setHeaders(QStringList() << tr("Ban"));
}


ModuleWidget::~ModuleWidget()
{
	delete d;
}

void ModuleWidget::handlePackage(Base::Package *const pkg)
{
	if( pkg->root() == "banlist" )
	{
		BanListParser parser;
		if ( parser.parse(pkg->xml()) )
		{
			foreach(QString ban, parser.bans())
			{
				QTreeWidgetItem *item = new QTreeWidgetItem(tree());
				item->setText(0, ban);
				
				d->items.insert(ban, item);
			}
			setFilled(true);
			pkg->accept();
		}
	}
	else if ( pkg->root() == "removeban" )
	{
		RemoveBanParser parser;
		if( parser.parse(pkg->xml()))
		{
			QString pattern = parser.pattern();
			
			if(d->items.contains(pattern))
			{
				delete d->items.take(pattern);
			}
			
			pkg->accept();
		}
	}
}

void ModuleWidget::updateList()
{
	tree()->clear();
	emit sendPackage("<listbans/>");
}

void ModuleWidget::addActionSelected(QTreeWidgetItem */*current*/)
{
	Form *form = new Form;
	
	emit postWidget(form);
}


void ModuleWidget::delActionSelected(QTreeWidgetItem *current)
{
	if( current )
	{
		QString pattern = current->text(0);
		Packages::RemoveBan removeBan(pattern);
		
		emit sendPackage(removeBan.toString());
	}
}


void ModuleWidget::modifyActionSelected(QTreeWidgetItem */*current*/)
{
}


void ModuleWidget::queryActionSelected(QTreeWidgetItem */*current*/)
{
}



}
