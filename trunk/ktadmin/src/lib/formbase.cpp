/***************************************************************************
 *   Copyright (C) 2007 by Jorge Cuadrado                                  *
 *   kuadrosxx@gmail.com                                                   *
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

#include "formbase.h"

#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDragEnterEvent>
#include <QMimeData>
#include <QDataStream>
#include <QAbstractItemModel>
#include <QTreeWidgetItem>
#include <QTreeWidget>
#include <QModelIndex>

#include <kgui/kseparator.h>

namespace Base {

struct Form::Private
{
	QLabel *title;
	QWidget *center;
	QDialogButtonBox *buttons;
	QVBoxLayout *layout;
	
};

Form::Form(const QString &title, QWidget *parent) : QFrame(parent), KFormValidator(this), k(new Private)
{
	setAttribute(Qt::WA_DeleteOnClose);
	
	
	k->layout = new QVBoxLayout(this);
	k->layout->setSpacing(2);
	k->title = new QLabel;
	setTitle(title);
	
	k->layout->addWidget(k->title);
	k->layout->addWidget(new KSeparator());
	
	k->center = new QWidget();
	k->layout->addWidget(k->center);
	
	k->layout->addWidget(new KSeparator());
	
	k->buttons = new QDialogButtonBox();
	
	QPushButton *ok = k->buttons->addButton(QDialogButtonBox::Ok);
	connect(ok, SIGNAL(clicked()), this, SLOT(_acceptForm()));
	
	QPushButton *cancel = k->buttons->addButton(QDialogButtonBox::Cancel);
	connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
	
	QPushButton *reset = k->buttons->addButton(QDialogButtonBox::Reset);
	connect(reset, SIGNAL(clicked()), this, SLOT(resetForm()));
	
	QPushButton *help = k->buttons->addButton(QDialogButtonBox::Help);
	connect(help, SIGNAL(clicked()), this, SLOT(showHelp()));
	
	k->layout->addWidget(k->buttons);
}

Form::~Form()
{
	
}


void Form::setCentralWidget(QWidget *widget)
{
	k->center->hide();
	k->layout->removeWidget ( k->center );
	k->layout->insertWidget(2, widget);
	if(k->center)
	{
		delete k->center;
	}
	k->center = widget;
	setForm(widget);
}

void Form::setButtons(QDialogButtonBox::StandardButtons buttons)
{
	k->buttons->setStandardButtons (  buttons );
}

void Form::setTitle( const QString &title)
{
	k->title->setText("<h3>"+title+"</h3>");
}

void Form::showHelp()
{
}

/**
 * @internal
 */
void Form::_acceptForm()
{
	if ( validate() )
	{
		acceptForm();
	}
}


void Form::dragEnterEvent(QDragEnterEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
	{
		event->acceptProposedAction();
	}
}

void Form::dropEvent(QDropEvent *event)
{
	if (event->mimeData()->hasFormat("application/x-qabstractitemmodeldatalist"))
	{
		QTreeWidget *tree = dynamic_cast<QTreeWidget *>(event->source());
		
		
		QByteArray itemData = event->mimeData()->data("application/x-qabstractitemmodeldatalist");
		QDataStream stream(&itemData, QIODevice::ReadOnly);
		
		int r, c;
		QMap<int, QVariant> v;
		stream >> r >> c >> v;
		
		QTreeWidgetItem *item = tree->topLevelItem(r);
		
		if( item )
		{
			itemDropped(item);
		}
	}
}

void Form::itemDropped(QTreeWidgetItem *item)
{
	Q_UNUSED(item);
}

}

