/***************************************************************************
 *   Copyright (C) 2005 by Jorge Cuadrado                                 *
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
// #include "status.h"
#include "ktdebug.h"

KTLayerExposure::KTLayerExposure(const QString &initial_text, int id, int numFrame, QWidget *parent)
	: QFrame(parent) , m_id(id), m_currentFrame(0)
{
	KTINIT;
	m_layout = new QBoxLayout(this, QBoxLayout::TopToBottom);
	m_header = new ESLayer(initial_text,this);
	connect( m_header, SIGNAL(clicked(bool, QMouseEvent*)), this, SLOT(setSelected(bool, QMouseEvent*)));
	connect( m_header, SIGNAL(renamed( const QString & )), this, SLOT(changedName(const QString &)));
	
	m_layout->addWidget(m_header);
	
	for(int i = 0; i < numFrame; i++)
	{
		ESFrame *frame = new ESFrame( tr( "Drawing " ) + QString::number( m_id ) + QString( "-" ) + QString::number( i + 1 ), i , this);
		m_layout->addWidget(frame);
		m_frames.append(frame);
		connect( frame, SIGNAL(clicked(int, int, int, int )), this, SLOT(frameSelect(int, int, int, int)));
		connect(this, SIGNAL(frameSelected(int )), frame, SLOT(otherSelected(int)));
		connect( frame, SIGNAL(renamed( int, const QString&)), this, SLOT(frameRename(int, const QString&)));
	}
	m_useFrame = 0;
	m_frames[0]->setUsed( true );
	
// 	menuFrame = new QPopupMenu(this);
// 	menuFrame->insertItem( tr( "Rename Frame" ), RenameFrame, RenameFrame);
// 	menuFrame->insertItem( tr( "Remove this Frame" ), RemoveThisFrame, RemoveThisFrame);
// 	menuFrame->insertItem( tr( "Lock this Frame" ), LockThisFrame, LockThisFrame);
// 	menuFrame->insertItem( tr( "Copy this Frame" ), CopyThisFrame, CopyThisFrame);
// 	menuFrame->insertItem( tr( "Paste into this Frame" ), PasteIntoFrame, PasteIntoFrame);
	
// 	connect(menuFrame, SIGNAL(activated ( int )), this, SLOT(clickMenuFrame(int)));
	createMenuRight();
	
}

void KTLayerExposure::createMenuRight()
{
	menuFrame = new QMenu(this);
	menuFrame->insertItem( tr( "Rename Frame" ), RenameFrame, RenameFrame);
	menuFrame->insertItem( tr( "Remove this Frame" ), RemoveThisFrame, RemoveThisFrame);
	menuFrame->insertItem( tr( "Lock this Frame" ), LockThisFrame, LockThisFrame);
	menuFrame->insertItem( tr( "Copy this Frame" ), CopyThisFrame, CopyThisFrame);
	menuFrame->insertItem( tr( "Paste into this Frame" ), PasteIntoFrame, PasteIntoFrame);
	connect(menuFrame, SIGNAL(activated ( int )), this, SLOT(applyAction(int)));
	
	menuLayer = new QMenu(this);
	menuLayer->insertItem( tr( "Rename Layer" ), RenameLayer);
	menuLayer->insertItem( tr( "Remove this Layer" ), RemoveThisLayer );
	menuLayer->insertSeparator();
	menuLayer->insertItem( tr( "Insert Frames" ), InsertFrames  );
	menuLayer->insertItem( tr( "Remove Frames" ), RemoveThisFrame );
	connect(menuLayer, SIGNAL(activated ( int )), this, SLOT(applyAction(int)));
}


KTLayerExposure::~KTLayerExposure()
{
	KTEND;
	qDeleteAll(m_frames);
}

void KTLayerExposure::frameSelect(int id, int button, int x, int y)
{
	
	if(id < 0)
		id = 0;
	m_currentFrame = id;
	setSelected(true);
	m_header->animateClick();
	emit frameSelected(id);
	emit clicked(id, m_id, button, x, y);
	ktDebug() << "KTLayerExposure: id layer " << m_id << " id frame " <<  id << endl;
	if(m_useFrame + 1 == id && !(m_frames[id]->isUsed()))
	{
		emit requestInsertFrame();
// 		m_frames[id]->setUsed( true );
// 		m_useFrame++;
// 		emit(setUsedFrame(m_frames[id]->name()));

	}
	if(button == Qt::RightButton)
	{
		
		menuFrame->exec(QPoint(x,y));
	}
}

void KTLayerExposure::setSelected(bool select, QMouseEvent *e)
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


void KTLayerExposure::insertFrame(int id, QString text)
{
	if(text == QString::null)
	{
		text = tr( "Drawing " ) + QString::number( m_id ) + QString( "-" ) + QString::number( m_useFrame + 1 );
	}
	ESFrame *frame = new ESFrame( text, id , this);
	m_layout->insertWidget(id+1,frame, 10);
	
	m_frames.insert(id, frame);//m_currentFrame, frame ) ;
	connect( frame, SIGNAL(clicked(int, int, int, int )), this, SLOT(frameSelect(int, int, int, int)));
	connect(this, SIGNAL(frameSelected(int )), frame, SLOT(otherSelected(int)));
	
	connect( frame, SIGNAL(renamed( int, const QString&)), this, SLOT(frameRename(int, const QString&)));
	frame->show();
}

void KTLayerExposure::invertFrames(int id1, int id2)
{
	m_layout->remove(m_frames[id1]);
	m_layout->remove(m_frames[id2]);
	m_layout->insertWidget(id1+1, m_frames[id2], 10);
	m_layout->insertWidget(id2+1, m_frames[id1], 10);
}

void KTLayerExposure::setUseFrames()
{
	if(m_currentFrame == m_useFrame )
	{
		m_useFrame++;
		if(m_useFrame < m_frames.count())
		{
			m_frames[m_useFrame]->setUsed( true );
			frameSelect( m_useFrame, 0, 0, 0);
// 			emit(setUsedFrame(m_frames[m_useFrame]->name()));
		}
	}
	//FIXME:
	else if(m_frames[m_currentFrame]->isUsed() && m_useFrame != m_frames.count() )
	{
		m_useFrame++;
		m_layout->removeWidget(m_frames[m_useFrame]);
		m_layout->insertWidget( m_layout->indexOf(m_frames[m_currentFrame])+1, m_frames[m_useFrame], 10);
		m_frames[m_useFrame]->setUsed( true );
// 		emit(setUsedFrame(m_frames[m_useFrame]->name()));
	}
	else
	{
		for(int i = m_useFrame; i <= m_currentFrame; i++)
		{
			m_frames[i]->setUsed( true );
// 			emit(setUsedFrame(m_frames[i]->name()));
		}
		m_useFrame = m_currentFrame;
	}
}

void KTLayerExposure::insertFrames()
{
	if(m_currentFrame == m_useFrame )
	{
// 		m_useFrame++;
		if(m_useFrame+1 < m_frames.count())
		{
			ktDebug() << "Emiting 1" << endl;
			emit requestInsertFrame();
// 			m_frames[m_useFrame]->setUsed( true );
// 			frameSelect( m_useFrame, 0, 0, 0);
// 			emit(setUsedFrame(m_frames[m_useFrame]->name()));
		}
	}
	//FIXME:
	else if(m_frames[m_currentFrame]->isUsed() && m_useFrame != m_frames.count() )
	{
		ktDebug() << "Emiting 2" << endl;
// 		m_useFrame++;
// 		m_layout->removeWidget(m_frames[m_useFrame]);
// 		m_layout->insertWidget( m_layout->indexOf(m_frames[m_currentFrame])+1, m_frames[m_useFrame], 10);
// 		m_frames[m_useFrame]->setUsed( true );
// 		emit(setUsedFrame(m_frames[m_useFrame]->name()));
		emit requestInsertFrame();
	}
	else
	{
		for(int i = m_useFrame; i <= m_currentFrame; i++)
		{
			ktDebug() << "Emiting 3" << endl;
			emit requestInsertFrame();
// 			m_frames[i]->setUsed( true );
// 			emit(setUsedFrame(m_frames[i]->name()));
		}
// 		m_useFrame = m_currentFrame;
	}
}


void KTLayerExposure::removeFrame(int id)
{
	int pos = m_layout->indexOf(m_frames[id]);
	//FIXME:
	
	if(m_useFrame > 0 && m_frames[id]->isUsed())
	{
		if(m_frames[id]->isUsed() && m_frames[id]->isSelected())
		{
			m_layout->remove(m_frames[id]);
			m_frames.removeAt(id);
			for( int i = id; i < m_frames.count(); i++)
			{
// 				ktDebug() << "new id for " << m_frames[i]->id() << " is " << m_frames[i]->id() -1<< endl;
				m_frames[i]->setId(m_frames[i]->id()-1);
				
			}
			frameSelect(id);
			m_useFrame--;
			insertFrame(m_frames.last()->id()+1);
			
// 			emit removedFrame(id);
		}
		else if(m_selected)
		{
// 			emit removedFrame(id);
			m_layout->remove(m_frames[m_useFrame]);
			m_frames.removeAt(m_useFrame);
			for( int i = m_useFrame; i < m_frames.count(); i++)
			{
				m_frames[i]->setId(i);
			}
			
			m_useFrame--;
			frameSelect(m_useFrame);
			insertFrame(m_frames.last()->id()+1);
		}
	}
}

void KTLayerExposure::moveCurrentFrameUp()
{
	int pos = m_layout->indexOf(m_frames[m_currentFrame]);
	if(pos != 1)
	{
		if(m_frames[m_currentFrame]->isUsed())
		{
			m_layout->remove(m_frames[m_currentFrame]);
			m_layout->insertWidget(pos-1, m_frames[m_currentFrame], 10);
		}
	}
}

void KTLayerExposure::moveCurrentFrameDown()
{
	int pos = m_layout->indexOf(m_frames[m_currentFrame]);
	int idFrame = m_frames[pos+1]->id();
	if( m_frames[idFrame-1]->isUsed())
	{
		m_layout->remove(m_frames[m_currentFrame]);
		m_layout->insertWidget(pos+1, m_frames[m_currentFrame], 10);
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

int KTLayerExposure::frameUsed()
{
	return m_useFrame;
}

void KTLayerExposure::applyAction(int action)
{
	switch(action)
	{
		case RenameFrame:
		{
			renameCurrentFrame();
			break;
		}
		case RemoveThisFrame:
		{
			removeCurrentFrame();
			break;
		}
		case LockThisFrame:
		{
			lockFrame();
			break;
		}
		case InsertFrames:
		{
			currentFrameIsUsed();
			break;
		}
		case CopyThisFrame:
		{
			emit copyFrame();
			break;
		}
		
		case PasteIntoFrame:
		{
			emit pasteFrame();
			break;
		}
		
		case RenameLayer:
		{
			
			break;
		}
		
		case RemoveThisLayer:
		{
			emit removed(m_id);
			break;
		}
	}
}

QString KTLayerExposure::textHeader()
{
	return m_header->text();
}

void KTLayerExposure::loadFrames(Layer *layer)
{
	QList<KeyFrame*> keyframes = layer->keyFrames();
	for(int i = 0 ; i < keyframes.count(); i++)
	{
		m_frames[i]->setUsed(true);
		m_frames[i]->setName(keyframes.at(i)->nameKeyFrame());
	}
	
	m_currentFrame = keyframes.count()-1;
	m_useFrame = keyframes.count()-1;
}

void KTLayerExposure::changedName(const QString  &newName)
{
	emit(layerRenamed(m_id, newName));
}

void KTLayerExposure::frameRename(int idFrame, const QString&newName)
{
// 	QList<Layer*> ly = KTStatus -> currentScene() -> getLayers();
// 	ly.at(m_id)->keyFrames().at(idFrame)->setNameKeyFrame(newName);
}

int KTLayerExposure::id()
{
	return m_id;
}
