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
#include <QTreeWidget>
#include <QDomDocument>

#include <package.h>
#include "packages/projectlistparser.h"
#include "packages/listprojects.h"

#include "form.h"

namespace Projects {

struct ModuleWidget::Private
{
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
	if(pkg->root() == "addproject")
	{
		QDomDocument doc;
		doc.setContent(pkg->xml());
		
		QDomElement infoE = doc.firstChildElement("info");
		
		if(!infoE.isNull())
		{
			QTreeWidgetItem *item = new QTreeWidgetItem(tree());
			item->setText(0, infoE.attribute("name"));
			item->setText(1, infoE.attribute("author"));
			item->setText(2, infoE.attribute("description"));
		}
	}
}

void ModuleWidget::addActionSelected(QTreeWidgetItem *)
{
	Form *form = new Form();
	registerForm(form);
}

void ModuleWidget::delActionSelected(QTreeWidgetItem *current)
{
}

void ModuleWidget::modifyActionSelected(QTreeWidgetItem *current)
{
	
}

}
