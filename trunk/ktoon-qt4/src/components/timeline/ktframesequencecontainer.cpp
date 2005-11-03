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
	layout()->setMargin(5);
	
	m_ruler = new KTTLRuler(this);
	m_ruler->setMaximumHeight(20);
	m_ruler->setMinimumHeight(20);
	
	m_manager = new KTFrameSequenceManager(this);
	
	setupPropertiesBar();
}


KTFrameSequenceContainer::~KTFrameSequenceContainer()
{
}

void KTFrameSequenceContainer::setupPropertiesBar()
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
	
	setStretchFactor( m_editFPS, 1 );
	
	QLabel *m_time = new QLabel("<b><i>Time: </i></b>", m_propertiesBar);
	KTELabel *m_editTime = new KTELabel("24", m_propertiesBar);
	m_editTime->setMidLineWidth(1);
	m_editTime->setLineWidth(1);
	m_editTime->setFrameStyle(QFrame::Panel | QFrame::Raised);
	m_editTime->setMaximumWidth(50);
	
	QLabel *m_layer = new QLabel("<b><i>Current Layer: </i></b>", m_propertiesBar);
	KTELabel *m_editLayer = new KTELabel("0", m_propertiesBar);
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
	
	setStretchFactor( m_editFrame, 1 );
	
	new KTELabel("",m_propertiesBar);
}

KTFrameSequenceManager *KTFrameSequenceContainer::manager()
{
	return m_manager;
}

void KTFrameSequenceContainer::emitNewFPS(const QString &value)
{
	bool ok = false;
	
	int intValue = value.toInt(&ok);
	
	if ( ok )
	{
		ktWarning() << "Changing fps to " << intValue;
		emit fpsChanged( intValue);
	}
	else
	{
		ktError() << "Incorrect FPS value";
	}
}
