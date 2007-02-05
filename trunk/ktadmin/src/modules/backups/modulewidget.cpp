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
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>

#include <QDomDocument>

#include "packages/projectlistparser.h"
#include "packages/listprojects.h"
#include "package.h"

#include "backuplistparser.h"

#include <ddebug.h>

namespace Backups {

struct ModuleWidget::Private
{
	bool initialized;
};

ModuleWidget::ModuleWidget(QWidget *parent)
 : Base::ModuleListWidget(Base::ModuleButtonBar::Add | Base::ModuleButtonBar::Del | Base::ModuleButtonBar::Custom1 | Base::ModuleButtonBar::Query, parent), d(new Private)
{
	d->initialized = false;
	
	setWindowTitle(tr("Backups"));
	setWindowIcon(QIcon(THEME_DIR+"/icons/people.png"));
	setHeaders( QStringList() << tr("Project") << tr("Date"));
	
	buttonBar()->setText(Base::ModuleButtonBar::Custom1, tr("Restore"));
	buttonBar()->setStatusTip(Base::ModuleButtonBar::Custom1, tr("Restore backup"));
	buttonBar()->setIcon(Base::ModuleButtonBar::Custom1, QIcon(THEME_DIR+"/icons/reload.png"));
}


ModuleWidget::~ModuleWidget()
{
	delete d;
}

void ModuleWidget::handlePackage(Base::Package *const pkg)
{
	if(pkg->root() == "projectlist")
	{
		d->initialized = true;
		
		Packages::ProjectListParser parser;
		if(parser.parse(pkg->xml()))
		{
			typedef QHash<QString, QString> Hash;
			foreach(Hash values, parser.info())
			{
				QTreeWidgetItem *item = new QTreeWidgetItem(tree());
				item->setText(0, values["name"]);
			}
		}
		else
		{
			d->initialized = false;
		}
		
		setClearOnUpdate(!d->initialized);
		
		emit sendPackage("<listbackups />");
	}
	else if( pkg->root() == "backuplist" )
	{
		Backups::BackupListParser parser;
		
		if( parser.parse(pkg->xml()) )
		{
			for(int i = 0; i < tree()->topLevelItemCount(); i++)
			{
				QTreeWidgetItem *project = tree()->topLevelItem(i);
				
				QStringList backups = parser.backups(project->text(0));
				
				foreach(QString backup, backups)
				{
					QTreeWidgetItem *item = new QTreeWidgetItem(project);
					item->setText(1, backup);
				}
			}
			setFilled(true);
		}
	}
	else if(pkg->root() == "addproject")
	{
		QDomDocument doc;
		doc.setContent(pkg->xml());
		
		QDomElement infoE = doc.firstChildElement("info");
		
		if(!infoE.isNull())
		{
			QTreeWidgetItem *item = new QTreeWidgetItem(tree());
			item->setText(0, infoE.attribute("name"));
		}
	}
}

void ModuleWidget::updateList()
{
	if( !d->initialized )
	{
		tree()->clear();
		Packages::ListProjects pkg;
		pkg.requestAll();
		
		emit sendPackage(pkg.toString());
	}
	else
	{
		emit sendPackage("<listbackups />");
	}
}

void ModuleWidget::addActionSelected(QTreeWidgetItem *current)
{
}

void ModuleWidget::delActionSelected(QTreeWidgetItem *current)
{
}

void ModuleWidget::queryActionSelected(QTreeWidgetItem *current)
{
}

void ModuleWidget::custom1ActionSelected(QTreeWidgetItem *current)
{
}


}
