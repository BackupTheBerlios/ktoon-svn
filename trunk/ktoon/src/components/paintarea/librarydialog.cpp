/***************************************************************************
 *   Project KTOON: 2D Animation Toolkit 0.9                               *
 *   Project Contact: ktoon@toonka.com                                     *
 *   Project Website: http://ktoon.toonka.com                              *
 *   Copyright (C) 2005 by David Cuadrado <krawek@gmail.com>               *
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

LibraryDialog::LibraryDialog() : QDialog(), k(new Private)
{
    QVBoxLayout *layout = new QVBoxLayout(this);

    k->toolBox = new QToolBox;
    layout->addWidget(k->toolBox);

    QDialogButtonBox *buttons = new QDialogButtonBox(QDialogButtonBox::Help | QDialogButtonBox::Ok 
                                | QDialogButtonBox::Cancel, Qt::Horizontal );	
    connect(buttons, SIGNAL(accepted ()), this, SLOT(checkNames()));
    connect(buttons, SIGNAL(rejected ()), this, SLOT(reject()));

    layout->addWidget(buttons);
}

LibraryDialog::~LibraryDialog()
{
    delete k;
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

    QLayout *grid = KFormFactory::makeGrid( QStringList() << tr("Name"), QWidgetList() << name );	
    layout->addLayout(grid);

    int index = k->toolBox->addItem(container, tr("Item %1").arg(k->toolBox->count()+1));
    k->symbolNames.insert(item, name);
    k->tabs.insert(index, name);
}

QString LibraryDialog::symbolName(QGraphicsItem *item) const
{
    return k->symbolNames[item]->text();
}

void LibraryDialog::checkNames()
{
    for (int i = 0; i < k->toolBox->count(); i++) {
         if (k->tabs[i]->text().isEmpty()) {
             k->toolBox->setCurrentIndex (i);
             k->tabs[i]->setFocus();
             return;
         }
    }

    accept();
}
