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

#include <dseparator.h>

namespace Base {

struct Form::Private
{
	QLabel *title;
	QWidget *center;
	QDialogButtonBox *buttons;
	QVBoxLayout *layout;
	
};

Form::Form(const QString &title, QWidget *parent) : QFrame(parent), DFormValidator(this), d(new Private)
{
	setAttribute(Qt::WA_DeleteOnClose);
	
	
	d->layout = new QVBoxLayout(this);
	d->layout->setSpacing(2);
	d->title = new QLabel;
	setTitle(title);
	
	d->layout->addWidget(d->title);
	d->layout->addWidget(new DSeparator());
	
	d->center = new QWidget();
	d->layout->addWidget(d->center);
	
	d->layout->addWidget(new DSeparator());
	
	d->buttons = new QDialogButtonBox();
	
	QPushButton *ok = d->buttons->addButton(QDialogButtonBox::Ok);
	connect(ok, SIGNAL(clicked()), this, SLOT(_acceptForm()));
	
	QPushButton *cancel = d->buttons->addButton(QDialogButtonBox::Cancel);
	connect(cancel, SIGNAL(clicked()), this, SLOT(close()));
	
	QPushButton *reset = d->buttons->addButton(QDialogButtonBox::Reset);
	connect(reset, SIGNAL(clicked()), this, SLOT(resetForm()));
	
	QPushButton *help = d->buttons->addButton(QDialogButtonBox::Help);
	connect(help, SIGNAL(clicked()), this, SLOT(showHelp()));
	
	d->layout->addWidget(d->buttons);
}

Form::~Form()
{
	
}


void Form::setCentralWidget(QWidget *widget)
{
	d->center->hide();
	d->layout->removeWidget ( d->center );
	d->layout->insertWidget(2, widget);
	if(d->center)
	{
		delete d->center;
	}
	d->center = widget;
	setForm(widget);
}

void Form::setButtons(QDialogButtonBox::StandardButtons buttons)
{
	d->buttons->setStandardButtons (  buttons );
}

void Form::setTitle( const QString &title)
{
	d->title->setText("<h3>"+title+"</h3>");
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

