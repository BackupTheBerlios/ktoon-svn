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

#include <dapplicationproperties.h>

#include <QIcon>
#include <QTreeWidgetItem>

#include "package.h"

#include "registerlistparser.h"


namespace Registers {

struct ModuleWidget::Private
{
	
};

ModuleWidget::ModuleWidget(QWidget *parent)
 : Base::ModuleListWidget(Base::ModuleButtonBar::Add | Base::ModuleButtonBar::Del, parent), d(new Private)
{
	setWindowTitle(tr("Registers"));
	setWindowIcon(QIcon(THEME_DIR+"/icons/picture.png"));
	setHeaders( QStringList() << tr("Login") << tr("Email"));
}


ModuleWidget::~ModuleWidget()
{
	delete d;
}

void ModuleWidget::handlePackage(Base::Package *const pkg)
{
	if( pkg->root() == "registerlist" )
	{
		Packages::RegisterListParser parser;
		
		if( parser.parse(pkg->xml()) )
		{
			typedef QHash<QString, QString> Hash;
			foreach(Hash hash, parser.registers())
			{
				QTreeWidgetItem *login = new QTreeWidgetItem(tree());
				login->setText(0, hash["login"]);
				login->setText(1, hash["email"]);
			}
			
			setFilled(true);
			pkg->accept();
		}
	}
}

void ModuleWidget::updateList()
{
	emit sendPackage("<listregisters/>");
}


void ModuleWidget::addActionSelected(QTreeWidgetItem* current)
{
	ModuleListWidget::addActionSelected(current);
}

void ModuleWidget::delActionSelected(QTreeWidgetItem* current)
{
	ModuleListWidget::delActionSelected(current);
}

void ModuleWidget::modifyActionSelected(QTreeWidgetItem* current)
{
	ModuleListWidget::modifyActionSelected(current);
}

void ModuleWidget::queryActionSelected(QTreeWidgetItem* current)
{
	ModuleListWidget::queryActionSelected(current);
}

}
