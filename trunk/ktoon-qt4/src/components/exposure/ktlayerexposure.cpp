/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                  *
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
#include "ddebug.h"

KTLayerExposure::KTLayerExposure(const QString &initial_text, int id, int numFrame, QWidget *parent)
	: QFrame(parent) , m_id(id), m_currentFrame(0)
{
	DINIT;
	m_layout = new QBoxLayout(QBoxLayout::TopToBottom, this );
	m_layout->setMargin(0);
	m_layout->setSpacing(0);
	m_header = new ESLayer(initial_text,this);
	connect( m_header, SIGNAL(clicked(bool, QMouseEvent*)), this, SLOT(setSelected(bool, QMouseEvent*)));
	connect( m_header, SIGNAL(renamed( const QString & )), this, SLOT(emitRequestRenameLayer(const QString &)));
	connect( m_header, SIGNAL(visibilityChanged(bool)), this, SLOT(setLayerVisible(bool)));
	
	m_layout->addWidget(m_header);
	
	for(int i = 0; i < numFrame; i++)
	{
		ESFrame *frame = new ESFrame( i , this);
		m_layout->addWidget(frame);
		m_frames.append(frame);
		connect( frame, SIGNAL(clicked(int, int, int, int )), this, SLOT(frameSelect(int, int, int, int)));
		connect(this, SIGNAL(frameSelected(int )), frame, SLOT(otherSelected(int)));
		connect( frame, SIGNAL(renamed( int, const QString&)), this, SLOT(emitRequestRenameFrame(int, const QString&)));
	}
	m_useFrame = 0;
	createMenuRight();
	
}

void KTLayerExposure::createMenuRight()
{
	
	menuLayer = new QMenu(this);
	//FIXME: implemtentar rename, remove slots
	menuLayer->addAction( tr( "Rename Layer"), this, SLOT(rename()));
	menuLayer->addAction( tr( "Remove this Layer"), this, SLOT(remove()));
	
	menuLayer->addSeparator();
	menuLayer->addAction( tr( "Insert Frames" ),this, SLOT(insertFrames())  );
	menuLayer->addAction( tr( "Remove Frame" ),this, SLOT(removeCurrentFrame()) );
}


KTLayerExposure::~KTLayerExposure()
{
	DEND;
	qDeleteAll(m_frames);
}

void KTLayerExposure::frameSelect(int id, int button, int x, int y)
{
	if(id < 0)
	{
		id = 0;
	}
	m_currentFrame = id;
	setSelected(true);
	m_header->animateClick();
	emit frameSelected(id);
	emit clicked( visualIndex( id), m_id, button, x, y);
	if(m_useFrame == id && !(m_frames[id]->isUsed()) && button == Qt::LeftButton)
	{
		emit requestInsertFrame(true);
	}
}

void KTLayerExposure::setSelected(bool select, QMouseEvent *e)
{
	m_selected = select;
	m_header->setSelected(select);
	m_header->slotSetDescription();
	if(select)
	{
		m_frames[m_currentFrame]->setSelected(select);
		emit selected(m_id);
	}
	else
	{
		m_frames[m_currentFrame]->setSelected(select);
	}
	if(e)
	{
		if(e->button() == Qt::RightButton)
		{
			menuLayer->exec(QPoint(e->globalX(),e->globalY()));
		}
	}
	
}

void KTLayerExposure::otherSelected(int id)
{
	if(m_id != id)
	{
		setSelected(false);
	}
}


void KTLayerExposure::insertFrame(int id, const QString &text)
{

	ESFrame *frame = new ESFrame(  id , this);
	m_layout->insertWidget(id+1,frame, 10);
	
	m_frames.insert(id, frame);
	connect( frame, SIGNAL(clicked(int, int, int, int )), this, SLOT(frameSelect(int, int, int, int)));
	connect(this, SIGNAL(frameSelected(int )), frame, SLOT(otherSelected(int)));
	connect( frame, SIGNAL(renamed( int, const QString&)), this, SLOT(emitRequestRenameFrame(int, const QString&)));
	frame->show();
}

