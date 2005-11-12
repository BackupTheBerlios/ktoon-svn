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

#include "ktdebug.h"

KTWidgetListView::KTWidgetListView(QWidget * parent) : QScrollArea(parent),m_header(0),  m_itemSelected(0)
{
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
	item->setMinimumHeight(20);
	item->setMaximumHeight(20);
	item->setParent(m_container);
	m_layout->addWidget(item, 1, Qt::AlignTop);
	
// 	item->show();
	
	m_items << item;
	
	connect( item, SIGNAL(selected()), this, SLOT(itemSelect()) );
	
	m_container->adjustSize();
	
	if ( m_itemSelected == 0 )
	{
		selectItem( item );
	}
}

void KTWidgetListView::removeItem(KTWidgetListItem *item)
{
	KT_FUNCINFO;
	if ( item )
	{
		int index = m_layout->indexOf(item);
		
		if ( index >= 0)
		{
			m_layout->removeWidget(item);
			
			item->hide();
			
			if ( item == m_itemSelected )
			{
				delete m_itemSelected;
				m_itemSelected = 0;
			}
			else
			{
				delete item;
				item = 0;
			}
			
			
			if ( m_layout->itemAt(index) )
			{
				selectItem( qobject_cast<KTWidgetListItem *>(m_layout->itemAt(index)->widget()) );
			}
		}
		else
		{
			ktError() << "Invalid item";
		}
	}
	else
	{
		ktError() << "Invalid item";
	}
}

void KTWidgetListView::itemSelect()
{
	if ( sender() )
	{
		KTWidgetListItem *itemSelected = qobject_cast<KTWidgetListItem *>(sender());
		selectItem( itemSelected );
	}
}

QWidget *KTWidgetListView::header()
{
	return m_header;
}

KTWidgetListItem *KTWidgetListView::currentItem() const
{
	return m_itemSelected;
}

void KTWidgetListView::selectItem(KTWidgetListItem *item)
{
	KT_FUNCINFO;
	if ( item && item != m_itemSelected && item != m_header )
	{
		QPalette pal = palette();
		
		if ( m_itemSelected != 0)
		{
			ktDebug() << "Break";
			m_itemSelected->setPalette(pal);
		}
		
		m_itemSelected = item;
		
		pal.setBrush(QPalette::Background, pal.highlight());
		pal.setBrush(QPalette::Foreground, pal.base());
		
		m_itemSelected->setPalette(pal);
		emit itemSelected( m_layout->indexOf(item) );
	}
	else
	{
		ktError() << "Invalid item selected";
	}
}

void KTWidgetListView::selectItem(int position)
{
	KT_FUNCINFO;
	
	if ( m_layout->itemAt(position) )
	{
		KTWidgetListItem *item = qobject_cast<KTWidgetListItem *>(m_layout->itemAt(position)->widget());
		if ( item )
		{
			selectItem( item );
		}
	}
}
