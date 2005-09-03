/***************************************************************************
 *   Copyright (C) 2005 by Jorger Cuadrado                                 *
 *   kuadrosx@toonka.com                                                   *
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
#include "ktdebug.h"

KTLayerExposure::KTLayerExposure(const QString &initial_text, int id, int numFrame, QWidget *parent, const char *name)
	: QFrame(parent, name) , m_id(id), m_currentFrame(0)
{
	KTINIT;
	m_layout = new QBoxLayout(this, QBoxLayout::TopToBottom);
	m_header = new ESLayer(initial_text,this);
	connect( m_header, SIGNAL(clicked()), this, SLOT(setSelected()));
	m_layout->addWidget(m_header);
	m_frames.setAutoDelete(true);
	for(int i = 0; i < numFrame; i++)
	{
		ESFrame *frame = new ESFrame( tr( "Drawing " ) + QString::number( m_id ) + QString( "-" ) + QString::number( i + 1 ), i , this);
		m_layout->addWidget(frame);
		m_frames.append(frame);
		connect( frame, SIGNAL(clicked(int, int, int, int )), this, SLOT(frameSelect(int, int, int, int)));
		connect(this, SIGNAL(frameSelected(int )), frame, SLOT(otherSelected(int)));
// 		frame->resize(frame->sizeHint());
	}
	m_useFrame = 0;
	m_frames.at(0)->setUsed( true );
	
	menuFrame = new QPopupMenu(this);
	menuFrame->insertItem( tr( "Rename Frame" ), this, SLOT( renameCurrentFrame() ));
	menuFrame->insertItem( tr( "Remove this Frame" ), this, SLOT( removeCurrentFrame() ) );
	menuFrame->insertItem( tr( "Lock this Frame" ), this, SLOT( lockCurrentFrame() ) );
	menuFrame->insertItem( tr( "Copy this Frame" ), this, SLOT( copyCurrentFrame() ) );
	menuFrame->insertItem(tr( "Paste into this Frame" ), this, SLOT( pasteCurrentFrame()));
}

KTLayerExposure::~KTLayerExposure()
{
	KTEND;
}

void KTLayerExposure::frameSelect(int id, int button, int x, int y)
{
	if(id < 0)
		id = 0;
	
	m_currentFrame = id;
	
	if(m_useFrame + 1 == id && !(m_frames.at(id)->isUsed()))
	{
		m_frames.at(id)->setUsed( true );
		m_useFrame++;
	}
	setSelected(true);
	m_header->animateClick();
	emit frameSelected(id);
	emit clicked(id, m_id, button, x, y);

	
	if(button == Qt::RightButton && m_frames.at(m_currentFrame)->isUsed())
	{
		menuFrame->exec(QPoint(x,y));
		//show pupo gx, gy
	}
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
		m_frames.at(m_currentFrame)->setSelected(select);
	}
}

void KTLayerExposure::otherSelected(int id)
{
	if(m_id != id)
	{
		setSelected(false);
	}
}

void KTLayerExposure::insertFrame(int id)
{
	
	ESFrame *frame = new ESFrame( tr( "Drawing " ) + QString::number( m_id ) + QString( "-" ) + QString::number( m_useFrame + 1 ), id , this);
	m_layout->insertWidget(id+1,frame, 10);
	
	m_frames.insert(id, frame);//m_currentFrame, frame ) ;
	connect( frame, SIGNAL(clicked(int, int, int, int )), this, SLOT(frameSelect(int, int, int, int)));
	connect(this, SIGNAL(frameSelected(int )), frame, SLOT(otherSelected(int)));
	frame->show();
}


void KTLayerExposure::invertFrames(int id1, int id2)
{
	m_layout->remove(m_frames.at(id1));
	m_layout->remove(m_frames.at(id2));
	m_layout->insertWidget(id1+1, m_frames.at(id2), 10);
	m_layout->insertWidget(id2+1, m_frames.at(id1), 10);
}


void KTLayerExposure::setUseFrames(int id)
{
	//FIXME:
	if(m_currentFrame == m_useFrame )
	{
		m_useFrame++;
		if(m_useFrame <= m_frames.count()-1)
		{
			m_frames.at(m_useFrame)->setUsed( true );
			frameSelect( m_useFrame, 0, 0, 0);
		}
	}
	else if(m_frames.at(m_currentFrame)->isUsed() && m_useFrame != m_frames.count() )
	{
		m_useFrame++;
		m_layout->remove(m_frames.at(m_useFrame));
		m_layout->insertWidget( m_currentFrame+2, m_frames.at(m_useFrame), 10);
		m_frames.at(m_useFrame)->setUsed( true );
	}
	else
	{
// 		ktDebug(1) << "else " << m_useFrame << " " << m_currentFrame;
		for(int i = m_useFrame; i <= m_currentFrame; i++)
		{
			m_frames.at(i)->setUsed( true );
		}
		m_useFrame = m_currentFrame;
	}
}

void KTLayerExposure::removeFrame(int id)
{
	int pos = m_layout->findWidget(m_frames.at(id));
	//FIXME:
	if(m_useFrame > 0)
	{
		if(m_frames.at(m_currentFrame)->isUsed() && m_frames.at(m_currentFrame)->isSelected())
		{
			m_layout->remove(m_frames.at(m_currentFrame));
			m_frames.remove(m_currentFrame);
			for( int i = m_currentFrame; i < m_frames.count(); i++)
			{
				m_frames.at(i)->setId(i);
			}
			m_useFrame--;
			insertFrame(m_frames.getLast()->id()+1);
// 			int pos = m_layout->findWidget(m_frames.at(id+1));
// 			ktDebug(1) << m_frames.at(pos)->id();
// 			frameSelect( m_frames.at(pos)->id() ,0,0,0);
		}
		else if(m_selected)
		{
			
			m_layout->remove(m_frames.at(m_useFrame));
			m_frames.remove(m_useFrame);
			for( int i = m_useFrame; i < m_frames.count(); i++)
			{
				m_frames.at(i)->setId(i);
			}
			ktDebug(1) << "else " << m_useFrame << " " << m_currentFrame;
// 			m_frames.at(m_currentFrame)->setSelected(true);
			frameSelect( m_currentFrame ,0,0,0);
			
			m_useFrame--;
			insertFrame(m_frames.getLast()->id()+1);
			
// 			frameSelect( m_frames.at()->id() ,0,0,0);
		}
		
// 			pos = m_layout->findWidget(m_frames.at(id));
// 			/*int */pos = m_layout->findWidget(m_frames.at(m_currentFrame));
// 				if(pos > 0)
// 				{
// 			ktDebug(1) << m_frames.at(pos)->id();
// 			frameSelect( m_frames.at(pos-1)->id() ,0,0,0);}
// 	 	}
	}
}

