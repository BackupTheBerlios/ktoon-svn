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

#include "modulelistwidget.h"

#include <dtreelistwidget.h>
#include <dtreewidgetsearchline.h>
#include <ddebug.h>

#include <QToolButton>
#include <QTreeWidget>
#include <QTreeWidgetItem>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>
#include <QPushButton>
#include <QMenu>

namespace Base {

struct ModuleListWidget::Private
{
	QTreeWidget *tree;
	DTreeWidgetSearchLine *search;
	ModuleButtonBar *buttonBar;
	
	bool filled;
	bool clearOnUpdate;
};

ModuleListWidget::ModuleListWidget(ModuleButtonBar::Buttons buttons, QWidget *parent) : ModuleWidget(parent), d(new Private)
{
	d->filled = false;
	d->clearOnUpdate = true;
	
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setMargin(2);
	layout->setSpacing(1);
	
	QHBoxLayout *search = new QHBoxLayout;
	search->setSpacing(2);

	QToolButton *button = new QToolButton;
	button->setIcon( QIcon(THEME_DIR+"/icons/clear_right.png"));
	
	search->addWidget(button);
	d->tree = new QTreeWidget;
	d->tree->setSortingEnabled(true);
	d->tree->setDragEnabled(true);
	
	d->tree->setContextMenuPolicy(Qt::CustomContextMenu);
	
	connect(d->tree, SIGNAL(customContextMenuRequested( const QPoint& )), this, SLOT(showMenu(const QPoint &)));
	connect(d->tree, SIGNAL(itemActivated( QTreeWidgetItem*, int )), this, SLOT(defaultBehavior(QTreeWidgetItem*, int)));
	
	d->tree->header()->hide();
	
	
	d->search = new DTreeWidgetSearchLine(this,d->tree);
	search->addWidget( d->search );
	connect(button, SIGNAL(clicked()), d->search, SLOT(clear()));
	
	layout->addLayout(search);
	layout->addWidget(d->tree);
	
	d->buttonBar = new Base::ModuleButtonBar(buttons);
	layout->addWidget(d->buttonBar);
	connect(d->buttonBar, SIGNAL(buttonClicked( int )), this, SLOT(actionSelected(int)));
}


ModuleListWidget::~ModuleListWidget()
{
	delete d;
}

void ModuleListWidget::setHeaders(const QStringList &headers)
{
	d->tree->setHeaderLabels(headers);
// 	d->tree->header()->setSortIndicatorShown(true);
	
	d->tree->header()->show();
}

QTreeWidget *ModuleListWidget::tree() const
{
	return d->tree;
}

QMenu *ModuleListWidget::createMenu()
{
	QMenu *menu = new QMenu(this);
	
	if ( d->buttonBar->hasButton(ModuleButtonBar::Add) )
	{
		QAction *add = menu->addAction(tr("Add"), this, SLOT(selectFromAction()));
		add->setData(ModuleButtonBar::Add);
	}
	
	if ( d->buttonBar->hasButton(ModuleButtonBar::Del) )
	{
		QAction *del = menu->addAction(tr("Delete"), this, SLOT(selectFromAction()));
		del->setData(ModuleButtonBar::Del);
	}
	
	if ( d->buttonBar->hasButton(ModuleButtonBar::Query) )
	{
		QAction *query = menu->addAction(tr("Query"), this, SLOT(selectFromAction()));
		query->setData(ModuleButtonBar::Query);
	}
	
	if ( d->buttonBar->hasButton(ModuleButtonBar::Modify) )
	{
		QAction *modify = menu->addAction(tr("Modify"), this, SLOT(selectFromAction()));
		modify->setData(ModuleButtonBar::Modify);
	}
	
	
	return menu;
}

void ModuleListWidget::setClearOnUpdate(bool cou)
{
	d->clearOnUpdate = cou;
}

bool ModuleListWidget::clearOnUpdate() const
{
	return d->clearOnUpdate;
}

void ModuleListWidget::actionSelected(int action)
{
	QTreeWidgetItem *currentItem = d->tree->currentItem();
	
	switch(action)
	{
		case ModuleButtonBar::Add:
		{
			addActionSelected(currentItem);
		}
		break;
		case ModuleButtonBar::Del:
		{
			delActionSelected(currentItem);
		}
		break;
		case ModuleButtonBar::Query:
		{
			queryActionSelected(currentItem);
		}
		break;
		case ModuleButtonBar::Modify:
		{
			modifyActionSelected(currentItem);
		}
		break;
		case ModuleButtonBar::Custom1:
		{
			custom1ActionSelected(currentItem);
		}
		break;
		case ModuleButtonBar::Custom2:
		{
			custom2ActionSelected(currentItem);
		}
		break;
		default: break;
	}
}

void ModuleListWidget::selectFromAction()
{
	QAction *sender = static_cast<QAction *>(this->sender());
	
	if( sender )
	{
		int action = sender->data().toInt();
		actionSelected(action);
	}
}

void ModuleListWidget::showMenu(const QPoint &pos)
{
	QMenu *menu = createMenu();
	
	menu->exec( d->tree->mapToGlobal( pos) );
}

void ModuleListWidget::addActionSelected(QTreeWidgetItem *current)
{
	Q_UNUSED(current);
}


void ModuleListWidget::delActionSelected(QTreeWidgetItem *current)
{
	Q_UNUSED(current);
}


void ModuleListWidget::queryActionSelected(QTreeWidgetItem *current)
{
	Q_UNUSED(current);
}


void ModuleListWidget::modifyActionSelected(QTreeWidgetItem *current)
{
	Q_UNUSED(current);
}

void ModuleListWidget::custom1ActionSelected(QTreeWidgetItem *current)
{
	Q_UNUSED(current);
}

void ModuleListWidget::custom2ActionSelected(QTreeWidgetItem *current)
{
	Q_UNUSED(current);
}

void ModuleListWidget::showEvent(QShowEvent *e)
{
	if ( !d->filled )
		update();
	ModuleWidget::showEvent(e);
}

void ModuleListWidget::setFilled(bool f)
{
	d->filled = f;
}

bool ModuleListWidget::filled() const
{
	return d->filled;
}

void ModuleListWidget::update()
{
	if( d->clearOnUpdate )
		d->tree->clear();
	updateList();
}

ModuleButtonBar *ModuleListWidget::buttonBar() const
{
	return d->buttonBar;
}

void ModuleListWidget::defaultBehavior(QTreeWidgetItem *item, int)
{
	modifyActionSelected(item);
}

}


