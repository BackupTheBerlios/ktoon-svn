/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado                                  *
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

#include "ktframesequencecontainer.h"
#include "delabel.h"

#include "ddebug.h"

#include <QIntValidator>
#include <QHeaderView>

KTFrameSequenceContainer::KTFrameSequenceContainer(QWidget *parent) : DVHBox(parent, Qt::Vertical )
{
	layout()->setSpacing(0);
	layout()->setMargin(0);
	
	m_frameTable = new TFramesTable(this);
		
	connect(m_frameTable, SIGNAL(itemClicked( TFramesTableItem* )), this, SLOT(selectFrameFromItem(TFramesTableItem *)));
	connect(m_frameTable, SIGNAL(itemEntered( TFramesTableItem* )), this, SLOT(selectFrameFromItem(TFramesTableItem *)));
	
// 	connect(m_frameTable, SIGNAL(itemSelectionChanged()), this, SLOT(selectCurrentFrame()));
}


KTFrameSequenceContainer::~KTFrameSequenceContainer()
{
}

TFramesTable const *KTFrameSequenceContainer::manager()
{
	return m_frameTable;
}

void KTFrameSequenceContainer::addLayer()
{
	D_FUNCINFO;

	m_frameTable->addLayer();
}

void KTFrameSequenceContainer::addFrameToLayer(int layerPos)
{
// 	D_FUNCINFO;
	m_frameTable->addFrame( layerPos );
}


void KTFrameSequenceContainer::selectFrameFromItem(TFramesTableItem *item)
{
// 	D_FUNCINFO;
	emit frameSelected( m_frameTable->verticalHeader()->logicalIndex(m_frameTable->row(item)), m_frameTable->column(item));
}

void KTFrameSequenceContainer::selectCurrentFrame()
{
	D_FUNCINFO;
	TFramesTableItem *item = m_frameTable->selectedItems()[0];
	if ( item != m_frameTable->currentItem())
	{
		m_frameTable->setCurrentFrame( item );
		emit frameSelected( m_frameTable->row(item), m_frameTable->column(item));
	}
}

void KTFrameSequenceContainer::removeCurrentLayer()
{
	D_FUNCINFO;
	m_frameTable->removeCurrentLayer();
}

void KTFrameSequenceContainer::removeLayer(int pos)
{
	m_frameTable->removeLayer( pos );
}

void KTFrameSequenceContainer::selectLayer(int pos)
{
	m_frameTable->setCurrentLayer( pos );
}

void KTFrameSequenceContainer::selectCell(int layer, int frame)
{
// 	m_frameTable->setCurrentItem(  m_frameTable->item( m_frameTable->verticalHeader()->logicalIndex(layer), frame) );
	m_frameTable->selectCell( layer, frame );
}


void KTFrameSequenceContainer::moveCurrentLayer(bool up)
{
	int index = m_frameTable->verticalHeader()->visualIndex(m_frameTable->currentRow());
	
	if ( up )
	{
		if ( index > 0 )
		{
			m_frameTable->verticalHeader()->moveSection(index, index-1);
		}
	}
	else
	{
		if ( index < m_frameTable->rowCount() )
		{
			m_frameTable->verticalHeader()->moveSection(index, index+1);
		}
	}
}



