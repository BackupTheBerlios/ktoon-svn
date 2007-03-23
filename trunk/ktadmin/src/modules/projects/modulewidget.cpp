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

#include <dcore/dapplicationproperties.h>
#include <dcore/ddebug.h>


#include <QIcon>
#include <QTreeWidget>
#include <QDomDocument>

#include <package.h>
#include "packages/projectlistparser.h"
#include "packages/listprojects.h"
#include "packages/removeproject.h"
#include "packages/queryproject.h"
#include "packages/userlistparser.h"

#include "projectqueryparser.h"
#include "projectactionparser.h"

#include "form.h"

namespace Projects {

struct ModuleWidget::Private
{
	QStringList users;
};

ModuleWidget::ModuleWidget(QWidget *parent)
	: Base::ModuleListWidget(Base::ModuleButtonBar::Add | Base::ModuleButtonBar::Del | Base::ModuleButtonBar::Modify, parent), d(new Private())
{
	setWindowTitle(tr("Projects"));
	setWindowIcon(QIcon(THEME_DIR+"/icons/attach.png"));
	setHeaders( QStringList() << tr("Name") << tr("Author") << tr("Description"));
}


ModuleWidget::~ModuleWidget()
{
}

void ModuleWidget::updateList()
{
	Packages::ListProjects pkg;
	pkg.requestAll();
	
	emit sendPackage(pkg.toString());
}

void ModuleWidget::handlePackage(Base::Package *const pkg)
{
	if(pkg->root() == "projectlist")
	{
		if( filled() ) return;
		
		Packages::ProjectListParser parser;
		if(parser.parse(pkg->xml()))
		{
			typedef QHash<QString, QString> Hash;
			foreach(Hash values, parser.info())
			{
				QTreeWidgetItem *item = new QTreeWidgetItem(tree());
				
				item->setText(0, values["name"]);
				item->setText(1, values["author"]);
				item->setText(2, values["description"]);
			}
			setFilled( true);
		}
	}
	else if(pkg->root() == "addproject" )
	{
		Projects::ProjectActionParser parser;
		if(parser.parse(pkg->xml()))
		{
			QTreeWidgetItem *item = new QTreeWidgetItem(tree());
			item->setText(0, parser.name());
			item->setText(1, parser.author());
			item->setText(2, parser.description());
		}
	}
	else if(pkg->root() == "updateproject")
	{
		Projects::ProjectActionParser parser;
		if(parser.parse(pkg->xml()))
		{
			removeProject(parser.name());
			QTreeWidgetItem *item = new QTreeWidgetItem(tree());
			item->setText(0, parser.name());
			item->setText(1, parser.author());
			item->setText(2, parser.description());
		}
	}
	else if(pkg->root() == "removeproject")
	{
		Projects::ProjectActionParser parser;
		if(parser.parse(pkg->xml()))
		{
			removeProject(parser.name());
		}
	}
	else if(pkg->root() == "projectquery")
	{
		Projects::ProjectQueryParser parser;
		if(parser.parse(pkg->xml()))
		{
			Form *form = new Form(parser.name(), parser.author(), parser.description(), d->users, parser.users());
			registerForm(form);
		}
	}
	else if(pkg->root() == "userlist")
	{
		Packages::UserListParser parser;
		if(parser.parse(pkg->xml()))
		{
			foreach(QStringList values, parser.info())
			{
				d->users << values[0];
			}
		}
	}
	
}


void ModuleWidget::addActionSelected(QTreeWidgetItem *)
{
	Form *form = new Form(d->users);
	registerForm(form);
}

void ModuleWidget::delActionSelected(QTreeWidgetItem *current)
{
	if(current)
	{
		Packages::RemoveProject remove(current->text(0));
		emit sendPackage(remove.toString());
	}
}

void ModuleWidget::modifyActionSelected(QTreeWidgetItem *current)
{
	if(current)
	{
		Packages::QueryProject query(current->text(0));
		emit sendPackage(query.toString());
	}
}

void ModuleWidget::removeProject(const QString& name)
{
	QTreeWidgetItemIterator it(tree());
	while( (*it) )
	{
		if( name == (*it)->text(0) )
		{
			delete (*it);
			break;
		}
		++it;
	}
}

}
