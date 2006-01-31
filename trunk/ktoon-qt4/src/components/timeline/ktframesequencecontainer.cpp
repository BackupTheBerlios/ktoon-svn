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
#include "ktelabel.h"

#include "ktdebug.h"

#include <QIntValidator>

KTFrameSequenceContainer::KTFrameSequenceContainer(QWidget *parent) : KTVHBox(parent, Qt::Vertical )
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

TFramesTable *KTFrameSequenceContainer::manager()
{
	return m_frameTable;
}

void KTFrameSequenceContainer::addLayer()
{
	KT_FUNCINFO;
	int layerId = 0;
	if ( !m_lastUsedFrame.isEmpty() )
	{
		layerId = m_lastUsedFrame[m_lastUsedFrame.count()-1]+1;
	}
	SHOW_VAR(layerId);
	
	m_lastUsedFrame.insert(layerId, 0);
}

void KTFrameSequenceContainer::addFrameToLayer(int layerId)
{
	KT_FUNCINFO;
	SHOW_VAR(layerId);
	m_frameTable->setAttribute( layerId, m_lastUsedFrame[layerId], TFramesTableItem::IsUsed, true );
	
// 	m_lastUsedFrame++;
	m_lastUsedFrame[layerId] = m_lastUsedFrame[layerId]+1;
}


void KTFrameSequenceContainer::selectFrameFromItem(TFramesTableItem *item)
{
	KT_FUNCINFO;
}

void KTFrameSequenceContainer::removeCurrentLayer()
{
	
}



