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

	setCaption(tr("The Time line"));
	
	m_container = new KTVHBox(this, false);
	
	addChild(m_container/*, Qt::AlignTop*/);
	
	m_splitter = new QSplitter( m_container );
	
	m_splitter->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	
// 	m_splitter->setMinimumHeight(100);
// 	m_container->addWidget(m_splitter);
	
	m_layerManager = new KTLayerManager( m_splitter );
	m_splitter->addWidget(m_layerManager);
// 	m_layerManager->resize( 590, m_layerManager->height() );
	

	m_sequenceManager = new KTFrameSequenceContainer(m_splitter);
	m_splitter->addWidget(m_sequenceManager);
	
	connect(m_layerManager, SIGNAL(actionSelected(int)), this, SLOT(execAction(int)));
	
	// Mover scrolls simetricamente
	connect( m_sequenceManager->manager()->verticalScrollBar(), SIGNAL( valueChanged( int ) ), m_layerManager->verticalScrollBar(), SLOT( setValue( int ) ) );
	
	connect( m_layerManager->verticalScrollBar(), SIGNAL( valueChanged( int ) ), m_sequenceManager->manager()->verticalScrollBar(), SLOT( setValue( int ) ) );
	
	connect(m_sequenceManager->manager(), SIGNAL( itemSelected(int )), this, SLOT(selectCurrentLayer(int)));
	connect(m_layerManager->layerSequence(), SIGNAL( itemSelected(int )), this, SLOT(selectCurrentLayer(int)));

// 	show();

// 	m_container->setMinimumHeight( m_container->sizeHint().height() );

	m_splitter->setSizes( QList<int>() << 190 << 590 );
	
	setupPropertiesBar();
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

void KTTimeLine::createLayer(const QString &name, bool toEnd)
{
	m_layerManager->layerSequence()->createNewLayer(name, toEnd);
	m_sequenceManager->manager()->insertFrameSequence();
}

void KTTimeLine::removeCurrentLayer()
{
	m_layerManager->layerSequence()->removeLayer();
	m_sequenceManager->manager()->removeFrameSequence();
}

void KTTimeLine::selectCurrentLayer(int index)
{
	m_sequenceManager->manager()->selectItem(index);
	
	m_layerManager->layerSequence()->selectItem(index);
	
	emit layerSelected( index);
	
	m_editLayer->setText(QString::number(index));
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
