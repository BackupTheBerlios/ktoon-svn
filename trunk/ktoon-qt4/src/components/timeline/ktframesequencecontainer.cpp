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

KTFrameSequenceContainer::KTFrameSequenceContainer(QWidget *parent) : DVHBox(parent, Qt::Vertical )
{
	layout()->setSpacing(0);
	layout()->setMargin(0);
	
	m_ruler = new KTTLRuler(this);
	m_ruler->setMaximumHeight(20);
	m_ruler->setMinimumHeight(20);
	
	m_frameTable = new TFramesTable(this);
	connect(m_frameTable, SIGNAL(itemClicked( TFramesTableItem* )), this, SLOT(selectFrameFromItem(TFramesTableItem *)));
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
	D_FUNCINFO;
	m_frameTable->addFrame( layerPos );
}


void KTFrameSequenceContainer::selectFrameFromItem(TFramesTableItem *item)
{
	D_FUNCINFO;
	emit frameSelected( m_frameTable->row(item), m_frameTable->column(item));
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
	m_frameTable->setCurrentItem(  m_frameTable->item(layer, frame) );
}