void KTLayerExposure::moveCurrentFrameUp()
{
	int pos = m_layout->findWidget(m_frames.at(m_currentFrame));
// 	ktDebug(1) << "moverCurrentFrameUp " << pos << " " << m_currentFrame ;
	if(pos != 1)
	{
		if(m_frames.at(m_currentFrame)->isUsed())
		{
			m_layout->remove(m_frames.at(m_currentFrame));
			m_layout->insertWidget(pos-1, m_frames.at(m_currentFrame), 10);
		}
	}
}

void KTLayerExposure::moveCurrentFrameDown()
{
	int pos = m_layout->findWidget(m_frames.at(m_currentFrame));
	int idFrame = m_frames.at(pos+1)->id();
	if( m_frames.at(idFrame-1)->isUsed())
	{
		m_layout->remove(m_frames.at(m_currentFrame));
		m_layout->insertWidget(pos+1, m_frames.at(m_currentFrame), 10);
	}
}

void KTLayerExposure::lockFrame(int id)
{
	if(m_frames.at(m_currentFrame)->isLocked())
	{
		m_frames.at(m_currentFrame)->setLocked(false);
	}
	else
	{
		m_frames.at(m_currentFrame)->setLocked(true);
	}
}

bool KTLayerExposure::isSelected()
{
	return m_selected;
}

void KTLayerExposure::setId(int id)
{
	m_id = id;
}

void KTLayerExposure::renameCurrentFrame()
{
	m_frames.at(m_currentFrame)->slotSendDoubleClickEvent();
}

void KTLayerExposure::removeCurrentFrame()
{
	removeFrame(m_currentFrame);
}

void KTLayerExposure::lockCurrentFrame()
{
	lockFrame(m_currentFrame);
}

void KTLayerExposure::copyCurrentFrame()
{
	
}

void KTLayerExposure::pasteCurrentFrame()
{
	
}

bool KTLayerExposure::currentFrameIsUsed()
{
	return m_frames.at(m_currentFrame)->isUsed();
}

int KTLayerExposure::useFrame()
{
	return m_useFrame;
}

