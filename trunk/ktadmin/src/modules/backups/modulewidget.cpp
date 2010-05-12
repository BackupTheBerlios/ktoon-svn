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

#include <QIcon>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QTreeWidgetItemIterator>
#include <QDateTime>
#include <QHashIterator>

#include <QDomDocument>

#include "packages/projectlistparser.h"
#include "packages/listprojects.h"
#include "packages/addbackup.h"
#include "packages/removebackup.h"
#include "packages/restorebackup.h"

#include "package.h"

#include "backuplistparser.h"
#include "addbackupparser.h"

#include <kcore/kdebug.h>
#include <kcore/kapplicationproperties.h>
#include <kgui/kosd.h>

namespace Backups {

struct ModuleWidget::Private
{
	bool initialized;
};

ModuleWidget::ModuleWidget(QWidget *parent)
 : Base::ModuleListWidget(Base::ModuleButtonBar::Add | Base::ModuleButtonBar::Del | Base::ModuleButtonBar::Custom1, parent), d(new Private)
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
		if ( d->initialized ) return;
		
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
		if ( filled() || !d->initialized ) return;
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
	else if(pkg->root() == "addbackup" )
	{
		Backups::AddBackupParser parser;
		
		if(parser.parse(pkg->xml()) )
		{
			QHash<QString, QDateTime> entries = parser.entries();
			
			for(int i = 0; i < tree()->topLevelItemCount(); i++)
			{
				QTreeWidgetItem *project = tree()->topLevelItem(i);
				
				if ( entries.contains(project->text(0)) )
				{
					QTreeWidgetItem *bk = new QTreeWidgetItem(project);
					bk->setText(1, entries[project->text(0)].toString(Qt::ISODate));
				}
			}
		}
	}
	else if ( pkg->root() == "removebackup" )
	{
		Backups::AddBackupParser parser;
		
		if(! parser.parse(pkg->xml()) ) return;
		
		QHash<QString, QDateTime> entries = parser.entries();
		
		for(int i = 0; i < tree()->topLevelItemCount(); i++)
		{
			QTreeWidgetItem *project = tree()->topLevelItem(i);
			
			if ( !entries.contains(project->text(0)) ) continue;
			
			QString date = entries[project->text(0)].toString(Qt::ISODate);
			for(int i = 0; i < project->childCount(); i++)
			{
				QTreeWidgetItem *child = project->child(i);
				if(child->text(1) == date)
				{
					delete child;
					break;
				}
			}
		}
	}
	else if ( pkg->root() == "restorebackup" )
	{
		Backups::AddBackupParser parser;
		
		if(! parser.parse(pkg->xml()) ) return;
		
		QHash<QString, QDateTime> entries = parser.entries();
		
		QHashIterator<QString, QDateTime> it(entries);
		
		QString message = tr("Restored:")+"\n";
		while(it.hasNext())
		{
			it.next();
			
			message += it.key()+"-"+it.value().toString(Qt::ISODate);
		}
		KOsd::self()->display(message, KOsd::Info);
	}
}

void ModuleWidget::clear()
{
	Base::ModuleListWidget::clear();
	d->initialized = false;
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
	if( ! current ) return;
	
	QTreeWidgetItem *project = current;
	if ( current->parent() )
		project = current->parent();
	
	Packages::AddBackup pkg;
	pkg.addEntry(project->text(0));
	
	emit sendPackage(pkg.toString());
}

void ModuleWidget::delActionSelected(QTreeWidgetItem *current)
{
	if( !current ) return;
	
	if( QTreeWidgetItem *project = current->parent() )
	{
		Packages::RemoveBackup pkg;
		pkg.addEntry(project->text(0), QDateTime::fromString(current->text(1), Qt::ISODate));
		
		emit sendPackage(pkg.toString());
	}
}


void ModuleWidget::custom1ActionSelected(QTreeWidgetItem *current) // RESTORE
{
	if( !current ) return;
	
	if( QTreeWidgetItem *project = current->parent() )
	{
		Packages::RestoreBackup pkg;
		pkg.addEntry(project->text(0), QDateTime::fromString(current->text(1), Qt::ISODate));
		
		emit sendPackage(pkg.toString());
	}
}


}
