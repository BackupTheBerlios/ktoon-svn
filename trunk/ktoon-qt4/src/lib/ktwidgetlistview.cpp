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
#include "ktwidgetlistitem.h"

KTWidgetListView::KTWidgetListView(QWidget * parent) : QScrollArea(parent),m_header(0),  m_itemSelected(0)
{
// 	setVerticalScrollBarPolicy (Qt::ScrollBarAlwaysOn);
	m_container = new QFrame;
	
	m_layout = new QVBoxLayout(m_container);
	m_layout->setMargin(1);
	m_layout->setSpacing(2);
	
	m_container->setLayout(m_layout);

	QPalette pal = palette();
	
// 	pal.setBrush(QPalette::Background, pal.button()  );
// 	pal.setBrush(QPalette::Foreground,  );
	
	m_container->setPalette(pal);
	
	setWidget(m_container);
	
	setWidgetResizable(true);
// 	m_container->resize(sizeHint());
	
	m_container->show();
}


KTWidgetListView::~KTWidgetListView()
{
}

void KTWidgetListView::setHeader(QWidget *header)
{
	header->setParent(m_container);
	m_layout->insertWidget(0, header, 0, Qt::AlignTop);
	
	QPalette pal = palette();
	pal.setBrush(QPalette::Background, pal.base() );
	pal.setBrush(QPalette::Foreground, pal.highlightedText ()  );
	
	m_header = header;
	
	m_header->setPalette(pal);
	
	m_container->adjustSize();
}

void KTWidgetListView::addItem(KTWidgetListItem *item)
{	
	item->setParent(m_container);
	m_layout->addWidget(item, 1, Qt::AlignTop);
	
	item->show();
	
	m_items<<item;
	
	connect(item, SIGNAL(selected()), this, SLOT(itemSelect()));
	
	m_container->adjustSize();
}

void KTWidgetListView::itemSelect()
{
	KTWidgetListItem *itemSelected = qobject_cast<KTWidgetListItem *>(sender());
	
	if ( itemSelected && itemSelected != m_header )
	{
		QPalette pal = palette();
		
		if ( m_itemSelected )
		{
			m_itemSelected->setPalette(pal);
		}
		m_itemSelected = itemSelected;
		
		pal.setBrush(QPalette::Background, pal.highlight());
		pal.setBrush(QPalette::Foreground, pal.base());
		
		m_itemSelected->setPalette(pal);
	}
}

QWidget *KTWidgetListView::header()
{
	return m_header;
}

KTWidgetListItem *KTWidgetListView::itemSelected()
{
	return m_itemSelected;
}
