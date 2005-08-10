/***************************************************************************
 *   Copyright (C) 2005 by David Cuadrado   *
 *   krawek@toonka.com   *
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
#include "ktlayerexposure.h"

KTLayerExposure::KTLayerExposure(const QString &initial_text, int id, int numFrame, QWidget *parent, const char *name)
	: QFrame(parent, name) , m_id(id), m_currentFrame(0)
{
	m_layout = new QBoxLayout(this, QBoxLayout::TopToBottom);
	m_header = new ESLayer(initial_text,this);
	connect( m_header, SIGNAL(clicked()), this, SLOT(setSelected()));
	m_layout->addWidget(m_header);
	
	for(int i = 0; i < numFrame; i++)
	{
		ESFrame *frame = new ESFrame( tr( "Drawing " ) + QString::number( m_id ) + QString( "-" ) + QString::number( i + 1 ), i , this);
		m_layout->addWidget(frame);
		m_frames.append(frame);
		connect( frame, SIGNAL(clicked(int, int, int, int )), this, SLOT(frameSelect(int, int, int, int)));
		connect(this, SIGNAL(frameSelected(int )), frame, SLOT(otherSelected(int)));
	}
	m_useFrame = 0;
	m_frames[0]->setUsed( true );
	frameSelect(0,0,0,0);
}

KTLayerExposure::~KTLayerExposure()
{
	
}

void KTLayerExposure::frameSelect(int id, int button, int x, int y)
{
	m_currentFrame = id;
	setSelected(true);
	emit frameSelected(id);
	emit clicked(id, m_id, button, x, y);
}

void KTLayerExposure::setSelected(bool select)
{
	m_selected = select;
	m_header->setSelected(select);
	m_header->slotSetDescription();
	if(select)
	{
		emit selected(m_id);
	}
	else
	{
		m_frames[m_currentFrame]->setSelected(select);
	}
}

void KTLayerExposure::otherSelected(int id)
{
	if(m_id != id)
	{
		setSelected(false);
// 		m_header->slotSetDescription();
	}
}

void KTLayerExposure::insertFrame(int id)
{
	
	ESFrame *frame = new ESFrame( tr( "Drawing " ) + QString::number( m_id ) + QString( "-" ) + QString::number( m_useFrame + 1 ), id , this);
	frame->setUsed( true );
// 	m_layout->insertWidget(id,frame);
	m_layout->insertWidget(id
			+1,frame, 10);
// 	this->repaint(true);
	m_frames.insert( m_frames.find(m_frames[m_currentFrame])++, frame ) ;
	connect( frame, SIGNAL(clicked(int, int, int, int )), this, SLOT(frameSelect(int, int, int, int)));
	connect(this, SIGNAL(frameSelected(int )), frame, SLOT(otherSelected(int)));
	frameSelect(id,0,0,0);
	frame->show();
	
}


void KTLayerExposure::setUseFrames(int id)
{
	if(id == m_useFrame)
	{
		m_useFrame++;
		m_frames[m_useFrame]->setUsed( true );
		frameSelect( m_useFrame, 0, 0, 0);
	}
	else if(m_frames[m_currentFrame]->isUsed())
	{
		for(int i = id; i <  m_frames.count() ; i++)
		{
			m_frames[i]->setId(i+1);
		}
		insertFrame(m_currentFrame);
		m_useFrame++;
	}
	else
	{
		for(int i = m_useFrame; i <= id; i++)
		{
			m_frames[i]->setUsed( true );
		}
		m_useFrame = id;
	}
}


