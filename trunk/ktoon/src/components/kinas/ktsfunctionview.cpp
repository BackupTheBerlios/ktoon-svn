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

#include "ktsfunctionview.h"
#include <QTreeWidgetItem>
#include <QHeaderView>
#include <QItemDelegate>

KTSFunctionView::KTSFunctionView(QWidget *parent) : QTreeWidget(parent)
{
    setItemDelegate(new QItemDelegate(this));
    setColumnCount(1);
    setHeaderLabels(QStringList() << "Members");
    setRootIsDecorated (true);

    header()->setResizeMode(QHeaderView::Stretch);

    setEditTriggers(QAbstractItemView::AnyKeyPressed);

    addModule("A Module", "A section", QStringList() << "Function1" << "Function2" << "Function3" << "Function4" );

    setDragEnabled ( true);
}

KTSFunctionView::~KTSFunctionView()
{
}

void KTSFunctionView::addModule(const QString &module, const QString &section, QStringList functions)
{
    // TODO: mirar si los modulos y secciones existen, sino crearlos

    QTreeWidgetItem *itemModule = new QTreeWidgetItem(this);
    itemModule->setText(0, module);

    QTreeWidgetItem *itemSection = new QTreeWidgetItem(itemModule);
    itemSection->setText(0, section);

    QStringListIterator iterator(functions);

    while (iterator.hasNext()) {
           QTreeWidgetItem *itemFunction = new QTreeWidgetItem(itemSection);
           itemFunction->setText(0, iterator.next());
    }
}

bool KTSFunctionView::dropMimeData ( QTreeWidgetItem * parent, int index, const QMimeData * data, Qt::DropAction action )
{
    return true;
}