void KTLayerExposure::addFrame(const QString &text ) 
{
	int id = m_frames.count();
	
	
	ESFrame *frame = new ESFrame( id, this);
	m_layout->insertWidget(id, frame, 10);
	m_frames.insert(id, frame);
	connect( frame, SIGNAL(clicked(int, int, int, int )), this, SLOT(frameSelect(int, int, int, int)));
	connect(this, SIGNAL(frameSelected(int )), frame, SLOT(otherSelected(int)));
	connect( frame, SIGNAL(renamed( int, const QString&)), this, SLOT(emitRequestRenameFrame(int, const QString&)));
	frame->show();
}



void KTLayerExposure::swapFrames(int id1, int id2) 
{
	m_layout->removeWidget(m_frames[id1]);
	m_layout->removeWidget(m_frames[id2]);
	m_layout->insertWidget(id1+1, m_frames[id2], 10);
	m_layout->insertWidget(id2+1, m_frames[id1], 10);
}

void KTLayerExposure::setUseFrames(const QString &name, bool addedToEnd)
{
	if( addedToEnd )
	{
		if(m_useFrame < m_frames.count())
		{
			m_frames[m_useFrame]->setName(name);
			m_frames[m_useFrame]->setUsed( true );
			frameSelect( m_useFrame, 0, 0, 0);
			m_useFrame++;
		}
	}
	else
	{
		
		m_layout->removeWidget(m_frames[m_useFrame] );
		m_layout->insertWidget( m_layout->indexOf(m_frames[m_currentFrame])+1, m_frames[m_useFrame], 10);
		m_frames[m_useFrame]->setName(name);
		m_frames[m_useFrame]->setUsed( true );
		frameSelect( m_useFrame, 0, 0, 0);
		m_useFrame++;
	}
}

void KTLayerExposure::insertFrames()
{
	D_FUNCINFO;
	if ( m_frames.count() > 0 )
	{
		ESFrame *esFrame = m_frames[m_currentFrame];
		if(esFrame )
		{
			if( (esFrame->isUsed() && m_selected ) || m_currentFrame == 0)
			{
// 				dDebug() << "remove frame " << (m_layout->indexOf(esFrame) == m_layout->indexOf(m_frames[m_useFrame])-1) << "||" << (m_useFrame == 0);
				if(m_layout->indexOf(m_frames[m_currentFrame]) == m_layout->indexOf(m_frames[m_useFrame])-1 || m_useFrame == 0 )
				{
					emit requestInsertFrame(true);
				}
				else 
				{
					emit requestInsertFrame(false);
				}
				
			}
			else if(  m_useFrame-1 < m_currentFrame )
			{
				int end = m_currentFrame;
				for(int i = m_useFrame; i <= end; i++)
				{
					emit requestInsertFrame(true);
				}
			}
		}
	}
}



void KTLayerExposure::removeFrame(int id)
{
	int pos = m_layout->indexOf(m_frames[id]) -1;
	ESFrame *frame = m_frames[id];
	if(frame  && m_frames[id]->isUsed() && m_frames[id]->isSelected())
	{
		m_layout->removeWidget(m_frames[id]);
		m_frames[id]->hide();
		delete m_frames.takeAt(id);
		for( int i = id; i < m_frames.count(); i++)
		{
			m_frames[i]->setId(m_frames[i]->id()-1);
		}
		if(pos != 0)
		{
			frameSelect(pos-1);
		}
		else
		{
			frameSelect(pos);
		}
		m_useFrame--;
		insertFrame(m_frames.last()->id()+1, "");
	}
}

