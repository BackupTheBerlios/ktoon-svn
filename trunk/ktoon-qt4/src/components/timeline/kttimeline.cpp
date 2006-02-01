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

#include <QList>

#include "kttimeline.h"
#include "ktapplication.h"
#include "ktdebug.h"

KTTimeLine::KTTimeLine(QWidget *parent) : KTModuleWidgetBase(parent, "KTTimeLine")
{
	KTINIT;

	setCaption(tr("The time line"));
	
	m_container = new QStackedWidget(this);
// 	connect(m_container, SIGNAL(currentChanged (int)), this, SIGNAL(requestChangeScene( int )));
	
	addChild(m_container);
	
	setupPropertiesBar();
}

void KTTimeLine::addScene(const QString &name)
{
	QSplitter *m_splitter = new QSplitter( m_container );
	
	m_splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
	KTLayerManager *m_layerManager = new KTLayerManager( m_splitter );
	m_splitter->addWidget(m_layerManager);

	KTFrameSequenceContainer *m_sequenceManager = new KTFrameSequenceContainer(m_splitter);
	m_splitter->addWidget(m_sequenceManager);
	
	connect(m_layerManager, SIGNAL(actionSelected(int)), this, SLOT(execAction(int)));
	
	// Mover scrolls simetricamente
	connect( m_sequenceManager->manager()->verticalScrollBar(), SIGNAL( valueChanged( int ) ), m_layerManager->verticalScrollBar(), SLOT( setValue( int ) ) );
	
	connect( m_layerManager->verticalScrollBar(), SIGNAL( valueChanged( int ) ), m_sequenceManager->manager()->verticalScrollBar(), SLOT( setValue( int ) ) );
	
	/***/
	connect(m_sequenceManager->manager(), SIGNAL( itemSelected(int )), this, SLOT(selectCurrentLayer(int)));
// 	connect(m_layerManager->layerSequence(), SIGNAL( itemSelected(int )), this, SLOT(selectCurrentLayer(int)));
	/***/
	
	m_splitter->setSizes( QList<int>() << 250 << 700 );
	
	m_container->addWidget(m_splitter);
	m_container->setCurrentWidget(m_splitter);
	
	connect(m_sequenceManager->manager(), SIGNAL(requestInsertFrame(bool)), this, SIGNAL(requestInsertFrame( bool )));
	connect(m_sequenceManager->manager(), SIGNAL(requestInsertLayer()), this, SIGNAL(requestInsertLayer( )));
	
	connect(m_sequenceManager, SIGNAL(frameSelected(int,int)), this, SLOT(emitFrameSelected( int,int )));
	
	connect(m_sequenceManager->manager(), SIGNAL(layerVisibilityChanged(int, bool)), this, SIGNAL(layerVisibilityChanged( int,bool )));
	
	connect(m_sequenceManager->manager(), SIGNAL(layerSelected(int)), this, SIGNAL(layerSelected( int )));
	
	connect(m_sequenceManager->manager(), SIGNAL(requestCopyFrame(int)), this, SIGNAL(requestCopyFrame( int )));
	connect(m_sequenceManager->manager(), SIGNAL(requestPasteFrame(int)), this, SIGNAL(requestPasteFrame( int )));
	
	connect(m_sequenceManager->manager(), SIGNAL(requestMoveFrame(bool)), this, SIGNAL(requestMoveFrame( bool )));
	
	connect(m_sequenceManager->manager(), SIGNAL(requestRemoveFrame()), this, SIGNAL(requestRemoveFrame( )));
	connect(m_sequenceManager->manager(), SIGNAL(requestLockFrame()), this, SIGNAL(requestLockFrame( )));
	
	connect(m_sequenceManager->manager(), SIGNAL(requestRemoveLayer()), this, SIGNAL(requestRemoveLayer( )));
}

void KTTimeLine::setScene(int index)
{
	m_container->setCurrentIndex(index);
}

void KTTimeLine::removeCurrentLayer()
{
	currentLayerManager()->removeLayer();
	currentFrameContainer()->removeCurrentLayer();
}

void KTTimeLine::moveFrame(bool up)
{
	
}

void KTTimeLine::removeCurrentFrame()
{
	
}

void KTTimeLine::lockCurrentFrame()
{
	
}

void KTTimeLine::setCurrentCell(int layer, int frame)
{
	KT_FUNCINFO;
	SHOW_VAR(layer);
	SHOW_VAR(frame);
	currentLayerManager()->selectLayer( layer );
	currentFrameContainer()->selectCell(layer, frame);
}

