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

#include "ktframesequencemanager.h"
#include "ktdebug.h"

#include <qlabel.h>

KTFrameSequenceManager::KTFrameSequenceManager(QWidget *parent) : QVBox(parent, "KTFrameSequenceManager")
{
	KTINIT;
	
	m_sequenceLayout = new QScrollView(this);
	m_sequenceLayout->enableClipper( true );
	
	m_vBox = new QVBox(m_sequenceLayout->viewport());
	m_sequenceLayout->addChild(m_vBox);
	
	m_ruler = new KTTLRuler(m_vBox);
	m_ruler->setMinimumSize(300, m_ruler->height());
	
	setMaximumHeight( sizeHint().height() );
	
	m_frameSequence = new KTFrameSequence(m_vBox);
	m_frameSequence->show();
#if 0
	for(uint i =0; i < 30; i++)
	{
		new KTFrameSequence(m_vBox);
	}
#endif
}


KTFrameSequenceManager::~KTFrameSequenceManager()
{
	KTEND;
}