void KTLayerExposure::moveCurrentFrameUp()
{
	int pos = m_layout->indexOf(m_frames[m_currentFrame]);
	if(pos > 1)
	{
		QString name =  m_frames[m_currentFrame]->name();
		QString name2 = m_frames[m_currentFrame-1]->name();
		m_frames[m_currentFrame]->blockSignals(true);
		m_frames[m_currentFrame-1]->blockSignals(true);
		m_frames[m_currentFrame]->setName(name2);
		m_frames[m_currentFrame-1]->setName(name);
		m_frames[m_currentFrame-1]->setSelected(true);
		m_frames[m_currentFrame]->setSelected(false);
		m_frames[m_currentFrame]->blockSignals(false);
		m_frames[m_currentFrame-1]->blockSignals(false);
		m_currentFrame = m_currentFrame-1;
	}
}

void KTLayerExposure::moveCurrentFrameDown()
{
	int pos = m_layout->indexOf(m_frames[m_currentFrame]);
	int idFrame = m_frames[pos+1]->id()-1;
	
	if( m_frames[idFrame-1]->isUsed())
	{
		QString name =  m_frames[m_currentFrame]->name();
		QString name2 = m_frames[idFrame]->name();
		m_frames[m_currentFrame]->blockSignals(true);
		m_frames[idFrame]->blockSignals(true);
		m_frames[m_currentFrame]->setName(name2);
		m_frames[idFrame]->setName(name);
		m_frames[idFrame]->setSelected(true);
		m_frames[m_currentFrame]->setSelected(false);
		m_frames[m_currentFrame]->blockSignals(false);
		m_frames[idFrame]->blockSignals(false);
		m_currentFrame = idFrame;
	}
}

void KTLayerExposure::lockFrame()
{
	if(m_frames[m_currentFrame]->isLocked())
	{
		m_frames[m_currentFrame]->setLocked(false);
	}
	else
	{
		m_frames[m_currentFrame]->setLocked(true);
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

void KTLayerExposure::setName(const QString & name)
{
	m_header->setText(name);
	m_header->slotSetDescription();
}

void KTLayerExposure::renameCurrentFrame()
{
	m_frames[m_currentFrame]->slotSendDoubleClickEvent();
}

void KTLayerExposure::removeCurrentFrame()
{
	removeFrame(m_currentFrame);
}

bool KTLayerExposure::currentFrameIsUsed()
{
	return m_frames[m_currentFrame]->isUsed();
}

int KTLayerExposure::numUsedFrame()
{
	return m_useFrame;
}

void KTLayerExposure::applyAction(int action)
{
	switch(action)
	{

		case RemoveThisFrame:
		{
			removeCurrentFrame();
			break;
		}
		case InsertFrames:
		{
			insertFrames();
			break;
		}
		
		case RenameLayer:
		{
			break;
		}
		
		case RemoveThisLayer:
		{
// 			emit removed(m_id);
			break;
		}
	}
}



void KTLayerExposure::setLayerVisible( bool value)
{
	emit (visibilityChanged( m_id, value));
}

int KTLayerExposure::id()
{
	return m_id;
}

int  KTLayerExposure::currentFrame()
{
	return m_currentFrame;
}

ListOfFrames KTLayerExposure::frames()
{
	return m_frames;
}

void  KTLayerExposure::emitRequestRenameFrame(int indexFrame, const QString& name)
{
	dDebug() << "emit (requestRenameFrame(" << m_id << "," <<  indexFrame << "," << name <<"))";
	emit (requestRenameFrame(m_id, indexFrame, name));
}

void KTLayerExposure::emitRequestRenameLayer(const QString  &newName)
{
	dDebug() << "emit(requestRenameLayer( " << m_id << "," << newName <<"))";
	emit(requestRenameLayer(m_id, newName));
}


int KTLayerExposure::visualIndex(int logicalIndex)
{
	return m_layout->indexOf ( m_frames[logicalIndex])-1;
}

int KTLayerExposure::logicalIndex(int visualIndex)
{
	return m_frames.indexOf (static_cast<ESFrame*>(m_layout->itemAt( visualIndex+1)->widget()));
}

