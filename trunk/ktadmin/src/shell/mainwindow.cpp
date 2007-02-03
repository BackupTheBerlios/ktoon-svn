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

#include "manager.h"
#include "connectdialog.h"

struct MainWindow::Private
{
	Users::ModuleWidget *usersModule;
	Projects::ModuleWidget *projectsModule;
	Server::ModuleWidget *serverModule;
	Bans::ModuleWidget *bansModule;
	
	Manager *manager;
};


MainWindow::MainWindow(QWidget *parent)
 : DWorkspaceMainWindow(parent), d(new Private)
{
	setWindowTitle(tr("KToonD Admin"));
	
	d->manager = new Manager(this);
	connect(d->manager, SIGNAL(connected()), this, SLOT(updateModules()));
	
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
	d->usersModule = new Users::ModuleWidget;
	addToolView(d->usersModule, Qt::LeftDockWidgetArea)->setDescription(tr("Administer users"));
	setupModule(d->usersModule);
	
	d->projectsModule = new Projects::ModuleWidget;
	addToolView(d->projectsModule, Qt::LeftDockWidgetArea)->setDescription(tr("Administer projects"));
	setupModule(d->projectsModule);
	
	d->serverModule = new Server::ModuleWidget;
	addToolView(d->serverModule, Qt::RightDockWidgetArea)->setDescription(tr("Administer server"));
	setupModule(d->serverModule);
	
	d->bansModule = new Bans::ModuleWidget;
	addToolView(d->bansModule, Qt::RightDockWidgetArea)->setDescription(tr("Administer bans"));
	setupModule(d->bansModule);
	
	d->manager->addObserver(d->usersModule);
	d->manager->addObserver(d->projectsModule);
	d->manager->addObserver(d->serverModule);
	d->manager->addObserver(d->bansModule);
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

void MainWindow::setupModule(const QWidget *w)
{
	connect(w, SIGNAL(postWidget(QWidget *)), this, SLOT(addWidgetAsWindow(QWidget *)));
	connect(w, SIGNAL(sendPackage(const QString &)),  d->manager, SLOT(sendPackage(const QString &)));
}

void MainWindow::updateModules()
{
	if ( d->usersModule->isVisible() )
	{
		d->usersModule->update();
	}
	
	if ( d->projectsModule->isVisible() )
	{
		d->projectsModule->update();
	}
	
	if ( d->serverModule->isVisible() )
	{
		d->serverModule->update();
	}
	
	if ( d->bansModule->isVisible() )
	{
		d->bansModule->update();
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

