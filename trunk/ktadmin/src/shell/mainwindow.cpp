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

#include <dtoolview.h>
#include <ddebug.h>

#include "users/modulewidget.h"
#include "projects/modulewidget.h"
#include "server/modulewidget.h"

#include "manager.h"

struct MainWindow::Private
{
	Users::ModuleWidget *usersModule;
	Projects::ModuleWidget *projectsModule;
	Server::ModuleWidget *serverModule;
	
	Manager *manager;
};


MainWindow::MainWindow(QWidget *parent)
 : DWorkspaceMainWindow(parent), d(new Private)
{
	setWindowTitle(tr("KToonD Admin"));
	
	d->manager = new Manager(this);
	
	createModules();
	createMenuBar();
	
	statusBar()->show();
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
	
	d->serverModule = new Server::ModuleWidget;
	addToolView(d->serverModule, Qt::RightDockWidgetArea)->setDescription(tr("Administer server"));
	
	d->manager->addObserver(d->usersModule);
	d->manager->addObserver(d->projectsModule);
	d->manager->addObserver(d->serverModule);
}

void MainWindow::createMenuBar()
{
	QMenu *fileMenu = menuBar()->addMenu(tr("&File"));
	
	fileMenu->addSeparator();
	fileMenu->addAction(tr("&Quit"), qApp, SLOT(closeAllWindows()));
	
}


void MainWindow::addWidgetAsWindow(QWidget *w)
{
	addWidget(w);
}

void MainWindow::setupModule(const QWidget *w)
{
	connect(w, SIGNAL(postWidget(QWidget *)), this, SLOT(addWidgetAsWindow(QWidget *)));
// 	connect(d->manager, 
}

