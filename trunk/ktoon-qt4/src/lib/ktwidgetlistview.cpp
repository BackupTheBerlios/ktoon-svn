/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "ktwidgetlistview.h"
#include <QHeaderView>
#include <QResizeEvent>
#include "ktdebug.h"

KTWidgetListView::KTWidgetListView(QWidget * parent) : QTableWidget(0,1,parent)
{
	verticalHeader()->hide();
	horizontalHeader()->hide();
	
	horizontalHeader()->setResizeMode ( QHeaderView::Custom);
// 	horizontalHeader()->setStretchLastSection(true);
}


KTWidgetListView::~KTWidgetListView()
{
}

QTableWidgetItem *KTWidgetListView::addWidget(QWidget *widget)
{
	QTableWidgetItem *newItem = new QTableWidgetItem();
	
	int newRowIndex = rowCount();
	
	insertRow(newRowIndex);
	setItem( newRowIndex, 0, newItem);
	#if QT_VERSION >= 0x040100
	setIndexWidget(indexFromItem(newItem), widget);
	#endif
	verticalHeader()->resizeSection(newRowIndex, widget->height());
	
	m_items.insert(widget, newItem);

	return newItem;
}

QTableWidgetItem *KTWidgetListView::insertWidget(int pos, QWidget *widget)
{
	QTableWidgetItem *newItem = new QTableWidgetItem();
	
	insertRow(pos);
	setItem( pos, 0, newItem);
	
	setIndexWidget(indexFromItem(newItem), widget);
	verticalHeader()->resizeSection(pos, widget->height());
	
	m_items.insert(widget, newItem);

	return newItem;
}


QWidget *KTWidgetListView::widget(QTableWidgetItem *treeItem)
{
	return indexWidget(indexFromItem(treeItem));
}

QTableWidgetItem *KTWidgetListView::item(QWidget *widget)
{
	return m_items[widget];
}

void KTWidgetListView::resizeEvent(QResizeEvent *e)
{
	horizontalHeader()->resizeSection (0, e->size().width() );
}


void KTWidgetListView::moveItemUp(int index)
{
	if ( index > 0 )
	{
		verticalHeader()->moveSection(index, index-1);
// 		setCurrentCell(index-1, 0);
// 		selectRow(index);
	}
}

void KTWidgetListView::moveItemDown(int index)
{
	if ( index < rowCount() )
	{
		verticalHeader()->moveSection(index, index+1);
// 		setCurrentCell(index+1, 0);
// 		selectRow(index);
	}
}


int KTWidgetListView::currentVisualRow() const
{
	return verticalHeader()->visualIndex(currentRow());
}