void KTTimeLine::setupPropertiesBar()
{
	m_propertiesBar = new KTVHBox(this, Qt::Horizontal);
	m_propertiesBar->boxLayout()->setMargin(0);
	m_propertiesBar->boxLayout()->setSpacing(5);
	
	m_propertiesBar->setFrameStyle( QFrame::Box | QFrame::Raised );
	m_propertiesBar->boxLayout()->setAlignment(Qt::AlignLeft);
	
	m_propertiesBar->setMaximumHeight(20);
	m_propertiesBar->setMinimumHeight(20);
	
	QLabel *m_fps = new QLabel("<b><i>FPS: </i></b>", m_propertiesBar);
	KTELabel *m_editFPS = new KTELabel("24", m_propertiesBar);
	
	m_editFPS->setValidator(new QIntValidator(0,1000, m_editFPS));
	
	m_editFPS->setMidLineWidth(1);
	m_editFPS->setLineWidth(1);
	m_editFPS->setFrameStyle(QFrame::Panel | QFrame::Raised);
	m_editFPS->setMaximumWidth(50);
	
	connect(m_editFPS, SIGNAL(edited(const QString &)), this, SLOT(emitNewFPS(const QString &)));
	
// 	setStretchFactor( m_editFPS, 1 );
	
	QLabel *m_time = new QLabel("<b><i>Time: </i></b>", m_propertiesBar);
	KTELabel *m_editTime = new KTELabel("24", m_propertiesBar);
	m_editTime->setMidLineWidth(1);
	m_editTime->setLineWidth(1);
	m_editTime->setFrameStyle(QFrame::Panel | QFrame::Raised);
	m_editTime->setMaximumWidth(50);
	
	QLabel *m_layer = new QLabel("<b><i>Current Layer: </i></b>", m_propertiesBar);
	m_editLayer = new KTELabel("0", m_propertiesBar);
// 	m_editLayer->setValidator(new QIntValidator(0,100, m_editFPS));
	
	m_editLayer->setMidLineWidth(1);
	m_editLayer->setLineWidth(1);
	m_editLayer->setFrameStyle(QFrame::Panel | QFrame::Raised);
	m_editLayer->setMaximumWidth(50);
	
	QLabel *m_frame = new QLabel("<b><i>Current Frame:</i></b> ", m_propertiesBar);
	KTELabel *m_editFrame = new KTELabel("0", m_propertiesBar);
	m_editFrame->setMidLineWidth(1);
	m_editFrame->setLineWidth(1);
	m_editFrame->setFrameStyle(QFrame::Panel | QFrame::Raised);
	m_editFrame->setMaximumWidth(50);
	
// 	setStretchFactor( m_editFrame, 1 );
	
	new KTELabel("",m_propertiesBar);
	
	addChild(m_propertiesBar);
}


KTTimeLine::~KTTimeLine()
{
	KTEND;
}

void KTTimeLine::execAction(int action)
{
	KT_FUNCINFO;
	SHOW_VAR(action);
	switch(action)
	{
		case KTLayerManager::InsertLayer:
		{
			emit requestInsertLayer();
		}
		break;
		case KTLayerManager::RemoveLayer:
		{
			emit requestRemoveLayer();
		}
		break;
		
	}
}

KTLayerManager *KTTimeLine::currentLayerManager()
{
	QSplitter *splitter = qobject_cast<QSplitter *>(m_container->currentWidget());
	if ( splitter )
	{
		return qobject_cast<KTLayerManager *>(splitter->widget(0));
	}
	
	return 0;
}

KTFrameSequenceContainer *KTTimeLine::currentFrameContainer()
{
	QSplitter *splitter = qobject_cast<QSplitter *>(m_container->currentWidget());
	
	if ( splitter )
	{
		return qobject_cast<KTFrameSequenceContainer *>(splitter->widget(1));
	}
	
	return 0;
}

void KTTimeLine::insertLayer(const QString &name, bool toEnd)
{
	currentLayerManager()->createNewLayer(name, toEnd);
// 	currentFrameContainer()->manager()->insertFrameSequence();
	currentFrameContainer()->addLayer();
}

void KTTimeLine::selectCurrentLayer(int pos)
{
	KT_FUNCINFO;
	currentFrameContainer()->selectLayer(pos);
	currentLayerManager()->selectLayer( pos );
	
	emit layerSelected( pos );
	
	m_editLayer->setText(QString::number(pos));
}

void KTTimeLine::emitNewFPS(const QString &value)
{
	bool ok = false;
	
	int intValue = value.toInt(&ok);
	
	if ( ok )
	{
		ktWarning() << "Changing fps to " << intValue;
		emit requestChangeFPS( intValue);
	}
	else
	{
		ktError() << "Incorrect FPS value";
	}
}

void KTTimeLine::insertFrame(int layerId, const QString &name, bool addToEnd )
{
	KT_FUNCINFO;
	currentFrameContainer()->addFrameToLayer( layerId );

}

void KTTimeLine::emitFrameSelected(int layer, int frame)
{
	KT_FUNCINFO;
	currentLayerManager()->selectLayer( layer );
	currentFrameContainer()->selectLayer( layer );
	
	emit frameSelected( layer, frame);
}
