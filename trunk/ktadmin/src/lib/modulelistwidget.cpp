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

#include <kgui/ktreelistwidget.h>
#include <kgui/ktreewidgetsearchline.h>
#include <kcore/kdebug.h>

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
	KTreeWidgetSearchLine *search;
	ModuleButtonBar *buttonBar;
	
	bool filled;
	bool clearOnUpdate;
};

ModuleListWidget::ModuleListWidget(ModuleButtonBar::Buttons buttons, QWidget *parent) : ModuleWidget(parent), k(new Private)
{
	k->filled = false;
	k->clearOnUpdate = true;
	
	QVBoxLayout *layout = new QVBoxLayout(this);
	layout->setMargin(2);
	layout->setSpacing(1);
	
	QHBoxLayout *search = new QHBoxLayout;
	search->setSpacing(2);

	QToolButton *button = new QToolButton;
	button->setIcon( QIcon(THEME_DIR+"/icons/clear_right.png"));
	
	search->addWidget(button);
	k->tree = new QTreeWidget;
	k->tree->setSortingEnabled(true);
	k->tree->setDragEnabled(true);
	
	k->tree->setContextMenuPolicy(Qt::CustomContextMenu);
	
	connect(k->tree, SIGNAL(customContextMenuRequested( const QPoint& )), this, SLOT(showMenu(const QPoint &)));
	connect(k->tree, SIGNAL(itemActivated( QTreeWidgetItem*, int )), this, SLOT(defaultBehavior(QTreeWidgetItem*, int)));
	
	k->tree->header()->hide();
	
	
	k->search = new KTreeWidgetSearchLine(this,k->tree);
	search->addWidget( k->search );
	connect(button, SIGNAL(clicked()), k->search, SLOT(clear()));
	
	layout->addLayout(search);
	layout->addWidget(k->tree);
	
	k->buttonBar = new Base::ModuleButtonBar(buttons);
	layout->addWidget(k->buttonBar);
	connect(k->buttonBar, SIGNAL(buttonClicked( int )), this, SLOT(actionSelected(int)));
}


ModuleListWidget::~ModuleListWidget()
{
	delete k;
}

void ModuleListWidget::setHeaders(const QStringList &headers)
{
	k->tree->setHeaderLabels(headers);
// 	k->tree->header()->setSortIndicatorShown(true);
	
	k->tree->header()->show();
}

QTreeWidget *ModuleListWidget::tree() const
{
	return k->tree;
}

QMenu *ModuleListWidget::createMenu()
{
	QMenu *menu = new QMenu(this);
	
	if ( k->buttonBar->hasButton(ModuleButtonBar::Add) )
	{
		QAction *add = menu->addAction(tr("Add"), this, SLOT(selectFromAction()));
		add->setData(ModuleButtonBar::Add);
	}
	
	if ( k->buttonBar->hasButton(ModuleButtonBar::Del) )
	{
		QAction *del = menu->addAction(tr("Delete"), this, SLOT(selectFromAction()));
		del->setData(ModuleButtonBar::Del);
	}
	
	if ( k->buttonBar->hasButton(ModuleButtonBar::Query) )
	{
		QAction *query = menu->addAction(tr("Query"), this, SLOT(selectFromAction()));
		query->setData(ModuleButtonBar::Query);
	}
	
	if ( k->buttonBar->hasButton(ModuleButtonBar::Modify) )
	{
		QAction *modify = menu->addAction(tr("Modify"), this, SLOT(selectFromAction()));
		modify->setData(ModuleButtonBar::Modify);
	}
	
	
	return menu;
}

void ModuleListWidget::setClearOnUpdate(bool cou)
{
	k->clearOnUpdate = cou;
}

bool ModuleListWidget::clearOnUpdate() const
{
	return k->clearOnUpdate;
}

void ModuleListWidget::addItem(const QStringList &itemData)
{
	QTreeWidgetItem *item = new QTreeWidgetItem(k->tree);
	int col = 0;
	
	foreach(QString data, itemData)
	{
		item->setText(col, data);
		col++;
	}
}

void ModuleListWidget::actionSelected(int action)
{
	QTreeWidgetItem *currentItem = k->tree->currentItem();
	
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
	
	menu->exec( k->tree->mapToGlobal( pos) );
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
	if ( !k->filled )
		update();
	ModuleWidget::showEvent(e);
}

void ModuleListWidget::setFilled(bool f)
{
	k->filled = f;
}

bool ModuleListWidget::filled() const
{
	return k->filled;
}

void ModuleListWidget::update()
{
	if( k->clearOnUpdate )
		k->tree->clear();
	updateList();
}

void ModuleListWidget::clear()
{
	k->filled = false;
	k->tree->clear();
}

ModuleButtonBar *ModuleListWidget::buttonBar() const
{
	return k->buttonBar;
}

void ModuleListWidget::defaultBehavior(QTreeWidgetItem *item, int)
{
	modifyActionSelected(item);
}

}


