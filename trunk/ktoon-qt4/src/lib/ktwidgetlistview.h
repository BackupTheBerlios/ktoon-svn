/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@gmail.com   *
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

#ifndef KTWIDGETLISTVIEW_H
#define KTWIDGETLISTVIEW_H

#include <qlistview.h>
#include <QVBoxLayout>
#include <QScrollArea>


class KTWidgetListItem;

/**
 * @author David Cuadrado <krawek@gmail.com>
*/

class KTWidgetListView : public QScrollArea
{
	Q_OBJECT
	public:
		KTWidgetListView( QWidget * parent = 0);
		~KTWidgetListView();
		
		void setHeader(QWidget *header);
		
		void addItem(KTWidgetListItem *item);
		
		QWidget *header();
		KTWidgetListItem *itemSelected();
		
	private slots:
		void itemSelect();

	private:
		QWidget *m_header;
		
		QFrame *m_container;
		QVBoxLayout *m_layout;
		
		QList<KTWidgetListItem *> m_items;
		
		KTWidgetListItem *m_itemSelected;

};

#endif
