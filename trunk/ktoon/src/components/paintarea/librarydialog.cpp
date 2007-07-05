/***************************************************************************
 *   Copyright (C) 2006 by David Cuadrado                                  *
 *   krawek@toonka.com                                                     *
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

#include "librarydialog.h"

#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QToolBox>
#include <QGraphicsItem>
#include <QHBoxLayout>
#include <QLineEdit>

#include "ktitempreview.h"
#include <kgui/kformfactory.h>
#include <QMap>


struct LibraryDialog::Private
{
		QToolBox *toolBox;
		QMap<QGraphicsItem *, QLineEdit *> symbolNames;
		QMap<int, QLineEdit *> tabs;
};

LibraryDialog::LibraryDialog()
	: QDialog(), d(new Private)
{
	QVBoxLayout *layout = new QVBoxLayout(this);
	
	
	d->toolBox = new QToolBox;
	
	
	layout->addWidget(d->toolBox);
	
	QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Help | QDialogButtonBox::Ok | QDialogButtonBox::Cancel, Qt::Horizontal );
	
	
	connect(buttons, SIGNAL(accepted ()), this, SLOT(checkNames()));
	connect(buttons, SIGNAL(rejected ()), this, SLOT(reject()));
	
	layout->addWidget(buttons);
}


LibraryDialog::~LibraryDialog()
{
	delete d;
}

void LibraryDialog::addItem(QGraphicsItem *item)
{
	KTItemPreview *preview = new KTItemPreview;
	
	preview->render( item );
	
	QWidget *container = new QWidget;
	
	QVBoxLayout *layout = new QVBoxLayout(container);
	layout->addWidget(preview);
	
	QLineEdit *name = new QLineEdit;
	connect(name, SIGNAL(returnPressed()), this, SLOT(checkNames()));
	
	QLayout *grid = DFormFactory::makeGrid( QStringList() << tr("Name"), QWidgetList() << name );
	
	layout->addLayout(grid);
	
	int index = d->toolBox->addItem(container, tr("Item %1").arg(d->toolBox->count()+1));
	
	d->symbolNames.insert(item, name);
	d->tabs.insert(index, name);
}

QString LibraryDialog::symbolName(QGraphicsItem *item) const
{
	return d->symbolNames[item]->text();
}

void LibraryDialog::checkNames()
{
	for(int i = 0; i < d->toolBox->count(); i++)
	{
		if ( d->tabs[i]->text().isEmpty())
		{
			d->toolBox->setCurrentIndex (i);
			d->tabs[i]->setFocus();
			return;
		}
	}
	
	accept();
}



