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
#include "modulebuttonbar.h"

#include <dtreelistwidget.h>
#include <dtreewidgetsearchline.h>
#include <ddebug.h>

#include <package.h>
#include "form.h"

#include <QToolButton>
#include <QTreeWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>

#include <QMainWindow>

#include <packages/removeuser.h>
#include <packages/queryuser.h>
#include <userlistparser.h>
#include <userparser.h>

namespace Users {


struct ModuleWidget::Private
{
};

ModuleWidget::ModuleWidget(QWidget *parent)
	: Base::ModuleListWidget(Base::ModuleButtonBar::Add | Base::ModuleButtonBar::Del | Base::ModuleButtonBar::Modify, parent), d(new Private)
{
	setWindowTitle(tr("Users"));
	setWindowIcon(QIcon(THEME_DIR+"/icons/users.png"));
	
	setHeaders( QStringList() << tr("Login") << tr("Name") );
	
}


ModuleWidget::~ModuleWidget()
{
}


void ModuleWidget::addActionSelected(QTreeWidgetItem *current)
{
	Form *form = new Form();
	registerForm(form);
	
}

void ModuleWidget::delActionSelected(QTreeWidgetItem *current)
{
	if(current)
	{
		Packages::RemoveUser removeuser(current->text(0));
		emit sendPackage(removeuser.toString());
	}
}


void ModuleWidget::modifyActionSelected(QTreeWidgetItem *current)
{
	if(current)
	{
		Packages::QueryUser query(current->text(0));
		emit sendPackage(query.toString());
	}
}

void ModuleWidget::handlePackage(Base::Package *const pkg)
{
	if(pkg->root() == "adduser")
	{
		QDomDocument doc;
		doc.setContent(pkg->xml());
		
		QString login, name;
	
		QDomElement loginE = doc.firstChild().firstChildElement("login");
		if(!loginE.isNull())
		{
			QDomNode loginText = loginE.firstChild();
			if(loginText.isText())
			{
				
				login =  loginText.toText().data();
			}
		}
		
		QDomElement nameE = doc.firstChild().firstChildElement("name");
		
		if(!nameE.isNull())
		{
			QDomNode nameText = nameE.firstChild();
			if(nameText.isText())
			{
				name =  nameText.toText().data();
			}
		}
		
		if(!(name.isNull() || login.isNull()))
		{
			QTreeWidgetItem *item = new QTreeWidgetItem(tree());
			item->setText(0, login);
			item->setText(1, name);
		}
		pkg->accept();
	}
	else if(pkg->root() == "removeuser")
	{
		QDomDocument doc;
		doc.setContent(pkg->xml());
		QDomElement login = doc.firstChild().firstChildElement("login");
		if(!login.isNull())
		{
			QDomNode loginText = login.firstChild();
			if(loginText.isText())
			{
				removeUser(loginText.toText().data()); 
			}
		}
		pkg->accept();
	}
	else if(pkg->root() == "updateuser")
	{
		QDomDocument doc;
		doc.setContent(pkg->xml());
	
		QString login, name;
		
		QDomElement loginE = doc.firstChild().firstChildElement("login");
		if(!loginE.isNull())
		{
			QDomNode loginText = loginE.firstChild();
			if(loginText.isText())
			{
			
				login =  loginText.toText().data();
			}
		}
	
		QDomElement nameE = doc.firstChild().firstChildElement("name");
	
		if(!nameE.isNull())
		{
			QDomNode nameText = nameE.firstChild();
			if(nameText.isText())
			{
				name =  nameText.toText().data();
			}
		}
	
		if(!(name.isNull() || login.isNull()))
		{
			removeUser(login);
			
			QTreeWidgetItem *item = new QTreeWidgetItem(tree());
			item->setText(0, login);
			item->setText(1, name);
		}
		pkg->accept();
	}
	else if(pkg->root() == "userlist")
	{
		Packages::UserListParser parser;
		if(parser.parse(pkg->xml()))
		{
			foreach(QStringList values, parser.info())
			{
				int count = 0;
				QTreeWidgetItem *item = new QTreeWidgetItem(tree());
				foreach(QString value, values)
				{
					item->setText(count, value);
					count++;
				}
			}
			setFilled( true);
		}
		pkg->accept();
	}
	else if(pkg->root() == "user")
	{
		Packages::UserParser parser;
		if(parser.parse(pkg->xml()))
		{
			Form *form = new Form(parser.login(), parser.name(), parser.permissions());
			emit postWidget(form);
			connect(form, SIGNAL(sendPackage(const QString &)), this, SIGNAL(sendPackage(const QString &)));
		}
		pkg->accept();
	}
	
	SHOW_VAR(pkg->xml());
}

void ModuleWidget::removeUser(const QString& login)
{
	QTreeWidgetItemIterator it(tree());
	while( (*it) )
	{
		if( login == (*it)->text(0) )
		{
			delete (*it);
			break;
		}
		++it;
	}
}


void ModuleWidget::updateList()
{
	tree()->clear();
	emit sendPackage("<listusers/>");
}

}
