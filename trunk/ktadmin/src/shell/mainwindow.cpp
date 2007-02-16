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

#include "mainwindow.h"

#include <QMenuBar>
#include <QMenu>
#include <QStatusBar>
#include <QApplication>
#include <QToolBar>

#include <dtoolview.h>
#include <ddebug.h>
#include <dosd.h>

#include "users/modulewidget.h"
#include "projects/modulewidget.h"
#include "server/modulewidget.h"
#include "bans/modulewidget.h"
#include "backups/modulewidget.h"
#include "registers/modulewidget.h"

#include "manager.h"
#include "connectdialog.h"

struct MainWindow::Private
{
	QList<Base::ModuleWidget *> modules;
	Manager *manager;
};


MainWindow::MainWindow(QWidget *parent)
 : DWorkspaceMainWindow(parent), d(new Private)
{
	setWindowTitle(tr("KToonD Admin"));
	
	d->manager = new Manager(this);
	connect(d->manager, SIGNAL(connected()), this, SLOT(updateModules()));
	connect(d->manager, SIGNAL(disconnected()), this, SLOT(clearModules()));
	
	createModules();
	createMenuBar();
	
	statusBar()->show();
	setCurrentPerspective(DefaultPerspective);
}


MainWindow::~MainWindow()
{
	delete d;
}

void MainWindow::createModules()
{
	Users::ModuleWidget *usersModule = new Users::ModuleWidget;
	addToolView(usersModule, Qt::LeftDockWidgetArea)->setDescription(tr("Administer users"));
	registerModule(usersModule);
	
	Projects::ModuleWidget *projectsModule = new Projects::ModuleWidget;
	addToolView(projectsModule, Qt::LeftDockWidgetArea)->setDescription(tr("Administer projects"));
	registerModule(projectsModule);
	
#if 0
	Server::ModuleWidget *serverModule = new Server::ModuleWidget;
	addToolView(serverModule, Qt::RightDockWidgetArea)->setDescription(tr("Administer server"));
	registerModule(serverModule);
#endif
	
	Bans::ModuleWidget *bansModule = new Bans::ModuleWidget;
	addToolView(bansModule, Qt::RightDockWidgetArea)->setDescription(tr("Administer bans"));
	registerModule(bansModule);
	
	Backups::ModuleWidget *backupsModule = new Backups::ModuleWidget;
	addToolView(backupsModule, Qt::RightDockWidgetArea)->setDescription(tr("Administer backups"));
	registerModule(backupsModule);
	
	Registers::ModuleWidget *registersModule = new Registers::ModuleWidget;
	addToolView(registersModule, Qt::RightDockWidgetArea)->setDescription(tr("Administer registers"));
	registerModule(registersModule);
}

void MainWindow::createMenuBar()
{
	// Create file menu
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	
	QAction *connectAction = fileMenu->addAction(tr("Connect..."), this, SLOT(connectToServer()));
	connectAction->setIcon(QIcon(THEME_DIR+"/icons/connect.png"));
	connectAction->setStatusTip(tr("Connect to server..."));
	
	fileMenu->addSeparator();
	fileMenu->addAction(tr("&Quit"), qApp, SLOT(closeAllWindows()));
	
	
	
	// Add actions to toolbar
	QToolBar *mainToolBar = new QToolBar(this);
	mainToolBar->addAction(connectAction);
	
	
	addToolBar(Qt::TopToolBarArea, mainToolBar);
}


void MainWindow::addWidgetAsWindow(QWidget *w)
{
	if ( d->manager->isEnabled() )
	{
		addWidget(w);
	}
	else
	{
		DOsd::self()->display(tr("Please connect to server"));
	}
}

void MainWindow::registerModule(Base::ModuleWidget *w)
{
	d->manager->addObserver(w);
	d->modules << w;
	
	connect(w, SIGNAL(postWidget(QWidget *)), this, SLOT(addWidgetAsWindow(QWidget *)));
	connect(w, SIGNAL(sendPackage(const QString &)),  d->manager, SLOT(sendPackage(const QString &)));
}

void MainWindow::updateModules()
{
	foreach(Base::ModuleWidget *module, d->modules)
	{
		if( module->isVisible() )
		{
			module->update();
		}
	}
}

void MainWindow::clearModules()
{
	foreach(Base::ModuleWidget *module, d->modules)
	{
		module->clear();
	}
}

void MainWindow::connectToServer()
{
	ConnectDialog cnndialog;
	
	if ( cnndialog.exec() != QDialog::Accepted )
	{
		return;
	}
	
	if ( d->manager->connectToServer(cnndialog.server(), cnndialog.port()) )
	{
		d->manager->authenticate(cnndialog.login(), cnndialog.password());
	}
	
}

